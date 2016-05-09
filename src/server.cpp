#include <iostream>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <set>

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

    Socket master;
    std::set<Socket> clients;

    master.setPort(port);
    master.startListening();

    IOEvents events(20);

    events.registerSocket(master, [&](Socket socket, short revents) {
        if (!(revents & POLLIN)) return;

        Socket client = master.acceptConnection();
        clients.insert(client);

        events.registerSocket(client, [&](Socket socket, short revents) {
            if (!(revents & (POLLIN | POLLERR))) return;

            char buf[BUF_SIZE];
            ssize_t rval = read(socket.getDescriptor(), buf, BUF_SIZE);

            if (rval < 0) {
                events.deregisterSocket(socket);
                socket.destroy();
                clients.erase(socket);
                throw std::system_error(errno, std::system_category());
            }

            if (rval == 0) {
                events.deregisterSocket(socket);
                socket.destroy();
                clients.erase(socket);
            } else {
                std::cout << "me: " << socket.getDescriptor() << "\n";
                std::cout << "others:";
                for (const Socket &client : clients) {
                    if (client != socket) {
                        std::cout << " " << client.getDescriptor();
                    }
                }
                std::cout << "\n";
            }
        });
    });

    while (true) {
         events.processEvents();
    }

    return 0;
}