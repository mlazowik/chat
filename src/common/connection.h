#ifndef CHAT_CONNECTION_H
#define CHAT_CONNECTION_H

#include <stream/stream_reader.h>
#include <io/descriptor.h>
#include <networking/socket.h>

class invalid_message_error : public std::runtime_error {
public:
    invalid_message_error(const std::string &what)
            : runtime_error(what) {
    }
};

class Connection : public Descriptor {
public:
    Connection();
    Connection(Socket &socket);

    void read();
    bool finishedReading() const;
    std::string getMessage() const;
    void sendMessage(std::string message) const;

private:
    Socket socket;
    Reader *reader;

    uint16_t messageLength;
    std::string message;

    enum class Reading {
        LENGTH,
        MESSAGE,
        NOTHING
    } reading;

    Reader* getLengthReader();
    Reader* getMessageReader();
};

#endif //CHAT_CONNECTION_H
