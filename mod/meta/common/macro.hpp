#pragma once

#include "dep.hpp"

// namuMeta macro's sub-commands:
//  CLONE:
//      defines normal clone func. it have to be defined if you
//      just have declared non abstract class.
//
//      MSVCHack:
//          MSVC couldn't handle covariant with __declspec(dllimport) when it declared by class
//          template. but clang, gcc can handle properly.
//          to avoid that MSVC complains, I have no choice but to declare ret type of clone
//          func as clonable. what a good compiler!
//
//          I leave an example below. please run the code MSVC 2022.
//
/*              class _declspec(dllimport) type {};

                class __declspec(dllimport) typeProvidable {
                public:
                    virtual const type& getType() const = 0;
                };

                class __declspec(dllimport) clonable {
                public: typedef clonable me;
                    virtual me* clone() const = 0;
                    virtual me* cloneDeep() const { return clone(); }
                };

                class __declspec(dllimport) instance : public typeProvidable, public clonable {
                    instance* cloneDeep() const override { return (instance*)clone(); }
                };

                class __declspec(dllimport) strTactic {};

                template <typename T, typename TACTIC = strTactic>
                class tnarr : public instance {
                public:
                    const type& getType() const override { const type* ret = nullptr; return *ret; }
                    tnarr* clone() const override;
                    tnarr* cloneDeep() const override;
                };
                class node;
                extern template class __declspec(dllimport) tnarr<node, strTactic>;

                class __declspec(dllexport) A : public tnarr<node> {
                public:
                    A* cloneDeep() const override;
                };
*/
//          this code occurs the error saying C2491: "you're not allowed to define func 'tnarr<node,
//          strTactic>::clone()' with dllimport."
//
//          but obviously it's very hard to notice the reason of error has been related to covariant
//          with the description above.
//          only way to resolve error condition so far is replace all return type of clone() to
//          'clonable', not to use covarint.

#define __NM__DECL_CLONE(ME)                                   \
public:                                                        \
    clonable* clone() const override { return new ME(*this); } \
                                                               \
private:

//  TYPE:
//      defines super meta type.
//      for instance, if you set TYPE(type), then when you call getType() of your class,
//      it will returns instance of type of type.
#define __NM__DECL_TYPE(METATYPE)                                      \
public:                                                                \
    typedef METATYPE metaType;                                         \
    const ntype& getType() const override { return ttype<me>::get(); } \
                                                                       \
private:
