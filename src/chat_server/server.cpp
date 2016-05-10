#include <iostream>
#include <stdexcept>

#include "server_options.h"
#include "chat_server.h"
#include "../common/socket.h"
#include "../common/io_events.h"

int main(int argc, char* argv[]) {
    const int DEFAULT_PORT = 20160;

    std::vector<std::string> arguments;

    for (int argument = 0; argument < argc; argument++) {
        arguments.push_back(argv[argument]);
    }

    ServerOptions options(arguments, DEFAULT_PORT);

    try {
        options.parse();
    } catch(std::exception &ex) {
        std::cerr << "Invalid arguments: " << ex.what() << "\n";
        std::cerr << options.getUsage() << "\n";
        exit(EXIT_FAILURE);
    }

    int port = options.getPort();

    Socket master;

    try {
        master.setPort(port);

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