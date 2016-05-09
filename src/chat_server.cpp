#include <unistd.h>
#include <system_error>
#include "chat_server.h"

ChatServer::ChatServer(Socket &serverSocket, IOEvents &events)
        : serverSocket(serverSocket), events(events) {
}

void ChatServer::run() {
    Connection serverConnection(this->serverSocket);

    this->events.registerSocket(serverConnection, [&](Connection &connection, short revents) {
        this->handleServerEvent(serverConnection, revents);
    });

    while (true) {
        this->events.processEvents();
    }
}

void ChatServer::handleServerEvent(Connection &connection, short revents) {
    if (!(revents & POLLIN)) return;

    Socket clientSocket = this->serverSocket.acceptConnection();
    Connection client(clientSocket);
    clients.insert(client);

    this->events.registerSocket(client, [&](Connection &connection, short revents) {
        this->handleClientEvent(connection, revents);
    });
}

void ChatServer::handleClientEvent(Connection &connection, short revents) {
    if (!(revents & (POLLIN | POLLERR))) return;

    char buf[this->BUFFER_SIZE];
    ssize_t rval = read(connection.getDescriptor(), buf, this->BUFFER_SIZE);

    if (rval < 0) {
        this->disconnectClient(connection);
        throw std::system_error(errno, std::system_category());
    }

    if (rval == 0) {
        this->disconnectClient(connection);
    } else {
        for (const Connection &client : clients) {
            if (client != connection) {
                if (write(connection.getDescriptor(), buf, rval) < 0) {
                    throw std::system_error(errno, std::system_category());
                }
            }
        }
    }
}

void ChatServer::disconnectClient(Connection &connection) {
    this->events.deregisterDescriptor(connection);
    connection.destroy();
    this->clients.erase(connection);
}