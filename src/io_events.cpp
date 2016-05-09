#include <exception>
#include <stdexcept>
#include <system_error>
#include "io_events.h"

IOEvents::IOEvents(size_t size) {
    this->size = size;
    this->descriptors = new struct pollfd[size];

    for (size_t i = 0; i < this->size; i++) {
        struct pollfd *descriptor = this->descriptors + i;

        descriptor->fd = -1;
        descriptor->events = POLLIN;
        descriptor->revents = 0;
    }
}

void IOEvents::registerSocket(Socket &socket, std::function<void(Socket&, short)> callback) {
    size_t i = 0;
    struct pollfd *descriptor;

    do {
        descriptor = this->descriptors + i;
        i++;
    } while (i < this->size && descriptor->fd != -1);

    if (i == this->size) {
        throw std::runtime_error("too many sockets");
    }

    i--;

    descriptor->fd = socket.getDescriptor();
    this->callbacks[i] = callback;
    this->sockets.emplace(i, socket);
}

void IOEvents::deregisterSocket(Socket &socket) {
    size_t i = 0;
    struct pollfd *descriptor;

    do {
        descriptor = this->descriptors + i;
        i++;
    } while (i < this->size && descriptor->fd != socket.getDescriptor());

    if (i == this->size) {
        throw std::logic_error("no such socket");
    }

    i--;

    descriptor->fd = -1;
    this->callbacks.erase(i);
    this->sockets.erase(i);
}

void IOEvents::processEvents() {
    int poll_result = poll(this->descriptors, (nfds_t)this->size, 5000);

    if (poll_result < 0) {
        throw std::system_error(errno, std::system_category());
    }

    if (poll_result > 0) {
        for (size_t i = 0; i < this->size; i++) {
            struct pollfd *descriptor = this->descriptors + i;

            if (descriptor->fd != -1 && descriptor->revents != 0) {
                this->callbacks[i](this->sockets[i], descriptor->revents);
            }
        }
    }
}