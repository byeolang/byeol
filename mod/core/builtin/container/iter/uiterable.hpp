// nested class of ucontainable.hpp
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
    virtual ncnt stepForward(ncnt step) = 0;
    virtual ncnt stepBackward(ncnt step) = 0;
    virtual R get() = 0;
    const R get() const NM_CONST_FUNC(get())
    virtual nbool isFrom(const tucontainable& it) const = 0;
    virtual tucontainable<T, R>& getContainer() = 0;
    const tucontainable<T, R>& getContainer() const NM_CONST_FUNC(getContainer())
};
