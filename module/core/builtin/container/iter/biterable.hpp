/// @file
// nested class of bicontainable.hpp
class iterable {
    BY_ME(iterable)

protected:
    enum iterationType {
        NEXT,
        PREV,
        FORWARD,
        BACKWARD
    };

public:
    virtual ~iterable() {}

    virtual nbool isEnd() const = 0;

    virtual nbool isReversed() const = 0;

    virtual void rel() = 0;

    /// proceed iteration obj to point next element by given step.
    /// @param  step    how much it proceeds.
    ///                 positive value only.
    /// @return how many step it proceeded. if it tries to proceed further when it reached to End()
    ///         it won't be counted as returning value.
    ///         it'll return '0' if it already reached to the end.
    virtual ncnt next(ncnt step) { return isReversed() ? stepBackward(step) : stepForward(step); }

    virtual ncnt prev(ncnt step) { return isReversed() ? stepForward(step) : stepBackward(step); }

    virtual ncnt stepForward(ncnt step) = 0;
    virtual ncnt stepBackward(ncnt step) = 0;

    virtual const K* getKey() const = 0;

    virtual V* getVal() = 0;
    const V* getVal() const BY_CONST_FUNC(getVal())

    virtual void setVal(const V& newVal) = 0;
    void setVal(const V* it) BY_SIDE_FUNC(setVal);

    virtual nbool isFrom(const tbicontainable& it) const = 0;
    nbool isFrom(const tbicontainable* it) const BY_SIDE_FUNC(isFrom);

    virtual tbicontainable<K, V>* getContainer() = 0;
    const tbicontainable<K, V>* getContainer() const BY_CONST_FUNC(getContainer())
};
