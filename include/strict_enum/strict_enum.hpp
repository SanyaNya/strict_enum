#pragma once

#include <utility>
#include <cassert>

namespace detail::strict_enum
{

template<typename E>
struct eat_assign
{
  E value;
  constexpr eat_assign(E e) noexcept : value(e) {}

  constexpr operator E() const noexcept { return value; }

  constexpr E operator=(auto&&) noexcept { return value; }
};

} //nemspace detail

//Convert EnumValue = expression to EnumValue
#define DETAIL_STRICT_ENUM_EAT_ASSIGN(E, EXPR) static_cast<E>((::detail::strict_enum::eat_assign<E>)E::EXPR)

//Rescan macro 256 times
#define DETAIL_STRICT_ENUM_RESCAN(...)  DETAIL_STRICT_ENUM_RESCAN1(DETAIL_STRICT_ENUM_RESCAN1(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN1(...) DETAIL_STRICT_ENUM_RESCAN2(DETAIL_STRICT_ENUM_RESCAN2(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN2(...) DETAIL_STRICT_ENUM_RESCAN3(DETAIL_STRICT_ENUM_RESCAN3(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN3(...) DETAIL_STRICT_ENUM_RESCAN4(DETAIL_STRICT_ENUM_RESCAN4(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN4(...) DETAIL_STRICT_ENUM_RESCAN5(DETAIL_STRICT_ENUM_RESCAN5(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN5(...) DETAIL_STRICT_ENUM_RESCAN6(DETAIL_STRICT_ENUM_RESCAN6(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN6(...) DETAIL_STRICT_ENUM_RESCAN7(DETAIL_STRICT_ENUM_RESCAN7(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN7(...) DETAIL_STRICT_ENUM_RESCAN8(DETAIL_STRICT_ENUM_RESCAN8(__VA_ARGS__))
#define DETAIL_STRICT_ENUM_RESCAN8(...) __VA_ARGS__

//Recursive macro
//Check out https://www.scs.stanford.edu/~dm/blog/va-opt.html
#define DETAIL_STRICT_ENUM_PARENS ()
#define DETAIL_STRICT_ENUM_F_AGAIN() DETAIL_STRICT_ENUM_F
#define DETAIL_STRICT_ENUM_F(E, VAL, ARG, ...) (VAL != DETAIL_STRICT_ENUM_EAT_ASSIGN(E, ARG)) __VA_OPT__(&& DETAIL_STRICT_ENUM_F_AGAIN DETAIL_STRICT_ENUM_PARENS(E, VAL, __VA_ARGS__))
#define DETAIL_STRICT_ENUM_INVALID_RANGE(E, VAL, ...) DETAIL_STRICT_ENUM_RESCAN(DETAIL_STRICT_ENUM_F(E, VAL, __VA_ARGS__))

#ifdef _MSC_VER
#define DETAIL_STRICT_ENUM_ALWAYS_INLINE __forceinline
#else
#define DETAIL_STRICT_ENUM_ALWAYS_INLINE [[gnu::always_inline]] inline
#endif

#define STRICT_ENUM(NAME, ...)                        \
struct NAME                                           \
{                                                     \
    constexpr NAME() noexcept = default;              \
    constexpr NAME(auto&& v) noexcept : m_value(v) {} \
                                                      \
    enum class EnumType_ __VA_OPT__(: __VA_ARGS__)    \
DETAIL_STRICT_ENUM_ENUMERATORS

#define DETAIL_STRICT_ENUM_ENUMERATORS(...)                                                  \
    {                                                                                        \
        __VA_ARGS__                                                                          \
    };                                                                                       \
                                                                                             \
    using enum EnumType_;                                                                    \
                                                                                             \
    DETAIL_STRICT_ENUM_ALWAYS_INLINE                                                         \
    __VA_OPT__(constexpr) operator EnumType_() const noexcept                                \
    {                                                                                        \
        /*Tell compiler that enum value not equal to one of enumerators is unreachable*/     \
        __VA_OPT__(if(DETAIL_STRICT_ENUM_INVALID_RANGE(EnumType_, m_value, __VA_ARGS__)))    \
        {                                                                                    \
          assert(((void)"Invalid enum value", false));                                       \
          std::unreachable();                                                                \
        }                                                                                    \
                                                                                             \
        return m_value;                                                                      \
    }                                                                                        \
                                                                                             \
private:                                                                                     \
  EnumType_ m_value;                                                                         \
}
