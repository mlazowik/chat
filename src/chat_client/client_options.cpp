#include "client_options.h"

ClientOptions::ClientOptions(std::vector <std::shared_ptr<Parser>> parsers)
        : Options(parsers, 1) { }

std::string ClientOptions::getUsage() const {
    return "Usage: chat_client host [PORT]";
}