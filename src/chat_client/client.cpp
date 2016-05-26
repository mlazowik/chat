#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <options/string_parser.h>
#include <options/number_parser.h>

#include "chat_client.h"
#include "../common/socket.h"
#include "client_options.h"

int main(int argc, char* argv[]) {
    const int DEFAULT_PORT = 20160;

    auto host = std::make_shared<StringParser>(argv[1]);
    auto port = std::make_shared<NumberParser>(StringParser(argv[2]));

    port->setDefaultValue(DEFAULT_PORT);

    ClientOptions options({host, port});

    try {
        options.parse((size_t)argc - 1);
    } catch(std::exception &ex) {
        std::cerr << "Invalid arguments: " << ex.what() << "\n";
        std::cerr << options.getUsage() << "\n";
        exit(EXIT_FAILURE);
    }

    Socket server;

    try {
        server.setPort(port->getValue());
        server.setHost(host->getValue());

        server.connect();

        IOEvents events(2);

        ChatClient client(server, events);
        client.run();
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        exit(100);
    }

    return 0;
}