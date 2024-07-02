// nested class of bicontainable.hpp
class iterable {
    NM_ME(iterable)

public:
    virtual ~iterable() {}

    virtual nbool isEnd() const = 0;
    /// proceed iteration obj to point next element by given step.
    /// @param  step    how much it proceeds.
    ///                 positive value only.
    /// @return how many step it proceeded. if it tries to proceed further when it reached to End()
    ///         it won't be counted as returning value.
    ///         it'll return '0' if it already reached to the end.
    virtual ncnt next(ncnt step) = 0;
    virtual const K& getKey() const = 0;
    virtual V& getVal() = 0;
    const V& getVal() const NM_CONST_FUNC(getVal())
    virtual void setVal(const V& newVal) = 0;
    void setVal(const V* newVal) { return setVal(*newVal); }
    virtual nbool isFrom(const tbicontainable& it) const = 0;
    virtual tbicontainable<K, V>& getContainer() = 0;
    const tbicontainable<K, V>& getContainer() const NM_CONST_FUNC(getContainer())
};
