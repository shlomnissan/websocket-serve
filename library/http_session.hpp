//
// Created by Shlomi Nissan on 12/8/19.
//

#ifndef CPP_WEBSOCKET_SERVER_HTTP_SESSION_HPP
#define CPP_WEBSOCKET_SERVER_HTTP_SESSION_HPP

#include <memory>
#include <cstdlib>

#include "../net.hpp"
#include "../beast.hpp"

#include "web_socket_session.hpp"
#include "shared_state.hpp"

class HttpSession: public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket, const std::shared_ptr<SharedState>& state);

    void Run();

private:
    tcp::socket socket_;
    beast::flat_buffer  buffer_;
    std::shared_ptr<SharedState> state_;
    http::request<http::string_body> request_;

    void Fail(error_code error, const char* what);
    void OnRead(error_code error, std::size_t bytes);
};


#endif //CPP_WEBSOCKET_SERVER_HTTP_SESSION_HPP
