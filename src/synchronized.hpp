
#include <mutex>
#include <utility>


//! Wrapper making all the arithmetic operators for integers thread-safe.
template <typename Integer>
class synchronized {
    typedef Integer value_type;
    typedef std::mutex mutex_type;
    typedef std::lock_guard<mutex_type> scoped_lock;

    mutex_type mutable mutex_;
    value_type value_;

    friend value_type get(synchronized const& self) {
        scoped_lock lock(self.mutex_);
        return self.value_;
    }

public:
    template <typename ...Args>
    explicit synchronized(Args&& ...args)
        : value_(std::forward<Args>(args)...)
    { }

    synchronized(synchronized const& other)
        : value_(get(other))
    { }

    synchronized& operator=(synchronized const& other) {
        scoped_lock lock(mutex_);
        value_ = get(other);
        return *this;
    }

    operator value_type() const {
        return get(*this);
    }

#define COMPOUND_ASSIGNMENT_OPERATOR(OP)                                    \
    friend synchronized&                                                    \
    operator OP(synchronized& self, synchronized const& other) {            \
        scoped_lock lock(self.mutex_);                                      \
        self.value_ OP get(other);                                          \
        return self;                                                        \
    }                                                                       \
/**/

#define BINARY_OPERATOR(OP)                                                 \
    friend auto operator OP(synchronized const& x, synchronized const& y)   \
   -> decltype(get(x) OP get(y))                                            \
   {    return get(x) OP get(y); }                                          \
/**/

#define UNARY_OPERATOR(OP)                                                  \
    friend auto operator OP(synchronized const& x)                          \
   -> decltype(OP get(x))                                                   \
   {    return OP get(x); }                                                 \
/**/

   // Compound assignment operators
    COMPOUND_ASSIGNMENT_OPERATOR(+=)
    COMPOUND_ASSIGNMENT_OPERATOR(-=)
    COMPOUND_ASSIGNMENT_OPERATOR(*=)
    COMPOUND_ASSIGNMENT_OPERATOR(/=)
    COMPOUND_ASSIGNMENT_OPERATOR(%=)
    COMPOUND_ASSIGNMENT_OPERATOR(&=)
    COMPOUND_ASSIGNMENT_OPERATOR(|=)
    COMPOUND_ASSIGNMENT_OPERATOR(^=)
    COMPOUND_ASSIGNMENT_OPERATOR(<<=)
    COMPOUND_ASSIGNMENT_OPERATOR(>>=)

    // Arithmetic operators
    BINARY_OPERATOR(+)
    BINARY_OPERATOR(-)
    BINARY_OPERATOR(*)
    BINARY_OPERATOR(/)
    BINARY_OPERATOR(%)

    // Relational operators
    BINARY_OPERATOR(==)
    BINARY_OPERATOR(!=)
    BINARY_OPERATOR(>)
    BINARY_OPERATOR(<)
    BINARY_OPERATOR(>=)
    BINARY_OPERATOR(<=)

    // Bitwise operators
    UNARY_OPERATOR(~)
    BINARY_OPERATOR(&)
    BINARY_OPERATOR(|)
    BINARY_OPERATOR(^)
    BINARY_OPERATOR(<<)
    BINARY_OPERATOR(>>)

    // Logical operators (not providing && and ||)
    UNARY_OPERATOR(!)

#undef UNARY_OPERATOR
#undef BINARY_OPERATOR
#undef COMPOUND_ASSIGNMENT_OPERATOR
};
