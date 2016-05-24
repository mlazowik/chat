#include <stdexcept>
#include <netinet/in.h>
#include <stream/primitive_type_reader.h>
#include <stream/string_reader.h>
#include <iostream>

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
                this->messageLength = (
                        (PrimitiveTypeReader<uint16_t >*)this->reader
                )->getValue();

                this->messageLength = ntohs(this->messageLength);

                delete this->reader;

                if (this->messageLength > 1000) {
                    throw invalid_message_error("message is too long");
                } else if (this->messageLength == 0) {
                    this->message = "";
                    this->reading = Reading::NOTHING;
                } else {
                    this->reader = getMessageReader();
                    this->reading = Reading::MESSAGE;
                }

                break;
            case Reading::MESSAGE:
                this->message = ((StringReader*)this->reader)->getValue();
                delete this->reader;
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

std::string Connection::getMessage() const {
    return this->message;
}

void Connection::sendMessage(std::string message) const {
    uint16_t len = htons(message.length());

    this->socket.sendChunk((char*)&len, sizeof(len));
    this->socket.sendChunk(message.c_str(), message.length());
}

Reader* Connection::getLengthReader() {
    return new PrimitiveTypeReader<uint16_t>(this->socket);
}

Reader* Connection::getMessageReader() {
    return new StringReader(this->socket, [&](std::string value) {
        return value.length() == this->messageLength;
    });
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