#pragma once
#include "EventBus/EventBus.hpp"
#include "EventBus/Listener.hpp"
#include "GlobalEventBus.hpp"


class EventListener {
private:
    static constexpr char const* const TAG{"[EventListener]"};

public:
    constexpr void listen(auto&& callback) { eventListener.listen(std::forward<decltype(callback)>(callback)); }
    void           unlistenAll() { eventListener.unlistenAll(); }

private:
    dexode::EventBus::Listener eventListener{GlobalEventBus::bus};
};