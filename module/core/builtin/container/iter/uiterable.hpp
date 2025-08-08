// nested class of ucontainable.hpp
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

    virtual RSquare operator*() = 0;
    const RSquare operator*() const BY_CONST_FUNC(operator*())

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

    virtual R get() = 0;
    const R get() const BY_CONST_FUNC(get())

    virtual nbool isFrom(const tucontainable& it) const = 0;
    nbool isFrom(const tucontainable* it) const BY_SIDE_FUNC(isFrom);

    virtual tucontainable<T, R, RSquare>* getContainer() = 0;
    const tucontainable<T, R, RSquare>* getContainer() const BY_CONST_FUNC(getContainer())
};
