#include <system_error>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "socket.h"

Socket::Socket() {
    this->descriptor = socket(PF_INET, SOCK_STREAM, 0);

    if (this->descriptor < 0) {
        throw std::system_error(errno, std::system_category());
    }
}

Socket::~Socket() {
    if (close(this->descriptor < 0)) {
        throw std::system_error(errno, std::system_category());
    }
}

int Socket::getDescriptor() {
    return this->descriptor;
}

Socket::Socket(int descriptor) {
    this->descriptor = descriptor;
}

void Socket::setPort(int port) {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(this->descriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
        throw std::system_error(errno, std::system_category());
    }
}

void Socket::startListening() {
    listen(this->descriptor, this->BACKLOG_QUEUE_LENGTH);
}

Socket Socket::acceptConnection() {
    int connectionDescriptor = accept(this->descriptor, (struct sockaddr*)0,
                               (socklen_t*)0);

    if (connectionDescriptor < 0) {
        throw std::system_error(errno, std::system_category());
    }

    return Socket(connectionDescriptor);
}