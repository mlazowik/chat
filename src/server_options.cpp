#include <iostream>
#include <stdexcept>
#include <string>
#include "server_options.h"

ServerOptions::ServerOptions(int arguments_count, char **arguments) {
    this->arguments_count = arguments_count;
    this->arguments = arguments;

    this->port = this->DEFAULT_PORT;
}

void ServerOptions::parse() {
    if (this->arguments_count > 2) {
        throw std::invalid_argument("too many arguments");
    }

    if (this->arguments_count == 2) {
        this->port = std::stoi(this->arguments[1]);
    }
}

int ServerOptions::getPort() {
    return this->port;
}

std::string ServerOptions::getUsage() {
    return "Usage: server [PORT]";
}