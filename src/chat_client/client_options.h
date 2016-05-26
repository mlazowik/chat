#ifndef CHAT_CLIENT_OPTIONS_H
#define CHAT_CLIENT_OPTIONS_H

#include <options/options.h>

class ClientOptions : public Options {
public:
    ClientOptions(std::vector<std::shared_ptr<Parser>> parsers);
    std::string getUsage() const;
};

#endif //CHAT_CLIENT_OPTIONS_H
