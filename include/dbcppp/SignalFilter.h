
#pragma once

#include <memory>
#include <functional>

#include "Message.h"

namespace dbcppp
{
    struct SignalFilterImpl;
    class SignalFilter
    {
    public:
        using callback_t = std::function<void(uint64_t signal_id, double phys, void* user_data)>;

        SignalFilter(callback_t&& cb);
        void addSignal(const Message* msg, const Signal* sig, uint64_t signal_id, void* user_data);
        void applyData(uint64_t frame_id, const void* bytes) const;

    private:
        std::unique_ptr<SignalFilterImpl> _pimpl;
    };
}
