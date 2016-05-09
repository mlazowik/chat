#ifndef CHAT_CHAT_SERVER_H
#define CHAT_CHAT_SERVER_H

#include <set>
#include "socket.h"
#include "io_events.h"

class ChatServer {
public:
    ChatServer(Socket &serverSocket, IOEvents &events);
    void run();

private:
    void handleServerEvent(Socket &socket, short revents);
    void handleClientEvent(Socket &socket, short revents);

    Socket serverSocket;
    IOEvents events;

    const int BUFFER_SIZE = 1024;

    std::set<Socket> clients;
};


#endif //CHAT_CHAT_SERVER_H
