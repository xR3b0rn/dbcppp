///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GSL_SPAN_H
#define GSL_SPAN_H

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GSL_CONTRACTS_H
#define GSL_CONTRACTS_H

#include <exception>
#include <stdexcept> // for logic_error

//
// make suppress attributes parse for some compilers
// Hopefully temporary until suppression standardization occurs
//
#if defined(__clang__)
#define GSL_SUPPRESS(x) [[gsl::suppress("x")]]
#else
#if defined(_MSC_VER)
#define GSL_SUPPRESS(x) [[gsl::suppress(x)]]
#else
#define GSL_SUPPRESS(x)
#endif // _MSC_VER
#endif // __clang__

//
// Temporary until MSVC STL supports no-exceptions mode.
// Currently terminate is a no-op in this mode, so we add termination behavior back
//
#if defined(_MSC_VER) && defined(_HAS_EXCEPTIONS) && !_HAS_EXCEPTIONS
#define GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND
#include <intrin.h>
#define RANGE_CHECKS_FAILURE 0

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-noreturn"
#endif

#endif

//
// There are three configuration options for this GSL implementation's behavior
// when pre/post conditions on the GSL types are violated:
//
// 1. GSL_TERMINATE_ON_CONTRACT_VIOLATION: std::terminate will be called (default)
// 2. GSL_THROW_ON_CONTRACT_VIOLATION: a gsl::fail_fast exception will be thrown
// 3. GSL_UNENFORCED_ON_CONTRACT_VIOLATION: nothing happens
//
#if !(defined(GSL_THROW_ON_CONTRACT_VIOLATION) || defined(GSL_TERMINATE_ON_CONTRACT_VIOLATION) ||  \
      defined(GSL_UNENFORCED_ON_CONTRACT_VIOLATION))
#define GSL_TERMINATE_ON_CONTRACT_VIOLATION
#endif

#define GSL_STRINGIFY_DETAIL(x) #x
#define GSL_STRINGIFY(x) GSL_STRINGIFY_DETAIL(x)

#if defined(__clang__) || defined(__GNUC__)
#define GSL_LIKELY(x) __builtin_expect(!!(x), 1)
#define GSL_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define GSL_LIKELY(x) (!!(x))
#define GSL_UNLIKELY(x) (!!(x))
#endif

//
// GSL_ASSUME(cond)
//
// Tell the optimizer that the predicate cond must hold. It is unspecified
// whether or not cond is actually evaluated.
//
#ifdef _MSC_VER
#define GSL_ASSUME(cond) __assume(cond)
#elif defined(__GNUC__)
#define GSL_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define GSL_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#endif

//
// GSL.assert: assertions
//

namespace gsl
{
struct fail_fast : public std::logic_error
{
    explicit fail_fast(char const* const message) : std::logic_error(message) {}
};

namespace details
{
#if defined(GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND)

    typedef void  (__cdecl *terminate_handler)();

    GSL_SUPPRESS(f.6) // NO-FORMAT: attribute
    [[noreturn]] inline void __cdecl default_terminate_handler()
    {
        __fastfail(RANGE_CHECKS_FAILURE);
    }

    inline gsl::details::terminate_handler& get_terminate_handler() noexcept
    {
        static terminate_handler handler = &default_terminate_handler;
        return handler;
    }

#endif

    [[noreturn]] inline void terminate() noexcept
    {
#if defined(GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND)
        (*gsl::details::get_terminate_handler())();
#else
        std::terminate();
#endif
    }

#if defined(GSL_TERMINATE_ON_CONTRACT_VIOLATION)

    template <typename Exception>
    [[noreturn]] void throw_exception(Exception&&) noexcept
    {
        gsl::details::terminate();
    }

#else

    template <typename Exception>
    [[noreturn]] void throw_exception(Exception&& exception)
    {
        throw std::forward<Exception>(exception);
    }

#endif // GSL_TERMINATE_ON_CONTRACT_VIOLATION

} // namespace details
} // namespace gsl

#if defined(GSL_THROW_ON_CONTRACT_VIOLATION)

#define GSL_CONTRACT_CHECK(type, cond)                                                             \
    (GSL_LIKELY(cond) ? static_cast<void>(0)                                                       \
                      : gsl::details::throw_exception(gsl::fail_fast(                              \
                            "GSL: " type " failure at " __FILE__ ": " GSL_STRINGIFY(__LINE__))))

#elif defined(GSL_TERMINATE_ON_CONTRACT_VIOLATION)

#define GSL_CONTRACT_CHECK(type, cond)                                                             \
    (GSL_LIKELY(cond) ? static_cast<void>(0) : gsl::details::terminate())

#elif defined(GSL_UNENFORCED_ON_CONTRACT_VIOLATION)

#define GSL_CONTRACT_CHECK(type, cond) GSL_ASSUME(cond)

#endif // GSL_THROW_ON_CONTRACT_VIOLATION

#define Expects(cond) GSL_CONTRACT_CHECK("Precondition", cond)
#define Ensures(cond) GSL_CONTRACT_CHECK("Postcondition", cond)

