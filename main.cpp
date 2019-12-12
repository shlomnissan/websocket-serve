#include <iostream>

#include "library/listener.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: websocket-serve <address> <port>\n";
        return EXIT_FAILURE;
    }

    const auto kAddress = net::ip::make_address(argv[1]);
    const auto kPort = static_cast<unsigned short>(std::atoi(argv[2]));

    net::io_context ioc;

    std::make_shared<Listener>(ioc, tcp::endpoint{kAddress, kPort}, std::make_shared<SharedState>())->Run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](error_code const &, int) {
        ioc.stop();
    });

    std::cout << "Listening for Websockets requests at " << kAddress << ':' << kPort << '\n';
    ioc.run(); // Start the I/O service on the main thread

    return EXIT_SUCCESS;
}