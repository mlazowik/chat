#include <stdexcept>

#include "connection.h"

Connection::Connection() {}

Connection::Connection(Socket &socket) : socket(socket) {
    this->reading = Reading::NOTHING;
}

int Connection::getDescriptor() const {
    return this->socket.getDescriptor();
}

void Connection::destroy() {
    this->socket.destroy();
}

void Connection::read() {
    if (finishedReading()) {
        this->reading = Reading::LENGTH;
        this->reader = this->getLengthReader();
    }

    this->reader->readNextChunk();

    if (this->reader->finished()) {
        switch (this->reading) {
            case Reading::LENGTH:
                delete[] this->reader;
                this->reader = getMessageReader();
                this->reading = Reading::MESSAGE;
                break;
            case Reading::MESSAGE:
                delete[] this->reader;
                this->reading = Reading::NOTHING;
                break;
            case Reading::NOTHING:
                throw std::logic_error("we cannot be reading nothing here!");
        }
    }
}

bool Connection::finishedReading() const {
    return this->reading == Reading::NOTHING;
}

StreamReader* Connection::getLengthReader() {
    return new StreamReader(this->socket, (char*)this->messageLength,
                            sizeof(this->messageLength));
}

StreamReader* Connection::getMessageReader() {
    return new StreamReader(this->socket, (char*)this->buffer,
                            this->BUFFER_LENGTH);
}

bool Connection::operator==(const Desciptor &rhs) const {
    return this->getDescriptor() == rhs.getDescriptor();
}

bool Connection::operator!=(const Desciptor &rhs) const {
    return !(*this == rhs);
}

bool Connection::operator<(const Desciptor &rhs) const {
    return this->getDescriptor() < rhs.getDescriptor();
}

bool Connection::operator>(const Desciptor &rhs) const {
    return !(*this < rhs || *this == rhs);
}