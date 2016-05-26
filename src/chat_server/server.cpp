#include <iostream>
#include <stdexcept>
#include <options/string_parser.h>
#include <options/number_parser.h>

#include "server_options.h"
#include "chat_server.h"
#include "../common/socket.h"
#include "../common/io_events.h"

int main(int argc, char* argv[]) {
    const int DEFAULT_PORT = 20160;

    auto port = std::make_shared<NumberParser>(StringParser(argv[1]));

    port->setDefaultValue(DEFAULT_PORT);

    ServerOptions options({port});

    try {
        options.parse((size_t)argc - 1);
    } catch(std::exception &ex) {
        std::cerr << "Invalid arguments: " << ex.what() << "\n";
        std::cerr << options.getUsage() << "\n";
        exit(EXIT_FAILURE);
    }

    Socket master;

    try {
        master.setPort(port->getValue());

        master.bindToAddress();
        master.startListening();

        IOEvents events(21);

        ChatServer server(master, events);
        server.run();
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        exit(EXIT_FAILURE);
    }

    return 0;
}