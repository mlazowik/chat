#include <vector>
#include <parser.h>

#include "server_options.h"

ServerOptions::ServerOptions(std::vector<std::shared_ptr<Parser>> parsers)
        : Options(parsers, 0) { }

std::string ServerOptions::getUsage() const {
    return "Usage: chat_client host [PORT]";
}