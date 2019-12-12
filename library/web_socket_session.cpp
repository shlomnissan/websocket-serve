//
// Created by Shlomi Nissan on 12/5/19.
//

#include "web_socket_session.hpp"

#include <iostream>

//region Public

WebSocketSession::WebSocketSession(tcp::socket socket, const std::shared_ptr<SharedState> &state):
        ws_(std::move(socket)),
        state_(state) {}

WebSocketSession::~WebSocketSession() {
    state_->Leave(*this);
}

void WebSocketSession::Send(const std::shared_ptr<const std::string> &str) {
    queue_.push_back(str);

    // If the queue is not empty, we're already writing
    if (queue_.size() > 1) {
        return;
    }

    ws_.async_write(net::buffer(*queue_.front()), [sp = shared_from_this()](error_code error, std::size_t bytes){
        sp->OnWrite(error, bytes);
    });
}

//endregion

//region Private

void WebSocketSession::Fail(error_code error, const char *what) {
    if (error == net::error::operation_aborted || error == websocket::error::closed) return;
    std::cerr << what << ": " << error.message() << '\n';
}

void WebSocketSession::OnAccept(error_code error) {
    if (error) return Fail(error, "accept");

    // Add the session to the list of active sessions
    state_->Join(*this);

    // Read a message
    ws_.async_read(buffer_, [sp = shared_from_this()](error_code error, std::size_t bytes){
        sp->OnRead(error, bytes);
    });

}

void WebSocketSession::OnRead(error_code error, std::size_t bytes) {
    if (error) return Fail(error, "read");

    const auto message = beast::buffers_to_string(buffer_.data());
    // TODO: Add logging
    state_->Send(message);

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Read another message
    ws_.async_read(buffer_, [sp = shared_from_this()](error_code error, std::size_t bytes){
        sp->OnRead(error, bytes);
    });
}

void WebSocketSession::OnWrite(error_code error, std::size_t bytes) {
    if (error) return Fail(error, "write");

    // Remove message from queue
    queue_.erase(queue_.begin());

    // Send the next message if it exists
    if (!queue_.empty()) {
        ws_.async_write(net::buffer(*queue_.front()), [sp = shared_from_this()](error_code error, std::size_t bytes){
            sp->OnWrite(error, bytes);
        });
    }
}

//endregion
