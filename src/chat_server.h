#ifndef CHAT_CHAT_SERVER_H
#define CHAT_CHAT_SERVER_H

#include <set>
#include "socket.h"
#include "io_events.h"
#include "stream_reader.h"
#include "connection.h"

class ChatServer {
public:
    ChatServer(Socket &serverSocket, IOEvents &events);
    void run();

private:
    void handleServerEvent(Connection &connection, short revents);
    void handleClientEvent(Connection &connection, short revents);

    void disconnectClient(Connection &connection);

    Socket serverSocket;
    IOEvents events;

    const int BUFFER_SIZE = 1024;

    std::set<Connection> clients;
};


#endif //CHAT_CHAT_SERVER_H
