//
// Created by Shlomi Nissan on 12/4/19.
//

#ifndef CPP_WEBSOCKET_SERVER_LISTENER_HPP
#define CPP_WEBSOCKET_SERVER_LISTENER_HPP

#include <memory>
#include <string>

#include "../net.hpp"
#include "shared_state.hpp"

class Listener: public std::enable_shared_from_this<Listener>  {
public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint, const std::shared_ptr<SharedState>& state);

    void Run();

private:
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::shared_ptr<SharedState> state_;

    void Fail(error_code error, const char* what);
    void OnAccept(error_code error);
};


#endif //CPP_WEBSOCKET_SERVER_LISTENER_HPP
