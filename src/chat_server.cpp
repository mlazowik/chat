#include <unistd.h>
#include <system_error>
#include <netinet/in.h>

#include "chat_server.h"

ChatServer::ChatServer(Socket &serverSocket, IOEvents &events)
        : serverSocket(serverSocket), events(events) {
}

void ChatServer::run() {
    Connection *serverConnection = new Connection(this->serverSocket);

    this->events.registerSocket(serverConnection, [&](Connection *connection, short revents) {
        this->handleServerEvent(serverConnection, revents);
    });

    while (true) {
        this->events.processEvents();
    }
}

void ChatServer::handleServerEvent(Connection *connection, short revents) {
    if (!(revents & POLLIN)) return;

    Socket clientSocket = this->serverSocket.acceptConnection();
    Connection *client = new Connection(clientSocket);
    clients.insert(client);

    this->events.registerSocket(client, [&](Connection *connection, short revents) {
        this->handleClientEvent(connection, revents);
    });
}

void ChatServer::handleClientEvent(Connection *connection, short revents) {
    if (!(revents & (POLLIN | POLLERR))) return;

    try {
        connection->read();
    } catch (invalid_message_error &ex) {
        this->disconnectClient(connection);
        return;
    } catch (stream_closed_error &ex) {
        this->disconnectClient(connection);
        return;
    }

    if (connection->finishedReading()) {
        std::string message;

        message = connection->getMessage();

        for (const Connection *client : clients) {
            if (*client != *connection) {
                uint16_t len = htons(message.length());

                if (write(client->getDescriptor(), (char*)&len, sizeof(len)) < 0) {
                    throw std::system_error(errno, std::system_category());
                }

                if (write(client->getDescriptor(), message.c_str(), message.length()) < 0) {
                    throw std::system_error(errno, std::system_category());
                }
            }
        }
    }
}

void ChatServer::disconnectClient(Connection *connection) {
    this->events.deregisterDescriptor(connection);
    connection->destroy();
    this->clients.erase(connection);
    delete connection;
}