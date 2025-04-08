#pragma once

#include <utility>
#include "../def/_nout.hpp"
#include "common/typedef.hpp"
#include "../common/typedef.hpp"
#include "declThis.hpp"
#include "namuMeta.hpp"

namespace nm {

    // `WHEN` macro:
    //  in `namu`, we actively apply the early-return pattern.
    //  this helps to reduce the depth, make the code clean, and handle exceptional situations
    //  immediately.
    //  in the case of general C++ code that applies the early pattern, you will use `if` as
    //  follows.
    //   ```cpp
    //       SomeAdapter* adapter = bridge.requestAdapter("token");
    //       if (!adapter) { // 1) ealry return
    //           printLogs("null adapter!");
    //           return;
    //       }
    //
    //       if (adapter->getState() == State.ACTIVATE) { // 2)
    //           adapter->onActivate();
    //       } else {
    //           const vector<Device>& devices = adapter->getDevices();
    //           if (devices.size() <= 0) { // 3) ealry return
    //               printLogs("there is no device in " + adapter->getName());
    //               return;
    //           }
    //
    //           for (const auto& device : devices) {
    //               DeviceResult res = device.attach();
    //               if (res == FAILURE) {
    //                   printLogs("device can't attach to system"); // 4) ealry early-return
    //                   return;
    //               }
    //               if (res == Pending) {
    //                   device.retryQueue();
    //               }
    //           }
    //       }
    //
    //
    //   ```
    //
    //   the problem is that, as can be seen in the code above, if is used so commonly that it is
    //   difficult to immediately know whether a normal branch is made for logic or an ealry return
    //   is made to prune in advance with exception handling. that's why in namu, `WHEN` is used for
    //   almost all ealry return exception handling.
    //
    //   the output is very intuitive.
    //
    //   ```cpp
    //       SomeAdapter* adapter = bridge.requestAdapter("token");
    //       WHEN_NUL(adapter).err("null adapter!");
    //
    //       if (adapter->getState() == State.ACTIVATE) {
    //           adapter->onActivate();
    //       } else {
    //           const vector<Device>& devices = adapter->getDevices();
    //           WHEN(devices.size() <= 0).err("there is no device in %s", adapter->getName());
    //
    //           for (const auto& device : devices) {
    //               DeviceResult res = device.attach();
    //               WHEN(res == FAILURE).err("device can't attach to system");
    //
    //               if (res == Pending) {
    //                   device.retryQueue();
    //               }
    //           }
    //       }
    //   ```
    //
    //  as in the example, it is clear which part handles exceptions and sends them out, and which
    //  part branches for logic. also, since `WHEN` is class-based, handling when the exception
    //  handling condition is satisfied can be handled based on the class.

    // __WHEN_OBJECT__?:
    //  since namu is structured as a multi-layered architecture, if the layer is low-level, it
    //  simply outputs the log to the screen, but in high-level layers, it requires more complex
    //  processing, such as creating an exception as an object and including stacktrace information
    //  to record it.
    //
    //  in this way, since different classes should be displayed when the `WHEN` macro is expanded
    //  depending on the layer, this is solved by redefining `__WHEN_OBJECT__`.

#define __WHEN_OBJECT__ __indep_when__

    class _nout __WHEN_OBJECT__ {
        NM(ME(__WHEN_OBJECT__))

    public:
        template <typename R, typename... Ts> R& ret([[maybe_unused]] R& r) const { return r; }

        template <typename R, typename... Ts> R&& ret([[maybe_unused]] R&& r) const {
            return std::move(r);
        }

        void ret() const {}

        template <typename T> T& retNul() const { return nulOf<T>(); }
    };

#define __WHEN_POSTFIX__ return ::nm::__WHEN_OBJECT__::get()
#define WHEN(condition) \
    if(condition) __WHEN_POSTFIX__
#define WHEN_NUL_1(v1) \
    if(nul(v1)) __WHEN_POSTFIX__
#define WHEN_NUL_2(v1, v2) \
    if(nul(v1) || nul(v2)) __WHEN_POSTFIX__
#define WHEN_NUL_3(v1, v2, v3) \
    if(nul(v1) || nul(v2) || nul(v3)) __WHEN_POSTFIX__
#define WHEN_NUL_4(v1, v2, v3, v4) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4)) __WHEN_POSTFIX__
#define WHEN_NUL_5(v1, v2, v3, v4, v5) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5)) __WHEN_POSTFIX__
#define WHEN_NUL_6(v1, v2, v3, v4, v5, v6) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5) || nul(v6)) __WHEN_POSTFIX__
#define WHEN_NUL(...) NM_OVERLOAD(WHEN_NUL, __VA_ARGS__)

} // namespace nm
