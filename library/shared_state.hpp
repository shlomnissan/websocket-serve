//
// Created by Shlomi Nissan on 12/5/19.
//

#ifndef CPP_WEBSOCKET_SERVER_SHARED_STATE_HPP
#define CPP_WEBSOCKET_SERVER_SHARED_STATE_HPP

#include <memory>
#include <string>
#include <unordered_set>

#include "web_socket_session.hpp"

// Forward declaration
class WebSocketSession;

class SharedState {
public:
    void Join(WebSocketSession& session);
    void Leave(WebSocketSession& session);
    void Send(const std::string message);

private:
    std::unordered_set<WebSocketSession *> sessions_;
};


#endif //CPP_WEBSOCKET_SERVER_SHARED_STATE_HPP