#if defined(GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // GSL_CONTRACTS_H
    
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GSL_BYTE_H
#define GSL_BYTE_H

//
// make suppress attributes work for some compilers
// Hopefully temporary until suppression standardization occurs
//
#if defined(__clang__)
#define GSL_SUPPRESS(x) [[gsl::suppress("x")]]
#else
#if defined(_MSC_VER)
#define GSL_SUPPRESS(x) [[gsl::suppress(x)]]
#else
#define GSL_SUPPRESS(x)
#endif // _MSC_VER
#endif // __clang__

#include <type_traits>

#ifdef _MSC_VER

#pragma warning(push)

// Turn MSVC /analyze rules that generate too much noise. TODO: fix in the tool.
#pragma warning(disable : 26493) // don't use c-style casts // TODO: MSVC suppression in templates does not always work

#ifndef GSL_USE_STD_BYTE
// this tests if we are under MSVC and the standard lib has std::byte and it is enabled
#if defined(_HAS_STD_BYTE) && _HAS_STD_BYTE

#define GSL_USE_STD_BYTE 1

#else // defined(_HAS_STD_BYTE) && _HAS_STD_BYTE

#define GSL_USE_STD_BYTE 0

#endif // defined(_HAS_STD_BYTE) && _HAS_STD_BYTE
#endif // GSL_USE_STD_BYTE

#else // _MSC_VER

#ifndef GSL_USE_STD_BYTE
// this tests if we are under GCC or Clang with enough -std:c++1z power to get us std::byte
// also check if libc++ version is sufficient (> 5.0) or libstc++ actually contains std::byte
#if defined(__cplusplus) && (__cplusplus >= 201703L) && \
  (defined(__cpp_lib_byte) && (__cpp_lib_byte >= 201603)  || \
   defined(_LIBCPP_VERSION) && (_LIBCPP_VERSION >= 5000))

#define GSL_USE_STD_BYTE 1
#include <cstddef>

#else // defined(__cplusplus) && (__cplusplus >= 201703L) &&
      //   (defined(__cpp_lib_byte) && (__cpp_lib_byte >= 201603)  ||
      //    defined(_LIBCPP_VERSION) && (_LIBCPP_VERSION >= 5000))

#define GSL_USE_STD_BYTE 0

#endif //defined(__cplusplus) && (__cplusplus >= 201703L) &&
       //   (defined(__cpp_lib_byte) && (__cpp_lib_byte >= 201603)  ||
       //    defined(_LIBCPP_VERSION) && (_LIBCPP_VERSION >= 5000))
#endif // GSL_USE_STD_BYTE

#endif // _MSC_VER

// Use __may_alias__ attribute on gcc and clang
#if defined __clang__ || (defined(__GNUC__) && __GNUC__ > 5)
#define byte_may_alias __attribute__((__may_alias__))
#else // defined __clang__ || defined __GNUC__
#define byte_may_alias
#endif // defined __clang__ || defined __GNUC__

namespace gsl
{
#if GSL_USE_STD_BYTE

using std::byte;
using std::to_integer;

#else // GSL_USE_STD_BYTE

// This is a simple definition for now that allows
// use of byte within span<> to be standards-compliant
enum class byte_may_alias byte : unsigned char
{
};

template <class IntegerType, class = std::enable_if_t<std::is_integral<IntegerType>::value>>
constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept
{
    return b = byte(static_cast<unsigned char>(b) << shift);
}

template <class IntegerType, class = std::enable_if_t<std::is_integral<IntegerType>::value>>
constexpr byte operator<<(byte b, IntegerType shift) noexcept
{
    return byte(static_cast<unsigned char>(b) << shift);
}

template <class IntegerType, class = std::enable_if_t<std::is_integral<IntegerType>::value>>
constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept
{
    return b = byte(static_cast<unsigned char>(b) >> shift);
}

template <class IntegerType, class = std::enable_if_t<std::is_integral<IntegerType>::value>>
constexpr byte operator>>(byte b, IntegerType shift) noexcept
{
    return byte(static_cast<unsigned char>(b) >> shift);
}

constexpr byte& operator|=(byte& l, byte r) noexcept
{
    return l = byte(static_cast<unsigned char>(l) | static_cast<unsigned char>(r));
}

constexpr byte operator|(byte l, byte r) noexcept
{
    return byte(static_cast<unsigned char>(l) | static_cast<unsigned char>(r));
}

constexpr byte& operator&=(byte& l, byte r) noexcept
{
    return l = byte(static_cast<unsigned char>(l) & static_cast<unsigned char>(r));
}

constexpr byte operator&(byte l, byte r) noexcept
{
    return byte(static_cast<unsigned char>(l) & static_cast<unsigned char>(r));
}

constexpr byte& operator^=(byte& l, byte r) noexcept
{
    return l = byte(static_cast<unsigned char>(l) ^ static_cast<unsigned char>(r));
}

constexpr byte operator^(byte l, byte r) noexcept
{
    return byte(static_cast<unsigned char>(l) ^ static_cast<unsigned char>(r));
}

constexpr byte operator~(byte b) noexcept { return byte(~static_cast<unsigned char>(b)); }

template <class IntegerType, class = std::enable_if_t<std::is_integral<IntegerType>::value>>
constexpr IntegerType to_integer(byte b) noexcept
{
    return static_cast<IntegerType>(b);
}

#endif // GSL_USE_STD_BYTE

template <bool E, typename T>
constexpr byte to_byte_impl(T t) noexcept
{
    static_assert(
        E, "gsl::to_byte(t) must be provided an unsigned char, otherwise data loss may occur. "
           "If you are calling to_byte with an integer contant use: gsl::to_byte<t>() version.");
    return static_cast<byte>(t);
}
template <>
// NOTE: need suppression since c++14 does not allow "return {t}"
// GSL_SUPPRESS(type.4) // NO-FORMAT: attribute // TODO: suppression does not work
constexpr byte to_byte_impl<true, unsigned char>(unsigned char t) noexcept
{
    return byte(t);
}

template <typename T>
constexpr byte to_byte(T t) noexcept
{
    return to_byte_impl<std::is_same<T, unsigned char>::value, T>(t);
}

template <int I>
constexpr byte to_byte() noexcept
{
    static_assert(I >= 0 && I <= 255,
                  "gsl::byte only has 8 bits of storage, values must be in range 0-255");
    return static_cast<byte>(I);
}

} // namespace gsl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // GSL_BYTE_H
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GSL_UTIL_H
#define GSL_UTIL_H

