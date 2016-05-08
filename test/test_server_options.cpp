#include "gtest/gtest.h"
#include "server_options.h"

TEST(ServerOptionsTest, ReturnsDefaultPort) {
    const int DEFAULT_PORT = 42;

    ServerOptions options({"server"}, DEFAULT_PORT);

    options.parse();

    EXPECT_EQ(options.getPort(), DEFAULT_PORT);
}

TEST(ServerOptionsTest, ReturnsProvidedPort) {
    const int DEFAULT_PORT = 42;
    int port = 43;

    ServerOptions options({"server", std::to_string(port)}, DEFAULT_PORT);

    options.parse();

    EXPECT_EQ(options.getPort(), port);
}

TEST(ServerOptionsTest, ThrowsOnWrongArgumentCount) {
    const int DEFAULT_PORT = 42;

    ServerOptions options({"server", "43", "43"}, DEFAULT_PORT);

    EXPECT_THROW(options.parse(), std::invalid_argument);
}


TEST(ServerOptionsTest, ThrowsOnNonNumericPort) {
    const int DEFAULT_PORT = 42;

    ServerOptions options1({"server", "43a"}, DEFAULT_PORT);
    EXPECT_THROW(options1.parse(), std::invalid_argument);

    ServerOptions options2({"server", "abc"}, DEFAULT_PORT);
    EXPECT_THROW(options2.parse(), std::invalid_argument);
}

TEST(ServerOptionsTest, ThrowsOnOutOfRange) {
    const int DEFAULT_PORT = 42;

    ServerOptions options1({"server", "43132423535123513"}, DEFAULT_PORT);
    EXPECT_THROW(options1.parse(), std::out_of_range);
}