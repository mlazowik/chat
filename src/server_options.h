#ifndef CHAT_SERVER_OPTIONS_H
#define CHAT_SERVER_OPTIONS_H

#include <string>
#include <vector>
#include "options.h"

class ServerOptions : Options {
public:
    ServerOptions(std::vector<std::string>, int defaultPort);
    void parse();
    int getPort();
    std::string getUsage();

private:
    std::vector<std::string> arguments;

    int port;
};

#endif //CHAT_SERVER_OPTIONS_H