#include <array>
#include <cstddef>          // for ptrdiff_t, size_t
#include <exception>        // for exception
#include <initializer_list> // for initializer_list
#include <type_traits>      // for is_signed, integral_constant
#include <utility>          // for forward

#if defined(_MSC_VER) && !defined(__clang__)

#pragma warning(push)
#pragma warning(disable : 4127) // conditional expression is constant

#if _MSC_VER < 1910
#pragma push_macro("constexpr")
#define constexpr /*constexpr*/
#endif            // _MSC_VER < 1910
#endif            // _MSC_VER

#if (defined(_MSC_VER) && _MSC_VER < 1910) || (!defined(__clang__) && defined(__GNUC__) && __GUNC__ < 6)
#define GSL_CONSTEXPR_NARROW 0
#else
#define GSL_CONSTEXPR_NARROW 1
#endif

namespace gsl
{
//
// GSL.util: utilities
//

// index type for all container indexes/subscripts/sizes
using index = std::ptrdiff_t;

// final_action allows you to ensure something gets run at the end of a scope
template <class F>
class final_action
{
public:
    explicit final_action(F f) noexcept : f_(std::move(f)) {}

    final_action(final_action&& other) noexcept : f_(std::move(other.f_)), invoke_(other.invoke_)
    {
        other.invoke_ = false;
    }

    final_action(const final_action&) = delete;
    final_action& operator=(const final_action&) = delete;
    final_action& operator=(final_action&&) = delete;

    GSL_SUPPRESS(f.6) // NO-FORMAT: attribute // terminate if throws
    ~final_action() noexcept
    {
        if (invoke_) f_();
    }

private:
    F f_;
    bool invoke_{true};
};

// finally() - convenience function to generate a final_action
template <class F>
final_action<F> finally(const F& f) noexcept
{
    return final_action<F>(f);
}

template <class F>
final_action<F> finally(F&& f) noexcept
{
    return final_action<F>(std::forward<F>(f));
}

// narrow_cast(): a searchable way to do narrowing casts of values
template <class T, class U>
GSL_SUPPRESS(type.1) // NO-FORMAT: attribute
constexpr T narrow_cast(U&& u) noexcept
{
    return static_cast<T>(std::forward<U>(u));
}

struct narrowing_error : public std::exception
{
};

namespace details
{
    template <class T, class U>
    struct is_same_signedness
        : public std::integral_constant<bool, std::is_signed<T>::value == std::is_signed<U>::value>
    {
    };
} // namespace details

// narrow() : a checked version of narrow_cast() that throws if the cast changed the value
template <class T, class U>
GSL_SUPPRESS(type.1) // NO-FORMAT: attribute
GSL_SUPPRESS(f.6) // NO-FORMAT: attribute // TODO: MSVC /analyze does not recognise noexcept(false)
#if GSL_CONSTEXPR_NARROW
constexpr
#endif
T narrow(U u) noexcept(false)
{
    T t = narrow_cast<T>(u);
    if (static_cast<U>(t) != u) gsl::details::throw_exception(narrowing_error());
    if (!details::is_same_signedness<T, U>::value && ((t < T{}) != (u < U{})))
        gsl::details::throw_exception(narrowing_error());
    return t;
}

//
// at() - Bounds-checked way of accessing builtin arrays, std::array, std::vector
//
template <class T, std::size_t N>
GSL_SUPPRESS(bounds.4) // NO-FORMAT: attribute
GSL_SUPPRESS(bounds.2) // NO-FORMAT: attribute
constexpr T& at(T (&arr)[N], const index i)
{
    Expects(i >= 0 && i < narrow_cast<index>(N));
    return arr[narrow_cast<std::size_t>(i)];
}

template <class Cont>
GSL_SUPPRESS(bounds.4) // NO-FORMAT: attribute
GSL_SUPPRESS(bounds.2) // NO-FORMAT: attribute
constexpr auto at(Cont& cont, const index i) -> decltype(cont[cont.size()])
{
    Expects(i >= 0 && i < narrow_cast<index>(cont.size()));
    using size_type = decltype(cont.size());
    return cont[narrow_cast<size_type>(i)];
}

template <class T>
GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
constexpr T at(const std::initializer_list<T> cont, const index i)
{
    Expects(i >= 0 && i < narrow_cast<index>(cont.size()));
    return *(cont.begin() + i);
}

} // namespace gsl

