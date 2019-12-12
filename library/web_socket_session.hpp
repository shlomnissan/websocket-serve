//
// Created by Shlomi Nissan on 12/5/19.
//

#ifndef CPP_WEBSOCKET_SERVER_WEB_SOCKET_SESSION_HPP
#define CPP_WEBSOCKET_SERVER_WEB_SOCKET_SESSION_HPP

#include "../net.hpp"
#include "../beast.hpp"
#include "shared_state.hpp"

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

// Forward declaration
class SharedState;

class WebSocketSession: public std::enable_shared_from_this<WebSocketSession> {
public:
    WebSocketSession(tcp::socket socket, const std::shared_ptr<SharedState>& _state);
    ~WebSocketSession();

    void Send(const std::shared_ptr<const std::string>& str);

    template<class Body, class Allocator>
            void Run(http::request<Body, http::basic_fields<Allocator>> request);

private:
    beast::flat_buffer buffer_;
    websocket::stream<tcp::socket> ws_;
    std::shared_ptr<SharedState> state_;
    std::vector<std::shared_ptr<const std::string>> queue_;

    void Fail(error_code error, const char* what);
    void OnAccept(error_code error);
    void OnRead(error_code error, std::size_t bytes);
    void OnWrite(error_code error, std::size_t bytes);
};

template<class Body, class Allocator>
void WebSocketSession::Run(http::request<Body, http::basic_fields<Allocator>> request) {
    ws_.async_accept(request, std::bind(&WebSocketSession::OnAccept, shared_from_this(), std::placeholders::_1));
}

#endif //CPP_WEBSOCKET_SERVER_WEB_SOCKET_SESSION_HPP
