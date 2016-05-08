#include <iostream>
#include <stdexcept>
#include "server_options.h"

int main(int argc, char* argv[]) {
    ServerOptions options = ServerOptions(argc, argv);

    try {
        options.parse();
    } catch(std::invalid_argument &ex) {
        std::cerr << "Invalid argument: " << ex.what() << "\n";
        std::cerr << options.getUsage() << "\n";
        exit(EXIT_FAILURE);
    }

    int port = options.getPort();

    std::cout << port << "\n";

    return 0;
}