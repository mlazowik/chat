#ifndef CHAT_CONNECTION_H
#define CHAT_CONNECTION_H

#include "socket.h"
#include "stream_reader.h"
#include "descriptor.h"

class Connection : public Desciptor {
public:
    Connection();
    Connection(Socket &socket);

    int getDescriptor() const;
    void destroy();

    bool operator==(const Desciptor &rhs) const;
    bool operator!=(const Desciptor &rhs) const;
    bool operator<(const Desciptor &rhs) const;
    bool operator>(const Desciptor &rhs) const;

    void read();
    bool finishedReading() const;

private:
    Socket socket;
    StreamReader *reader;

    const static size_t BUFFER_LENGTH = 1024;

    uint16_t messageLength;
    char buffer[BUFFER_LENGTH];

    enum class Reading {
        LENGTH,
        MESSAGE,
        NOTHING
    } reading;

    StreamReader* getLengthReader();
    StreamReader* getMessageReader();
};

#endif //CHAT_CONNECTION_H
