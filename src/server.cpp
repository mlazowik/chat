#include <iostream>
#include <stdexcept>

using namespace std;

const int DEFAULT_PORT = 20160;

void print_usage(std::ostream &out) {
    out << "Usage: server [PORT]\n";
}

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;

    try {
        if (argc > 2) {
            throw std::invalid_argument("too many arguments");
        }

        if (argc == 2) {
            port = std::stoi(argv[1]);
        }
    } catch(std::invalid_argument &ex) {
        std::cerr << "Invalid argument: " << ex.what() << "\n";
        print_usage(std::cerr);
        exit(EXIT_FAILURE);
    }

    cout << port << "\n";

    return 0;
}