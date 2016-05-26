#include <unistd.h>
#include <system_error>
#include <iostream>

#include "chat_server.h"

ChatServer::ChatServer(Socket &serverSocket, IOEvents &events)
        : serverSocket(serverSocket), events(events) {
}

void ChatServer::run() {
    Connection *serverConnection = new Connection(this->serverSocket);

    this->events.registerDescriptor(
            serverConnection,
            [&](Descriptor *connection, short revents) {
                this->handleServerEvent(serverConnection, revents);
            }
    );

    while (true) {
        this->events.processEvents();
    }
}

void ChatServer::handleServerEvent(Connection *connection, short revents) {
    if (!(revents & POLLIN)) return;

    Socket clientSocket = this->serverSocket.acceptConnection();
    Connection *client = new Connection(clientSocket);
    clients.insert(client);

    try {
        this->events.registerDescriptor(
                client,
                [&](Descriptor *descriptor, short revents) {
                    this->handleClientEvent((Connection *)descriptor, revents);
                }
        );
    } catch (std::runtime_error &ex) {
        std::cerr << "Cannot accept connection: " << ex.what() << "\n";
        connection->destroy();
        this->clients.erase(connection);
        delete connection;
    }
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
        std::string message = connection->getMessage();

        for (const Connection *client : clients) {
            if (*client != *connection) {
                client->sendMessage(message);
            }
        }
    }
}

void ChatServer::disconnectClient(Connection *connection) {
    this->events.removeDescriptor(connection);
    connection->destroy();
    this->clients.erase(connection);
    delete connection;
}