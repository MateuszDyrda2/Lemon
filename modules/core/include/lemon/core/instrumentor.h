#pragma once

#include "basic_types.h"

#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

#if LEMON_PROFILE
#    define LEMON_PROFILE_BEGIN(name, path) ::lemon::instrumentor::get().begin_session(name, path)
#    define LEMON_PROFILE_END()             ::lemon::instrumentor::get().end_session()
#    define LEMON_PROFILE_SCOPE(name)       ::lemon::instrumentation_timer _timer##__LINE__(name)
#    define LEMON_PROFILE_FUNCTION()        LEMON_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
#    define LEMON_PROFILE_BEGIN(name, path)
#    define LEMON_PROFILE_END()
#    define LEMON_PROFILE_SCOPE(name)
#    define LEMON_PROFILE_FUNCTION()
#endif

// Implementation based on TheCherno's video
// https://www.youtube.com/watch?v=xlAH4dbMVnU&t=944s
namespace lemon {
struct profile_data
{
    std::string name;
    i64 startTime, endTime;
    std::size_t threadID;
};
struct session
{
    std::string name;
};
class instrumentor
{
  public:
    void begin_session(const std::string& name, const std::string& path);
    void end_session();
    void write_data(const profile_data& data);
    static instrumentor& get();

  private:
    owned<session> currentSession;
    std::ofstream outputStream;
    std::mutex writeLock;
};
struct instrumentation_timer
{
    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
    instrumentation_timer(const char* name);
    ~instrumentation_timer();

    const char* name;
    time_point startTimepoint;
};
} // namespace lemon
