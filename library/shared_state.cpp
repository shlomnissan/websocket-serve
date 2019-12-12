//
// Created by Shlomi Nissan on 12/5/19.
//

#include "shared_state.hpp"

//region Public

void SharedState::Join(WebSocketSession &session) {
    sessions_.insert(&session);
}

void SharedState::Leave(WebSocketSession &session) {
    sessions_.erase(&session);
}

void SharedState::Send(const std::string message) {
    const auto kMessage = std::make_shared<const std::string>(std::move(message));
    for (auto session: sessions_) {
        session->Send(kMessage);
    }
}

//endregion
