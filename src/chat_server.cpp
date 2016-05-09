#include <unistd.h>
#include <system_error>
#include "chat_server.h"

ChatServer::ChatServer(Socket &serverSocket, IOEvents &events)
        : serverSocket(serverSocket), events(events) {
}

void ChatServer::run() {
    this->events.registerSocket(this->serverSocket, [&](Socket &socket, short revents) {
        this->handleServerEvent(socket, revents);
    });

    while (true) {
        this->events.processEvents();
    }
}

void ChatServer::handleServerEvent(Socket &socket, short revents) {
    if (!(revents & POLLIN)) return;

    Socket client = this->serverSocket.acceptConnection();
    clients.insert(client);

    this->events.registerSocket(client, [&](Socket &socket, short revents) {
        this->handleClientEvent(socket, revents);
    });
}

void ChatServer::handleClientEvent(Socket &socket, short revents) {
    if (!(revents & (POLLIN | POLLERR))) return;

    char buf[this->BUFFER_SIZE];
    ssize_t rval = read(socket.getDescriptor(), buf, this->BUFFER_SIZE);

    if (rval < 0) {
        this->disconnectClient(socket);
        throw std::system_error(errno, std::system_category());
    }

    if (rval == 0) {
        this->disconnectClient(socket);
    } else {
        for (const Socket &client : clients) {
            if (client != socket) {
                if (write(client.getDescriptor(), buf, rval) < 0) {
                    throw std::system_error(errno, std::system_category());
                }
            }
        }
    }
}

void ChatServer::disconnectClient(Socket &socket) {
    this->events.deregisterSocket(socket);
    socket.destroy();
    this->clients.erase(socket);
}