#ifndef CHAT_SERVER_OPTIONS_H
#define CHAT_SERVER_OPTIONS_H

#include "options.h"

class ServerOptions : Options {
public:
    ServerOptions(int arguments_count, char** arguments);
    void parse() final;
    int getPort();
    void printUsage(std::ostream &out);

private:
    int arguments_count;
    char** arguments;

    const int DEFAULT_PORT = 20160;

    int port;
};

#endif //CHAT_SERVER_OPTIONS_H
