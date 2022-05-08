#pragma once

#include "node.hpp"

namespace wrd {

#define ME node

	template <typename T>
	T& ME::sub() const {
		return subs().get<T>([](const std::string& key, const T& val) {
			return true;
		});
	}

    template <typename T>
    T& ME::sub(const std::string& name) const {
        return subs().get<T>([&](const std::string& key, const T& val) {
            return key == name;
        });
    }

    template <typename T>
    T& ME::sub(const std::string& name, const ucontainable& args) {
        if(nul(args))
            return sub<T>(name);

        return subs().get<T>([&](const std::string& key, const T& val) {
            return key == name && val.canRun(args);
        });
    }

    template <typename T>
    T& ME::sub(const std::string& name, const ucontainable& args) const WRD_UNCONST_FUNC(sub<T>(name, args))

	template <typename T>
	tnarr<T, strTactic> ME::subAll() const {
		return subs().getAll<T>([](const std::string& key, const T& val) {
			return true;
		});
	}

    template <typename T>
    tnarr<T, strTactic> ME::subAll(const std::string& name) const {
        return subs().getAll<T>([&](const std::string& key, const T& val) {
            return key == name;
        });
    }

    template <typename T>
    tnarr<T, strTactic> ME::subAll(const std::string& name, const ucontainable& args) {
        if(nul(args))
            return subAll<T>(name);

        return subs().getAll<T>([&](const std::string& key, const T& val) {
            return key == name && val.canRun(args);
        });
    }

    template <typename T>
    tnarr<T, strTactic> ME::subAll(const std::string& name, const ucontainable& args) const WRD_UNCONST_FUNC(subAll<T>(name, args))

#undef ME
}
