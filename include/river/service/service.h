#pragma once

namespace river {
class service
{
  public:
    service(const service&) = delete;
    service& operator=(const service&) = delete;
    service()                          = default;
    virtual ~service()                 = default;
};
} // namespace river
