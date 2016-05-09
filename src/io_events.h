#ifndef CHAT_IO_EVENTS_H
#define CHAT_IO_EVENTS_H

#include <cstddef>
#include <poll.h>
#include <functional>
#include <map>

#include "connection.h"

class IOEvents {
public:
    IOEvents(size_t size);
    void registerSocket(Connection *connection, std::function<void(Connection*, short)> callback);
    void deregisterDescriptor(Connection *connection);
    void processEvents();

private:
    struct pollfd *pollEvents;
    size_t size;

    std::map<int, std::function<void(Connection*, short)>> callbacks;
    std::map<int, Connection*> connections;
};

#endif //CHAT_IO_LOOP_H