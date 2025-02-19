#pragma once

#include <utility>
#include <cassert>
#include <type_traits>
#include <hpp/foreach.hpp>

namespace strict_enum::detail
{

template<typename E>
struct eat_assign
{
  E value;
  constexpr eat_assign(E e) noexcept : value(e) {}

  constexpr operator E() const noexcept { return value; }

  constexpr E operator=(auto&&) noexcept { return value; }
};

template<typename E>
using strict_enum_from_enumerator_t = decltype(detail_strict_enum_type_by_enumerator(std::declval<E>()));

template<typename E, typename = void>
struct is_strict_enumerator : std::false_type{};

template<typename E>
struct is_strict_enumerator<E, std::void_t<strict_enum_from_enumerator_t<E>>> : std::true_type{};

template<typename E>
constexpr bool is_strict_enumerator_v = is_strict_enumerator<E>::value;

} //nemspace detail

//Convert EnumValue = expression to EnumValue
#define DETAIL_STRICT_ENUM_EAT_ASSIGN(E, EXPR) static_cast<E>((::strict_enum::detail::eat_assign<E>)E::EXPR)

#define DETAIL_STRICT_ENUM_INVALID_RANGE_F(E, VAL, ARG) (VAL != DETAIL_STRICT_ENUM_EAT_ASSIGN(E, ARG))
#define DETAIL_STRICT_ENUM_INVALID_RANGE(E, VAL, ...) HPP_FOREACH(DETAIL_STRICT_ENUM_INVALID_RANGE_F, HPP_SEP_LOG_AND, (E, VAL), __VA_ARGS__)

#define DETAIL_STRICT_ENUM_CONVERTIBLE_RANGE_F(E1, E2, ARG) (std::to_underlying(DETAIL_STRICT_ENUM_EAT_ASSIGN(E2, ARG)) == std::to_underlying(DETAIL_STRICT_ENUM_EAT_ASSIGN(E1, ARG)))
#define DETAIL_STRICT_ENUM_CONVERTIBLE_RANGE(E1, E2, ...) HPP_FOREACH(DETAIL_STRICT_ENUM_CONVERTIBLE_RANGE_F, HPP_SEP_LOG_AND, (E1, E2), __VA_ARGS__)

#ifdef _MSC_VER
#define DETAIL_STRICT_ENUM_ALWAYS_INLINE __forceinline
#else
#define DETAIL_STRICT_ENUM_ALWAYS_INLINE [[gnu::always_inline]] inline
#endif

#define STRICT_ENUM(NAME, ...)                                                               \
struct NAME                                                                                  \
{                                                                                            \
    enum class EnumType_ __VA_OPT__(: __VA_ARGS__);                                          \
                                                                                             \
    friend NAME detail_strict_enum_type_by_enumerator(EnumType_);                            \
                                                                                             \
    constexpr NAME() noexcept = default;                                                     \
                                                                                             \
    constexpr NAME(EnumType_ e) noexcept : m_value_(e) {}                                    \
                                                                                             \
    template<typename E>                                                                     \
      requires (!std::is_same_v<std::remove_cvref_t<E>, EnumType_> &&                        \
                strict_enum::detail::is_strict_enumerator_v<E>)                              \
    explicit constexpr NAME(E e) noexcept                                                    \
      : m_value_(                                                                            \
          static_cast<EnumType_>(                                                            \
            strict_enum::detail::strict_enum_from_enumerator_t<E>(e))) {}                    \
                                                                                             \
    enum class EnumType_ __VA_OPT__(: __VA_ARGS__)                                           \
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
        __VA_OPT__(if(DETAIL_STRICT_ENUM_INVALID_RANGE(EnumType_, m_value_, __VA_ARGS__)))   \
        {                                                                                    \
          assert(((void)"Invalid enum value", false));                                       \
          std::unreachable();                                                                \
        }                                                                                    \
                                                                                             \
        return m_value_;                                                                     \
    }                                                                                        \
                                                                                             \
    DETAIL_STRICT_ENUM_ALWAYS_INLINE explicit __VA_OPT__(constexpr)                          \
    operator std::underlying_type_t<EnumType_>() const noexcept                              \
    {                                                                                        \
      return std::to_underlying(static_cast<EnumType_>(*this));                              \
    }                                                                                        \
                                                                                             \
    template<typename E> requires                                                            \
      std::is_enum_v<E> && (!std::is_same_v<E, EnumType_>)                                   \
      __VA_OPT__(&& DETAIL_STRICT_ENUM_CONVERTIBLE_RANGE(EnumType_, E, __VA_ARGS__))         \
    DETAIL_STRICT_ENUM_ALWAYS_INLINE                                                         \
    explicit __VA_OPT__(constexpr) operator E() const noexcept                               \
    {                                                                                        \
      return                                                                                 \
        static_cast<E>(                                                                      \
          static_cast<std::underlying_type_t<E>>(                                            \
            static_cast<EnumType_>(*this)));                                                 \
    }                                                                                        \
                                                                                             \
  EnumType_ m_value_;                                                                        \
}

namespace strict_enum
{

template<typename E, bool b = std::is_enum_v<E>, typename = void>
struct underlying_type {};

template<typename E>
struct underlying_type<E, false, std::void_t<typename E::EnumType_>> :
  std::underlying_type<typename E::EnumType_> {};

template<typename E>
struct underlying_type<E, true, void> :
  std::underlying_type<E> {};

template<typename E>
using underlying_type_t = typename underlying_type<E>::type;

template<typename E>
constexpr underlying_type_t<E> to_underlying(E e) noexcept
{
  return static_cast<underlying_type_t<E>>(e);
}

} //namespace strict_enum