#if defined(_MSC_VER) && !defined(__clang__)
#if _MSC_VER < 1910
#undef constexpr
#pragma pop_macro("constexpr")

#endif // _MSC_VER < 1910

#pragma warning(pop)

#endif // _MSC_VER

#endif // GSL_UTIL_H

#include <algorithm> // for lexicographical_compare
#include <array>     // for array
#include <cstddef>   // for ptrdiff_t, size_t, nullptr_t
#include <iterator>  // for reverse_iterator, distance, random_access_...
#include <limits>
#include <stdexcept>
#include <type_traits> // for enable_if_t, declval, is_convertible, inte...
#include <utility>
#include <memory> // for std::addressof

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)

// turn off some warnings that are noisy about our Expects statements
#pragma warning(disable : 4127) // conditional expression is constant
#pragma warning(disable : 4702) // unreachable code

// Turn MSVC /analyze rules that generate too much noise. TODO: fix in the tool.
#pragma warning(disable : 26495) // uninitalized member when constructor calls constructor
#pragma warning(disable : 26446) // parser bug does not allow attributes on some templates

#if _MSC_VER < 1910
#pragma push_macro("constexpr")
#define constexpr /*constexpr*/
#define GSL_USE_STATIC_CONSTEXPR_WORKAROUND

#endif // _MSC_VER < 1910
#endif // _MSC_VER

// See if we have enough C++17 power to use a static constexpr data member
// without needing an out-of-line definition
#if !(defined(__cplusplus) && (__cplusplus >= 201703L))
#define GSL_USE_STATIC_CONSTEXPR_WORKAROUND
#endif // !(defined(__cplusplus) && (__cplusplus >= 201703L))

// GCC 7 does not like the signed unsigned missmatch (size_t ptrdiff_t)
// While there is a conversion from signed to unsigned, it happens at
// compiletime, so the compiler wouldn't have to warn indiscriminently, but
// could check if the source value actually doesn't fit into the target type
// and only warn in those cases.
#if defined(__GNUC__) && __GNUC__ > 6
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif

namespace gsl
{

// [views.constants], constants
constexpr const std::ptrdiff_t dynamic_extent = -1;

template <class ElementType, std::ptrdiff_t Extent = dynamic_extent>
class span;

// implementation details
namespace details
{
    template <class T>
    struct is_span_oracle : std::false_type
    {
    };

    template <class ElementType, std::ptrdiff_t Extent>
    struct is_span_oracle<gsl::span<ElementType, Extent>> : std::true_type
    {
    };

    template <class T>
    struct is_span : public is_span_oracle<std::remove_cv_t<T>>
    {
    };

    template <class T>
    struct is_std_array_oracle : std::false_type
    {
    };

    template <class ElementType, std::size_t Extent>
    struct is_std_array_oracle<std::array<ElementType, Extent>> : std::true_type
    {
    };

    template <class T>
    struct is_std_array : public is_std_array_oracle<std::remove_cv_t<T>>
    {
    };

    template <std::ptrdiff_t From, std::ptrdiff_t To>
    struct is_allowed_extent_conversion
        : public std::integral_constant<bool, From == To || From == gsl::dynamic_extent ||
                                                  To == gsl::dynamic_extent>
    {
    };

    template <class From, class To>
    struct is_allowed_element_type_conversion
        : public std::integral_constant<bool, std::is_convertible<From (*)[], To (*)[]>::value>
    {
    };

    template <class Span, bool IsConst>
    class span_iterator
    {
        using element_type_ = typename Span::element_type;

    public:
#ifdef _MSC_VER
        // Tell Microsoft standard library that span_iterators are checked.
        using _Unchecked_type = typename Span::pointer;
#endif

        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::remove_cv_t<element_type_>;
        using difference_type = typename Span::index_type;

        using reference = std::conditional_t<IsConst, const element_type_, element_type_>&;
        using pointer = std::add_pointer_t<reference>;

        span_iterator() = default;

        constexpr span_iterator(const Span* span, typename Span::index_type idx) noexcept
            : span_(span), index_(idx)
        {}

        friend span_iterator<Span, true>;
        template <bool B, std::enable_if_t<!B && IsConst>* = nullptr>
        constexpr span_iterator(const span_iterator<Span, B>& other) noexcept
            : span_iterator(other.span_, other.index_)
        {}

        GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
        constexpr reference operator*() const
        {
            Expects(index_ != span_->size());
            return *(span_->data() + index_);
        }

