//
// Created by Shlomi Nissan on 12/4/19.
//

#include <iostream>

#include "listener.hpp"
#include "http_session.hpp"

//region Public

Listener::Listener(net::io_context &ioc, tcp::endpoint endpoint, const std::shared_ptr<SharedState> &state):
        acceptor_(ioc),
        socket_(ioc),
        state_(state) {
    error_code error;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), error);
    if (error) {
        Fail(error, "set_option");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true));
    if (error) {
        Fail(error, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, error);
    if (error) {
        Fail(error, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, error);
    if (error) {
        Fail(error, "listen");
        return;
    }
}

void Listener::Run() {
    acceptor_.async_accept(socket_, [self = shared_from_this()](error_code error){
        self->OnAccept(error);
    });
}

//endregion

//region Private

void Listener::Fail(error_code error, const char *what) {
    if (error == net::error::operation_aborted) {
        return;
    }
    std::cerr << what << ": " << error.message() << '\n';
}

void Listener::OnAccept(error_code error) {
    if (error) {
        return Fail(error, "accept");
    } else {
        std::make_shared<HttpSession>(std::move(socket_), state_)->Run();

        acceptor_.async_accept(socket_, [self = shared_from_this()](error_code error){
            self->OnAccept(error);
        });
    }
}

//endregion
