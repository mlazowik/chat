#ifndef CHAT_SERVER_OPTIONS_H
#define CHAT_SERVER_OPTIONS_H

#include <string>
#include <vector>
#include "../common/options.h"

class ServerOptions : public Options {
public:
    ServerOptions(std::vector<std::string>, int defaultPort);
    void parse();
    int getPort() const;
    std::string getUsage() const;

private:
    std::vector<std::string> arguments;

    int port;
};

#endif //CHAT_SERVER_OPTIONS_H
