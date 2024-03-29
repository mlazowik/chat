#include <cstdlib>
#include <iostream>
#include <cstring>
#include <system_error>

#include "chat_client.h"

ChatClient::ChatClient(Socket &serverSocket, IOEvents &events)
        : serverSocket(serverSocket), events(events) {
    this->clientSocket = new Socket(fileno(stdin));
}

ChatClient::~ChatClient() {
    delete this->clientSocket;
}

void ChatClient::run() {
    this->server = new Connection(this->serverSocket);
    this->client = new Connection(*this->clientSocket);

    this->events.registerDescriptor(
            this->server,
            [&](Descriptor *connection, short revents) {
                this->handleServerEvent((Connection *) connection, revents);
            }
    );

    this->events.registerDescriptor(
            this->client,
            [&](Descriptor *connection, short revents) {
                this->handleClientEvent((Connection *) connection, revents);
            }
    );

    while (true) {
        this->events.processEvents();
    }
}

void ChatClient::handleServerEvent(Connection *connection, short revents) {
    if (!(revents & (POLLIN | POLLHUP))) return;

    try {
        connection->read();
    } catch (invalid_message_error &ex) {
        std::cerr << "Invalid message\n";
        this->disconnectServer(connection);
        exit(100);
    } catch (stream_closed_error &ex) {
        std::cerr << "Server closed connection\n";
        this->disconnectServer(connection);
        exit(100);
    } catch (std::system_error &ex) {
        this->disconnectServer(connection);
        exit(100);
    }

    if (connection->finishedReading()) {
        std::string message = connection->getMessage();
        std::cout << message << "\n";
        fflush(stdout);
    }
}

void ChatClient::handleClientEvent(Connection *connection, short revents) {
    if (!(revents & (POLLIN | POLLHUP))) return;

    char buffer[this->BUFFER_SIZE];

    if (std::fgets(buffer, 1001, stdin) != NULL) {
        size_t lengthWithoutNewline = std::strcspn(buffer, "\n");

        if (!this->lastEndedWithNewline && lengthWithoutNewline == 0) {
            return;
        }
        this->lastEndedWithNewline = lengthWithoutNewline != 1000;
        buffer[lengthWithoutNewline] = '\0';
        std::string message(buffer);
        this->server->sendMessage(message);
    } else {
        if (std::feof(stdin)) {
            this->disconnectServer(server);
            exit(EXIT_SUCCESS);
        }
    }
}

void ChatClient::disconnectServer(Connection *connection) {
    this->events.removeDescriptor(connection);
    connection->destroy();
    delete connection;
}