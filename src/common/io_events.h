#ifndef CHAT_IO_EVENTS_H
#define CHAT_IO_EVENTS_H

#include <cstddef>
#include <poll.h>
#include <functional>
#include <map>

#include "descriptor.h"

class IOEvents {
public:
    IOEvents(size_t size);
    void registerDescriptor(Descriptor *descriptor,
                            std::function<void(Descriptor *, short)> callback);
    void removeDescriptor(Descriptor *descriptor);
    void processEvents();

private:
    struct pollfd *pollEvents;
    size_t size;

    std::map<size_t, std::function<void(Descriptor*, short)>> callbacks;
    std::map<size_t, Descriptor*> connections;
};

#endif //CHAT_IO_LOOP_H