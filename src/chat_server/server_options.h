#ifndef CHAT_SERVER_OPTIONS_H
#define CHAT_SERVER_OPTIONS_H

#include <options/options.h>

class ServerOptions : public Options {
public:
    ServerOptions(std::vector<std::shared_ptr<Parser>> parsers);
    std::string getUsage() const;
};

#endif //CHAT_SERVER_OPTIONS_H