        constexpr pointer operator->() const
        {
            Expects(index_ != span_->size());
            return span_->data() + index_;
        }

        constexpr span_iterator& operator++()
        {
            Expects(0 <= index_ && index_ != span_->size());
            ++index_;
            return *this;
        }

        constexpr span_iterator operator++(int)
        {
            auto ret = *this;
            ++(*this);
            return ret;
        }

        constexpr span_iterator& operator--()
        {
            Expects(index_ != 0 && index_ <= span_->size());
            --index_;
            return *this;
        }

        constexpr span_iterator operator--(int)
        {
            auto ret = *this;
            --(*this);
            return ret;
        }

        constexpr span_iterator operator+(difference_type n) const
        {
            auto ret = *this;
            return ret += n;
        }

        friend constexpr span_iterator operator+(difference_type n, span_iterator const& rhs)
        {
            return rhs + n;
        }

        constexpr span_iterator& operator+=(difference_type n)
        {
            Expects((index_ + n) >= 0 && (index_ + n) <= span_->size());
            index_ += n;
            return *this;
        }

        constexpr span_iterator operator-(difference_type n) const
        {
            auto ret = *this;
            return ret -= n;
        }

        constexpr span_iterator& operator-=(difference_type n) { return *this += -n; }

        constexpr difference_type operator-(span_iterator rhs) const
        {
            Expects(span_ == rhs.span_);
            return index_ - rhs.index_;
        }

        constexpr reference operator[](difference_type n) const { return *(*this + n); }

        constexpr friend bool operator==(span_iterator lhs, span_iterator rhs) noexcept
        {
            return lhs.span_ == rhs.span_ && lhs.index_ == rhs.index_;
        }

        constexpr friend bool operator!=(span_iterator lhs, span_iterator rhs) noexcept
        {
            return !(lhs == rhs);
        }

        constexpr friend bool operator<(span_iterator lhs, span_iterator rhs) noexcept
        {
            return lhs.index_ < rhs.index_;
        }

        constexpr friend bool operator<=(span_iterator lhs, span_iterator rhs) noexcept
        {
            return !(rhs < lhs);
        }

        constexpr friend bool operator>(span_iterator lhs, span_iterator rhs) noexcept
        {
            return rhs < lhs;
        }

        constexpr friend bool operator>=(span_iterator lhs, span_iterator rhs) noexcept
        {
            return !(rhs > lhs);
        }

#ifdef _MSC_VER
        // MSVC++ iterator debugging support; allows STL algorithms in 15.8+
        // to unwrap span_iterator to a pointer type after a range check in STL
        // algorithm calls
        friend constexpr void _Verify_range(span_iterator lhs, span_iterator rhs) noexcept
        { // test that [lhs, rhs) forms a valid range inside an STL algorithm
            Expects(lhs.span_ == rhs.span_        // range spans have to match
                    && lhs.index_ <= rhs.index_); // range must not be transposed
        }

        constexpr void _Verify_offset(const difference_type n) const noexcept
        { // test that the iterator *this + n is a valid range in an STL
            // algorithm call
            Expects((index_ + n) >= 0 && (index_ + n) <= span_->size());
        }

        GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
        constexpr pointer _Unwrapped() const noexcept
        { // after seeking *this to a high water mark, or using one of the
            // _Verify_xxx functions above, unwrap this span_iterator to a raw
            // pointer
            return span_->data() + index_;
        }

        // Tell the STL that span_iterator should not be unwrapped if it can't
        // validate in advance, even in release / optimized builds:
#if defined(GSL_USE_STATIC_CONSTEXPR_WORKAROUND)
        static constexpr const bool _Unwrap_when_unverified = false;
#else
        static constexpr bool _Unwrap_when_unverified = false;
#endif
        GSL_SUPPRESS(con.3) // NO-FORMAT: attribute // TODO: false positive
        constexpr void _Seek_to(const pointer p) noexcept
        { // adjust the position of *this to previously verified location p
            // after _Unwrapped
            index_ = p - span_->data();
        }
#endif

    protected:
        const Span* span_ = nullptr;
        std::ptrdiff_t index_ = 0;
    };

    template <std::ptrdiff_t Ext>
    class extent_type
    {
    public:
        using index_type = std::ptrdiff_t;

        static_assert(Ext >= 0, "A fixed-size span must be >= 0 in size.");

        constexpr extent_type() noexcept {}

        template <index_type Other>
        constexpr extent_type(extent_type<Other> ext)
        {
            static_assert(Other == Ext || Other == dynamic_extent,
                          "Mismatch between fixed-size extent and size of initializing data.");
            Expects(ext.size() == Ext);
        }

        constexpr extent_type(index_type size) { Expects(size == Ext); }

        constexpr index_type size() const noexcept { return Ext; }
    };

    template <>
    class extent_type<dynamic_extent>
    {
    public:
        using index_type = std::ptrdiff_t;

        template <index_type Other>
        explicit constexpr extent_type(extent_type<Other> ext) : size_(ext.size())
        {}

        explicit constexpr extent_type(index_type size) : size_(size) { Expects(size >= 0); }

