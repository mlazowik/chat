#include <cstdlib>
#include <iostream>
#include <system_error>

#include "chat_client.h"

ChatClient::ChatClient(Socket &serverSocket, IOEvents &events)
        : serverSocket(serverSocket), events(events) {
    this->clientSocekt = new Socket(fileno(stdin));
}

ChatClient::~ChatClient() {
    delete this->clientSocekt;
}

void ChatClient::run() {
    this->server = new Connection(this->serverSocket);
    this->client = new Connection(*this->clientSocekt);

    this->events.registerConnection(
            this->server,
            [&](Connection *connection, short revents) {
                this->handleServerEvent(connection, revents);
            }
    );

    this->events.registerConnection(
            this->client,
            [&](Connection *connection, short revents) {
                this->handleClientEvent(connection, revents);
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

    std::string message;

    std::getline(std::cin, message);

    if (std::cin.eof()) {
        this->disconnectServer(server);
        exit(EXIT_SUCCESS);
    }

    this->server->sendMessage(message);
}

void ChatClient::disconnectServer(Connection *connection) {
    this->events.deregisterConnection(connection);
    connection->destroy();
    delete connection;
}