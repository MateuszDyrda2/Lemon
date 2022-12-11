#pragma once

#include "core/math/vec2.h"
#include <core/defines.h>
#include <core/hash_string.h>
#include <core/pool_allocator.h>

#include <optional>
#include <unordered_map>
#include <vector>

namespace lemon {
struct LEMON_API message_payload
{
    enum type
    {
        Integer,
        Float,
        Bool,
        Vector,
        HashString,
    } payloadType;

    union
    {
        i32 asInteger;
        f32 asFloat;
        bool asBool;
        vec2 asVector;
        hashstr asHashString;
    };

    message_payload* next{ nullptr };
};

struct LEMON_API message
{
    std::string messageName;
    message_payload* payload;
};

class LEMON_API message_bus
{
  public:
    struct message_builder
    {
        template<class T>
        message_builder& add_arg(T arg);
        void push();

        message_bus& bus;
        message_payload* head;
        message_payload* last;
        u32 entityid;
        const char* messageName;
    };

  public:
    message_bus();
    ~message_bus();

    template<class... Args>
    void push_message(u32 entityid, const std::string& messageName, Args&&... args);
    message_builder create_message(u32 entityid, const char* messageName);
    std::optional<std::vector<message>> get_messages(u32 entityid);
    void clear();

  private:
    friend message_builder;
    std::unordered_map<u32, std::vector<message>> messages;
    pool_allocator<message_payload> payloads;

  private:
    message_payload* get_payload(i32 value);
    message_payload* get_payload(f32 value);
    message_payload* get_payload(bool value);
    message_payload* get_payload(vec2 value);
    message_payload* get_payload(hashstr value);
};

template<class T>
message_bus::message_builder& message_bus::message_builder::add_arg(T arg)
{
    auto p = bus.get_payload(arg);
    if (head == nullptr)
    {
        head = last = p;
    }
    else
    {
        last->next = p;
        last       = p;
    }
    return *this;
}

template<class... Args>
void message_bus::push_message(u32 entityid, const std::string& messageName, Args&&... args)
{
    if constexpr (sizeof...(Args) == 0)
    {
        messages[entityid].push_back(message{ .messageName = messageName, .payload = nullptr });
    }
    else
    {
        message_payload* payloads[] = { get_payload(args)... };
        for (std::size_t i = 0; i < (sizeof...(Args)) - 1; ++i)
        {
            payloads[i]->next = payloads[i + 1];
        }
        messages[entityid].push_back(message{ .messageName = messageName, .payload = payloads[0] });
    }
}
}
