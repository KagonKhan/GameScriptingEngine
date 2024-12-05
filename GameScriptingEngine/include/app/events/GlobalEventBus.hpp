#pragma once
#include <EventBus/EventBus.hpp>


class GlobalEventBus {
private:
    static constexpr char const* const TAG{"[GlobalEventBus]"};

public:
    static void Process() { bus->process(); }
    static bool Add(auto const& event) { return bus->postpone(event); }

private:
    inline static std::shared_ptr<dexode::EventBus> bus{std::make_shared<dexode::EventBus>()};
    friend class EventListener;
};