        constexpr index_type size() const noexcept { return size_; }

    private:
        index_type size_;
    };

    template <class ElementType, std::ptrdiff_t Extent, std::ptrdiff_t Offset, std::ptrdiff_t Count>
    struct calculate_subspan_type
    {
        using type = span<ElementType, Count != dynamic_extent
                                           ? Count
                                           : (Extent != dynamic_extent ? Extent - Offset : Extent)>;
    };
} // namespace details

// [span], class template span
template <class ElementType, std::ptrdiff_t Extent>
class span
{
public:
    // constants and types
    using element_type = ElementType;
    using value_type = std::remove_cv_t<ElementType>;
    using index_type = std::ptrdiff_t;
    using pointer = element_type*;
    using reference = element_type&;

    using iterator = details::span_iterator<span<ElementType, Extent>, false>;
    using const_iterator = details::span_iterator<span<ElementType, Extent>, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using size_type = index_type;

#if defined(GSL_USE_STATIC_CONSTEXPR_WORKAROUND)
    static constexpr const index_type extent{Extent};
#else
    static constexpr index_type extent{Extent};
#endif

    // [span.cons], span constructors, copy, assignment, and destructor
    template <bool Dependent = false,
              // "Dependent" is needed to make "std::enable_if_t<Dependent || Extent <= 0>" SFINAE,
              // since "std::enable_if_t<Extent <= 0>" is ill-formed when Extent is greater than 0.
              class = std::enable_if_t<(Dependent || Extent <= 0)>>
    constexpr span() noexcept : storage_(nullptr, details::extent_type<0>())
    {}

    constexpr span(pointer ptr, index_type count) : storage_(ptr, count) {}

    constexpr span(pointer firstElem, pointer lastElem)
        : storage_(firstElem, std::distance(firstElem, lastElem))
    {}

    template <std::size_t N>
    constexpr span(element_type (&arr)[N]) noexcept
        : storage_(KnownNotNull{std::addressof(arr[0])}, details::extent_type<N>())
    {}

    template <std::size_t N, class = std::enable_if_t<(N > 0)>>
    constexpr span(std::array<std::remove_const_t<element_type>, N>& arr) noexcept
        : storage_(KnownNotNull{arr.data()}, details::extent_type<N>())
    {
    }

    constexpr span(std::array<std::remove_const_t<element_type>, 0>&) noexcept
        : storage_(static_cast<pointer>(nullptr), details::extent_type<0>())
    {
    }

    template <std::size_t N, class = std::enable_if_t<(N > 0)>>
    constexpr span(const std::array<std::remove_const_t<element_type>, N>& arr) noexcept
        : storage_(KnownNotNull{arr.data()}, details::extent_type<N>())
    {
    }

    constexpr span(const std::array<std::remove_const_t<element_type>, 0>&) noexcept
        : storage_(static_cast<pointer>(nullptr), details::extent_type<0>())
    {
    }

    // NB: the SFINAE here uses .data() as a incomplete/imperfect proxy for the requirement
    // on Container to be a contiguous sequence container.
    template <class Container,
              class = std::enable_if_t<
                  !details::is_span<Container>::value && !details::is_std_array<Container>::value &&
                  std::is_convertible<typename Container::pointer, pointer>::value &&
                  std::is_convertible<typename Container::pointer,
                                      decltype(std::declval<Container>().data())>::value>>
    constexpr span(Container& cont) : span(cont.data(), narrow<index_type>(cont.size()))
    {}

    template <class Container,
              class = std::enable_if_t<
                  std::is_const<element_type>::value && !details::is_span<Container>::value &&
                  std::is_convertible<typename Container::pointer, pointer>::value &&
                  std::is_convertible<typename Container::pointer,
                                      decltype(std::declval<Container>().data())>::value>>
    constexpr span(const Container& cont) : span(cont.data(), narrow<index_type>(cont.size()))
    {}

    constexpr span(const span& other) noexcept = default;

    template <
        class OtherElementType, std::ptrdiff_t OtherExtent,
        class = std::enable_if_t<
            details::is_allowed_extent_conversion<OtherExtent, Extent>::value &&
            details::is_allowed_element_type_conversion<OtherElementType, element_type>::value>>
    constexpr span(const span<OtherElementType, OtherExtent>& other)
        : storage_(other.data(), details::extent_type<OtherExtent>(other.size()))
    {}

    ~span() noexcept = default;
    constexpr span& operator=(const span& other) noexcept = default;

    // [span.sub], span subviews
    template <std::ptrdiff_t Count>
    constexpr span<element_type, Count> first() const
    {
        Expects(Count >= 0 && Count <= size());
        return {data(), Count};
    }

    template <std::ptrdiff_t Count>
    GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
    constexpr span<element_type, Count> last() const
    {
        Expects(Count >= 0 && size() - Count >= 0);
        return {data() + (size() - Count), Count};
    }

