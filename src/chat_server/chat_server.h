#ifndef CHAT_CHAT_SERVER_H
#define CHAT_CHAT_SERVER_H

#include <set>
#include <io/io_events.h>
#include <networking/socket.h>

#include "common/connection.h"

class ChatServer {
public:
    ChatServer(Socket &serverSocket, IOEvents &events);
    void run();

private:
    void handleServerEvent(Connection *connection, short revents);
    void handleClientEvent(Connection *connection, short revents);

    void disconnectClient(Connection *connection);

    Socket serverSocket;
    IOEvents events;

    const int BUFFER_SIZE = 1024;

    std::set<Connection*> clients;
};


#endif //CHAT_CHAT_SERVER_H
