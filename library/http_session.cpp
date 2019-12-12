//
// Created by Shlomi Nissan on 12/8/19.
//

#include "http_session.hpp"

#include <iostream>

//region Public

HttpSession::HttpSession(tcp::socket socket, const std::shared_ptr<SharedState> &state):
    socket_(std::move(socket)), state_(state){ }

void HttpSession::Run() {
    http::async_read(socket_, buffer_, request_, [self = shared_from_this()](error_code error, std::size_t bytes){
        self->OnRead(error, bytes);
    });
}

//endregion

//region Private

void HttpSession::Fail(error_code error, const char *what) {
    // Don't report on canceled operation
    if (error == net::error::operation_aborted) return;
    std::cerr << what << ": " << error.message() << '\n';
}

void HttpSession::OnRead(error_code error, std::size_t bytes) {
    if (error == http::error::end_of_stream) {
        socket_.shutdown(tcp::socket::shutdown_send, error);
        return;
    }

    if (error) return Fail(error, "read");

    if(websocket::is_upgrade(request_)) {
        // Create a Websocket session by transferring the socket
        std::make_shared<WebSocketSession>(std::move(socket_), state_)->Run(std::move(request_));
    } else {
        // TODO: Return error, only accepting Websocket requests
    }
}

//endregion
