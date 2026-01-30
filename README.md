# Strict enum

Header only library for enums with values that can be only one of enumerators, which gives opportunity to better optimize conditions and switches with enums

### How it works
Strict enum is a class with conversion to enum.
Inside conversion to enum there is similar code:
~~~cpp
if(value != Enumerator1 && value != Enumerator2 ...)
{
  assert(false); //Error in debug builds
  std::unreachable(); //Tell compiler that enum value can not be outside of enumerator list
}
~~~

### Defining strict enum

~~~cpp
STRICT_ENUM(MyEnum)
(
  MyEnumerator1,
  MyEnumerator2,
);
~~~

### Using expressions for enumerators
You can use expressions for enumerators like in standard enum:
~~~cpp
template<auto V>
constexpr auto mycxfunc() { return static_cast<int>(V)*2; }

STRICT_ENUM(MyEnum)
(
  MyEnumerator1,
  MyEnumerator2 = mycxfunc<MyEnumerator1>(),
);
~~~

### Specify underlying type
~~~cpp
STRICT_ENUM(MyEnum, std::uint8_t)
(
  E1
);
~~~

### Converting to and from another enum
You can safely convert strict enum to another enum if strict enum is subset of that enum.
If strict enum is not subset of that enum you will get compile error.
~~~cpp
STRICT_ENUM(MyEnum)
(
  MyEnumerator1,
  MyEnumerator2,
);

enum class MyStdEnum
{
  MyEnumerator1,
  MyEnumerator2,
};

STRICT_ENUM(MyEnum2)
(
  MyEnumerator1,
  MyEnumerator2,
  MyEnumerator3,
  MyEnumerator4,
);

enum class MyStdEnum2
{
  MyEnumerator1,
  MyEnumerator2,
  MyEnumerator3,
  MyEnumerator4,
};

//MyEnum myenum = MyEnum2::MyEnumerator1; //Error

//MyEnum myenum = static_cast<MyEnum>(MyEnum2::MyEnumerator1); //Error

//MyEnum myenum22 = static_cast<MyEnum>(MyStdEnum::MyEnumerator1); //Error
//We can`t convert std enum to strict enum, because we can`t inspect all std enumerators

MyEnum2 myenum1 = MyEnum2(MyEnum::MyEnumerator1); //Ok MyEnum is subset of MyEnum2

MyEnum myenum = MyEnum::MyEnumerator1;

MyEnum2 myenum2 = static_cast<MyEnum2>(myenum); //Ok MyEnum is subset of MyEnum2
MyEnum2 myenum3(myenum); //Ok MyEnum is subset of MyEnum2

MyStdEnum2 mystdenum = static_cast<MyStdEnum2>(myenum); //Ok MyEnum is subset of MyStdEnum2
MyStdEnum2 mystdenum2(myenum); //Ok MyEnum is subset of MyStdEnum2
~~~

### Conversion to underlying type
~~~cpp
STRICT_ENUM(MyEnum, std::uint8_t)
(
  E1
);

using MyEnumUnderlying = strict_enum::underlying_type<MyEnum>;
auto myenum_underlying = strict_enum::to_underlying(MyEnum::E1);
~~~

### Get enumerators array
You can get array of enumerators or its values
~~~cpp
STRICT_ENUM(MyEnum, std::uint8_t)
(
  E1, E2, E3
);

static_assert(MyEnum::enumerators[0] == MyEnum::E1);
static_assert(MyEnum::values[0] == 0);
static_assert(MyEnum::count() == 3);
~~~

### Limitations
* Maximum enumerators count is 256
* Can`t use preprocessor directives in enumerator list
* MSVC currently ignores std::unreachable, [[assume(...)]], __builtin_assume() hints

#### Requirements: C++23

## Using with cmake
~~~cmake
include(FetchContent)

FetchContent_Declare(
  strict_enum
  GIT_REPOSITORY https://github.com/SanyaNya/strict_enum
  GIT_TAG v1.5.0)

FetchContent_MakeAvailable(strict_enum)

target_link_libraries(your_target strict_enum)
~~~
