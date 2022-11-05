#include <scripting/message_bus.h>

namespace lemon {
using namespace std;
message_bus::message_bus()
{ }

message_bus::~message_bus()
{ }

message_bus::message_builder message_bus::create_message(u32 entityid, const char* messageName)
{
    return { .bus = *this, .head = nullptr, .last = nullptr, .entityid = entityid, .messageName = messageName };
}
void message_bus::message_builder::push()
{
    bus.messages[entityid].push_back(message{ .messageName = messageName, .payload = head });
}

optional<vector<message>> message_bus::get_messages(u32 entityid)
{
    auto m = messages.find(entityid);
    if (m == messages.end()) return {};

    return m->second;
}

void message_bus::clear()
{
    payloads.clear();
    messages.clear();
}

message_payload* message_bus::get_payload(i32 value)
{
    auto p         = payloads.get();
    p->asInteger   = value;
    p->payloadType = message_payload::type::Integer;
    return p;
}

message_payload* message_bus::get_payload(f32 value)
{

    auto p         = payloads.get();
    p->asFloat     = value;
    p->payloadType = message_payload::type::Float;
    return p;
}

message_payload* message_bus::get_payload(bool value)
{
    auto p         = payloads.get();
    p->asBool      = value;
    p->payloadType = message_payload::type::Bool;
    return p;
}

message_payload* message_bus::get_payload(vec2 value)
{
    auto p         = payloads.get();
    p->asVector    = value;
    p->payloadType = message_payload::type::Vector;
    return p;
}

message_payload* message_bus::get_payload(hash_str value)
{
    auto p          = payloads.get();
    p->asHashString = value;
    p->payloadType  = message_payload::type::HashString;
    return p;
}
}
