#pragma once

#include "../common.hpp"
#include "../macro/namuMeta.hpp"
#include "../macro/declThis.hpp"
#include "../macro/unconstFunc.hpp"
#include <optional>

namespace nm {

    /// this is a wrapper class for std::optional.
    /// improved `optional`'s func name and signature because they are somewhat inconsistent
    /// from the perspective of namu and convention rules.
    template <typename T> class tmay {
        NM(ME(tmay))

    public:
        tmay() = default;

        explicit tmay(const T& value): _subj(value) {}

    public:
        T* operator->() { return &get(); }

        const T* operator->() const NM_CONST_FUNC(operator->());

        T& operator*() { return get(); }

        const T& operator*() const NM_CONST_FUNC(operator*());

        operator nbool() const { return has(); }

    public:
        nbool has() const { return _subj.has_value(); }

        T& get() {
            WHEN(!has()).template retNul<T>();
            return _subj.value();
        }

        void rel() { _subj.reset(); }

        template <typename... Ts> void set(Ts... args) { _subj.emplace(args...); }

    private:
        std::optional<T> _subj;
    };
} // namespace nm
