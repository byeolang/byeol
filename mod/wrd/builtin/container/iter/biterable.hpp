// nested class of bicontainable.hpp
class iterable {
    WRD_DECL_ME(iterable)

public:
    virtual ~iterable() {}

    virtual wbool isEnd() const = 0;
    /// proceed iteration obj to point next element by given step.
    /// @param  step    how much it proceeds.
    ///                 positive value only.
    /// @return how many step it proceeded. if it tries to proceed further when it reached to End()
    ///         it won't be counted as returning value.
    ///         it'll return '0' if it already reached to the end.
    virtual wcnt next(wcnt step) = 0;
    virtual const K& getKey() const = 0;
    virtual V& getVal() = 0;
    const V& getVal() const WRD_UNCONST_FUNC(getVal())
    virtual wbool isFrom(const tbicontainable& it) const = 0;
    virtual tnbicontainer<K, V>& getContainer() = 0;
    const tnbicontainer<K, V>& getContainer() const WRD_UNCONST_FUNC(getContainer())
};
