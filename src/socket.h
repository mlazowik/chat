#ifndef CHAT_SOCKET_H
#define CHAT_SOCKET_H

#include "stream.h"
#include "descriptor.h"

class Socket : public Stream, public Desciptor {
public:
    Socket();
    int getDescriptor() const;
    void destroy();
    void setPort(int port);
    void startListening();
    Socket acceptConnection();

    bool operator==(const Desciptor &rhs) const;
    bool operator!=(const Desciptor &rhs) const;
    bool operator<(const Desciptor &rhs) const;
    bool operator>(const Desciptor &rhs) const;

    size_t getChunk(void *buffer, size_t bufferSize);
    void sendChunk(void *buffer, size_t bufferSize);

private:
    Socket(int descriptor);

    int descriptor;

    const int BACKLOG_QUEUE_LENGTH = 5;
};



#endif //CHAT_SOCKET_H
