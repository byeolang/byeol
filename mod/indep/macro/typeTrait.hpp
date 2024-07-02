#pragma once

namespace nm
{
    template <typename T>
    class TypeTrait {
    public:
        typedef T Org;
        typedef T& Ref;
        typedef T* Ptr;
        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
    };

    template <typename T>
    class TypeTrait<T&> {
    public:
        typedef T Org;
        typedef T& Ref;
        typedef T* Ptr;

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = true;
    };

    template <typename T>
    class TypeTrait<T*> {
    public:
        typedef T Org;
        typedef T& Ref;
        typedef T* Ptr;

        static constexpr nbool is_ptr = true;
        static constexpr nbool is_ref = false;
    };
}