    template <std::ptrdiff_t Offset, std::ptrdiff_t Count = dynamic_extent>
    GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
    constexpr auto subspan() const ->
        typename details::calculate_subspan_type<ElementType, Extent, Offset, Count>::type
    {
        Expects((Offset >= 0 && size() - Offset >= 0) &&
                (Count == dynamic_extent || (Count >= 0 && Offset + Count <= size())));

        return {data() + Offset, Count == dynamic_extent ? size() - Offset : Count};
    }

    constexpr span<element_type, dynamic_extent> first(index_type count) const
    {
        Expects(count >= 0 && count <= size());
        return {data(), count};
    }

    constexpr span<element_type, dynamic_extent> last(index_type count) const
    {
        return make_subspan(size() - count, dynamic_extent, subspan_selector<Extent>{});
    }

    constexpr span<element_type, dynamic_extent> subspan(index_type offset,
                                                         index_type count = dynamic_extent) const
    {
        return make_subspan(offset, count, subspan_selector<Extent>{});
    }

    // [span.obs], span observers
    constexpr index_type size() const noexcept { return storage_.size(); }
    constexpr index_type size_bytes() const noexcept
    {
        return size() * narrow_cast<index_type>(sizeof(element_type));
    }
    constexpr bool empty() const noexcept { return size() == 0; }

    // [span.elem], span element access
    GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
    constexpr reference operator[](index_type idx) const
    {
        Expects(CheckRange(idx, storage_.size()));
        return data()[idx];
    }

    constexpr reference at(index_type idx) const { return this->operator[](idx); }
    constexpr reference operator()(index_type idx) const { return this->operator[](idx); }
    constexpr pointer data() const noexcept { return storage_.data(); }

    // [span.iter], span iterator support
    constexpr iterator begin() const noexcept { return {this, 0}; }
    constexpr iterator end() const noexcept { return {this, size()}; }

    constexpr const_iterator cbegin() const noexcept { return {this, 0}; }
    constexpr const_iterator cend() const noexcept { return {this, size()}; }

    constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator{end()}; }
    constexpr reverse_iterator rend() const noexcept { return reverse_iterator{begin()}; }

    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator{cend()};
    }
    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator{cbegin()};
    }

#ifdef _MSC_VER
    // Tell MSVC how to unwrap spans in range-based-for
    constexpr pointer _Unchecked_begin() const noexcept { return data(); }
    constexpr pointer _Unchecked_end() const noexcept
    {
        GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
        return data() + size();
    }
#endif // _MSC_VER

private:
    static constexpr bool CheckRange(index_type idx, index_type size) noexcept
    {
        // Optimization:
        //
        // idx >= 0 && idx < size
        // =>
        // static_cast<size_t>(idx) < static_cast<size_t>(size)
        //
        // because size >=0 by span construction, and negative idx will
        // wrap around to a value always greater than size when casted.

        // check if we have enough space to wrap around
#if defined(__cpp_if_constexpr)
        if constexpr (sizeof(index_type) <= sizeof(size_t))
#else
        if (sizeof(index_type) <= sizeof(size_t))
#endif
        {
            return narrow_cast<size_t>(idx) < narrow_cast<size_t>(size);
        }
        else
        {
            return idx >= 0 && idx < size;
        }
    }

    // Needed to remove unnecessary null check in subspans
    struct KnownNotNull
    {
        pointer p;
    };

    // this implementation detail class lets us take advantage of the
    // empty base class optimization to pay for only storage of a single
    // pointer in the case of fixed-size spans
    template <class ExtentType>
    class storage_type : public ExtentType
    {
    public:
        // KnownNotNull parameter is needed to remove unnecessary null check
        // in subspans and constructors from arrays
        template <class OtherExtentType>
        constexpr storage_type(KnownNotNull data, OtherExtentType ext)
            : ExtentType(ext), data_(data.p)
        {
            Expects(ExtentType::size() >= 0);
        }

        template <class OtherExtentType>
        constexpr storage_type(pointer data, OtherExtentType ext) : ExtentType(ext), data_(data)
        {
            Expects(ExtentType::size() >= 0);
            Expects(data || ExtentType::size() == 0);
        }

        constexpr pointer data() const noexcept { return data_; }

    private:
        pointer data_;
    };

    storage_type<details::extent_type<Extent>> storage_;

    // The rest is needed to remove unnecessary null check
    // in subspans and constructors from arrays
    constexpr span(KnownNotNull ptr, index_type count) : storage_(ptr, count) {}

    template <std::ptrdiff_t CallerExtent>
    class subspan_selector
    {
    };

    template <std::ptrdiff_t CallerExtent>
    span<element_type, dynamic_extent> make_subspan(index_type offset, index_type count,
                                                    subspan_selector<CallerExtent>) const
    {
        const span<element_type, dynamic_extent> tmp(*this);
        return tmp.subspan(offset, count);
    }

    GSL_SUPPRESS(bounds.1) // NO-FORMAT: attribute
    span<element_type, dynamic_extent> make_subspan(index_type offset, index_type count,
                                                    subspan_selector<dynamic_extent>) const
    {
        Expects(offset >= 0 && size() - offset >= 0);

        if (count == dynamic_extent) { return {KnownNotNull{data() + offset}, size() - offset}; }

        Expects(count >= 0 && size() - offset >= count);
        return {KnownNotNull{data() + offset}, count};
    }
};

