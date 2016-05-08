#include <stdexcept>
#include "server_options.h"

ServerOptions::ServerOptions(
        std::vector<std::string> arguments, int defaultPort) {
    this->arguments = arguments;

    this->port = defaultPort;
}

void ServerOptions::parse() {
    if (this->arguments.size() > 2) {
        throw std::invalid_argument("too many arguments");
    }

    if (this->arguments.size() == 2) {
        std::string port_string = this->arguments[1];

        std::string::size_type first_after_number;
        try {
            this->port = std::stoi(port_string, &first_after_number);
        } catch (std::invalid_argument &ex) {
            throw std::invalid_argument(port_string + " is not a number");
        } catch (std::out_of_range &ex) {
            throw std::out_of_range(port_string + " is out of range");
        }
        if (first_after_number != port_string.length()) {
            throw std::invalid_argument(port_string + " is not a number");
        }
    }
}

int ServerOptions::getPort() {
    return this->port;
}

std::string ServerOptions::getUsage() {
    return "Usage: server [PORT]";
}