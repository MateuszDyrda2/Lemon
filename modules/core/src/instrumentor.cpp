#include <lemon/core/instrumentor.h>

#include <algorithm>
#include <sstream>

namespace lemon {
using namespace std;
using namespace std::chrono;
void instrumentor::begin_session(const std::string& name, const std::string& path)
{
    lock_guard lock(writeLock);
    currentSession = create_owned<session>(name);
    outputStream.open(path, ios::binary);
    outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
    outputStream.flush();
}
void instrumentor::end_session()
{
    lock_guard lock(writeLock);
    outputStream << "]}";
    outputStream.flush();
    outputStream.close();
    currentSession.reset();
}
void instrumentor::write_data(const profile_data& data)
{
    stringstream ss;
    ss << ",{";
    ss << "\"cat\":\"function\",";
    ss << "\"dur\":" << (data.endTime - data.startTime) << ',';
    ss << "\"name\":\"" << data.name << "\",";
    ss << "\"ph\":\"X\",";
    ss << "\"pid\":0,";
    ss << "\"tid\":" << data.threadID << ",";
    ss << "\"ts\":" << data.startTime;
    ss << "}";

    {
        lock_guard lock(writeLock);
        outputStream << ss.rdbuf();
        outputStream.flush();
    }
}
instrumentor& instrumentor::get()
{
    static instrumentor s_instrumentor;
    return s_instrumentor;
}
instrumentation_timer::instrumentation_timer(const char* name):
    name(name), startTimepoint(high_resolution_clock::now())
{ }
instrumentation_timer::~instrumentation_timer()
{
    auto endTime = high_resolution_clock::now();

    auto start = time_point_cast<microseconds>(startTimepoint).time_since_epoch().count();
    auto end   = time_point_cast<microseconds>(endTime).time_since_epoch().count();

    auto tId = hash<thread::id>{}(this_thread::get_id());
    instrumentor::get().write_data(profile_data{ name, start, end, tId });
}
} // namespace lemon