#if defined(GSL_USE_STATIC_CONSTEXPR_WORKAROUND)
template <class ElementType, std::ptrdiff_t Extent>
constexpr const typename span<ElementType, Extent>::index_type span<ElementType, Extent>::extent;
#endif

// [span.comparison], span comparison operators
template <class ElementType, std::ptrdiff_t FirstExtent, std::ptrdiff_t SecondExtent>
constexpr bool operator==(span<ElementType, FirstExtent> l, span<ElementType, SecondExtent> r)
{
    return std::equal(l.begin(), l.end(), r.begin(), r.end());
}

template <class ElementType, std::ptrdiff_t Extent>
constexpr bool operator!=(span<ElementType, Extent> l, span<ElementType, Extent> r)
{
    return !(l == r);
}

template <class ElementType, std::ptrdiff_t Extent>
constexpr bool operator<(span<ElementType, Extent> l, span<ElementType, Extent> r)
{
    return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
}

template <class ElementType, std::ptrdiff_t Extent>
constexpr bool operator<=(span<ElementType, Extent> l, span<ElementType, Extent> r)
{
    return !(l > r);
}

template <class ElementType, std::ptrdiff_t Extent>
constexpr bool operator>(span<ElementType, Extent> l, span<ElementType, Extent> r)
{
    return r < l;
}

template <class ElementType, std::ptrdiff_t Extent>
constexpr bool operator>=(span<ElementType, Extent> l, span<ElementType, Extent> r)
{
    return !(l < r);
}

namespace details
{
    // if we only supported compilers with good constexpr support then
    // this pair of classes could collapse down to a constexpr function

    // we should use a narrow_cast<> to go to std::size_t, but older compilers may not see it as
    // constexpr
    // and so will fail compilation of the template
    template <class ElementType, std::ptrdiff_t Extent>
    struct calculate_byte_size
        : std::integral_constant<std::ptrdiff_t,
                                 static_cast<std::ptrdiff_t>(sizeof(ElementType) *
                                                             static_cast<std::size_t>(Extent))>
    {
    };

    template <class ElementType>
    struct calculate_byte_size<ElementType, dynamic_extent>
        : std::integral_constant<std::ptrdiff_t, dynamic_extent>
    {
    };
} // namespace details

// [span.objectrep], views of object representation
template <class ElementType, std::ptrdiff_t Extent>
span<const byte, details::calculate_byte_size<ElementType, Extent>::value>
as_bytes(span<ElementType, Extent> s) noexcept
{
    GSL_SUPPRESS(type.1) // NO-FORMAT: attribute
    return {reinterpret_cast<const byte*>(s.data()), s.size_bytes()};
}

template <class ElementType, std::ptrdiff_t Extent,
          class = std::enable_if_t<!std::is_const<ElementType>::value>>
span<byte, details::calculate_byte_size<ElementType, Extent>::value>
as_writeable_bytes(span<ElementType, Extent> s) noexcept
{
    GSL_SUPPRESS(type.1) // NO-FORMAT: attribute
    return {reinterpret_cast<byte*>(s.data()), s.size_bytes()};
}

//
// make_span() - Utility functions for creating spans
//
template <class ElementType>
constexpr span<ElementType> make_span(ElementType* ptr,
                                      typename span<ElementType>::index_type count)
{
    return span<ElementType>(ptr, count);
}

template <class ElementType>
constexpr span<ElementType> make_span(ElementType* firstElem, ElementType* lastElem)
{
    return span<ElementType>(firstElem, lastElem);
}

template <class ElementType, std::size_t N>
constexpr span<ElementType, N> make_span(ElementType (&arr)[N]) noexcept
{
    return span<ElementType, N>(arr);
}

template <class Container>
constexpr span<typename Container::value_type> make_span(Container& cont)
{
    return span<typename Container::value_type>(cont);
}

template <class Container>
constexpr span<const typename Container::value_type> make_span(const Container& cont)
{
    return span<const typename Container::value_type>(cont);
}

template <class Ptr>
constexpr span<typename Ptr::element_type> make_span(Ptr& cont, std::ptrdiff_t count)
{
    return span<typename Ptr::element_type>(cont, count);
}

template <class Ptr>
constexpr span<typename Ptr::element_type> make_span(Ptr& cont)
{
    return span<typename Ptr::element_type>(cont);
}

// Specialization of gsl::at for span
template <class ElementType, std::ptrdiff_t Extent>
constexpr ElementType& at(span<ElementType, Extent> s, index i)
{
    // No bounds checking here because it is done in span::operator[] called below
    return s[i];
}

} // namespace gsl

#if defined(_MSC_VER) && !defined(__clang__)
#if _MSC_VER < 1910
#undef constexpr
#pragma pop_macro("constexpr")

#endif // _MSC_VER < 1910

#pragma warning(pop)
#endif // _MSC_VER

#if defined(__GNUC__) && __GNUC__ > 6
#pragma GCC diagnostic pop
#endif // __GNUC__ > 6

#endif // GSL_SPAN_H
