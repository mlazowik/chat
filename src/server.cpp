#include <iostream>
#include <stdexcept>
#include <system_error>
#include <unistd.h>

#include "server_options.h"
#include "socket.h"
#include "io_events.h"

#define BUF_SIZE 1024

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

    Socket s;

    s.setPort(port);
    s.startListening();

    IOEvents events(20);
    char buf[BUF_SIZE];

    while (true) {
        Socket connection = s.acceptConnection();

        bool active = true;

        events.registerSocket(connection, [&](short revents) {
            ssize_t rval = read(connection.getDescriptor(), buf, BUF_SIZE);

            if (rval < 0) {
                throw std::system_error(errno, std::system_category());
            }

            if (rval == 0) {
                events.deregisterSocket(connection);
                active = false;
            }

            printf("-->%.*s\n", (int) rval, buf);
        });

        while (active) {
            events.processEvents();
        }
    }

    return 0;
}