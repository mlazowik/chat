#include <exception>
#include <stdexcept>
#include <system_error>
#include "io_events.h"

IOEvents::IOEvents(size_t size) {
    this->size = size;
    this->pollEvents = new struct pollfd[size];

    for (size_t i = 0; i < this->size; i++) {
        struct pollfd *descriptor = this->pollEvents + i;

        descriptor->fd = -1;
        descriptor->events = POLLIN;
        descriptor->revents = 0;
    }
}

void IOEvents::registerDescriptor(Descriptor *descriptor,
                                  std::function<void(Descriptor *,
                                                     short)> callback) {
    size_t i = 0;
    struct pollfd *pollEvent;

    do {
        pollEvent = this->pollEvents + i;
        i++;
    } while (i < this->size && pollEvent->fd != -1);

    i--;

    if (pollEvent->fd != -1) {
        throw std::runtime_error("too many descriptors");
    }

    pollEvent->fd = descriptor->getDescriptor();
    this->callbacks[i] = callback;
    this->connections.emplace(i, descriptor);
}

void IOEvents::removeDescriptor(Descriptor *descriptor) {
    size_t i = 0;
    struct pollfd *pollEvent;

    do {
        pollEvent = this->pollEvents + i;
        i++;
    } while (i < this->size && pollEvent->fd != descriptor->getDescriptor());

    i--;

    if (pollEvent->fd != descriptor->getDescriptor()) {
        throw std::logic_error("no such descriptor");
    }

    pollEvent->fd = -1;
    this->callbacks.erase(i);
    this->connections.erase(i);
}

void IOEvents::processEvents() {
    int poll_result = poll(this->pollEvents, (nfds_t)this->size, 5000);

    if (poll_result < 0) {
        throw std::system_error(errno, std::system_category());
    }

    if (poll_result > 0) {
        for (size_t i = 0; i < this->size; i++) {
            struct pollfd *descriptor = this->pollEvents + i;

            if (descriptor->fd != -1 && descriptor->revents != 0) {
                this->callbacks[i](this->connections[i], descriptor->revents);
            }
        }
    }
}