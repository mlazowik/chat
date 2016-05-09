#ifndef CHAT_SOCKET_H
#define CHAT_SOCKET_H

class Socket {
public:
    Socket();
    int getDescriptor() const;
    void setPort(int port);
    void startListening();
    Socket acceptConnection();
    void destroy();

    bool operator==(const Socket &rhs) const;
    bool operator!=(const Socket &rhs) const;
    bool operator<(const Socket &rhs) const;
    bool operator>(const Socket &rhs) const;

private:
    Socket(int descriptor);

    int descriptor;

    const int BACKLOG_QUEUE_LENGTH = 5;
};



#endif //CHAT_SOCKET_H
