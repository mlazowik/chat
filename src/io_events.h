#ifndef CHAT_IO_EVENTS_H
#define CHAT_IO_EVENTS_H

#include <cstddef>
#include <poll.h>
#include <functional>
#include <map>

#include "socket.h"

class IOEvents {
public:
    IOEvents(size_t size);
    void registerSocket(Socket &socket, std::function<void(Socket, short)> callback);
    void deregisterSocket(Socket &socket);
    void processEvents();

private:
    struct pollfd *descriptors;
    size_t size;

    std::map<int, std::function<void(Socket, short)>> callbacks;
    std::map<int, Socket> sockets;
};

#endif //CHAT_IO_LOOP_H