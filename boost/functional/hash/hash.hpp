
// Copyright 2005-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  Based on Peter Dimov's proposal
//  http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1756.pdf
//  issue 6.18. 

#if !defined(BOOST_FUNCTIONAL_HASH_HASH_HPP)
#define BOOST_FUNCTIONAL_HASH_HASH_HPP

#include <boost/functional/hash/hash_fwd.hpp>
#include <functional>
#include <boost/functional/hash/detail/hash_float.hpp>
#include <string>
#include <boost/limits.hpp>

#if defined(BOOST_HASH_NO_IMPLICIT_CASTS)
#include <boost/static_assert.hpp>
#endif

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#include <boost/type_traits/is_pointer.hpp>
#endif

#if BOOST_WORKAROUND(__GNUC__, < 3) \
    && !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
#define BOOST_HASH_CHAR_TRAITS string_char_traits
#else
#define BOOST_HASH_CHAR_TRAITS char_traits
#endif

namespace boost
{
#if defined(BOOST_HASH_NO_IMPLICIT_CASTS)

    // If you get a static assertion here, it's because hash_value
    // isn't declared for your type.
    template <typename T>
    std::size_t hash_value(T const&) {
        BOOST_STATIC_ASSERT((T*) 0 && false);
        return 0;
    }

#endif

    std::size_t hash_value(bool);
    std::size_t hash_value(char);
    std::size_t hash_value(unsigned char);
    std::size_t hash_value(signed char);
    std::size_t hash_value(short);
    std::size_t hash_value(unsigned short);
    std::size_t hash_value(int);
    std::size_t hash_value(unsigned int);
    std::size_t hash_value(long);
    std::size_t hash_value(unsigned long);

#if !defined(BOOST_NO_INTRINSIC_WCHAR_T)
    std::size_t hash_value(wchar_t);
#endif
    
#if !defined(BOOST_NO_LONG_LONG)
    std::size_t hash_value(boost::long_long_type);
    std::size_t hash_value(boost::ulong_long_type);
#endif

#if !BOOST_WORKAROUND(__DMC__, <= 0x848)
    template <class T> std::size_t hash_value(T* const&);
#else
    template <class T> std::size_t hash_value(T*);
#endif

#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
    template< class T, unsigned N >
    std::size_t hash_value(const T (&x)[N]);

    template< class T, unsigned N >
    std::size_t hash_value(T (&x)[N]);
#endif

    std::size_t hash_value(float v);
    std::size_t hash_value(double v);
    std::size_t hash_value(long double v);

    template <class Ch, class A>
    std::size_t hash_value(
        std::basic_string<Ch, std::BOOST_HASH_CHAR_TRAITS<Ch>, A> const&);

    // Implementation

    namespace hash_detail
    {
        template <class T>
        inline std::size_t hash_value_signed(T val)
        {
             const int size_t_bits = std::numeric_limits<std::size_t>::digits;
             // ceiling(std::numeric_limits<T>::digits / size_t_bits) - 1
             const int length = (std::numeric_limits<T>::digits - 1)
                 / size_t_bits;

             std::size_t seed = 0;
             T positive = val < 0 ? -1 - val : val;

             // Hopefully, this loop can be unrolled.
             for(unsigned int i = length * size_t_bits; i > 0; i -= size_t_bits)
             {
                 seed ^= (std::size_t) (positive >> i) + (seed<<6) + (seed>>2);
             }
             seed ^= (std::size_t) val + (seed<<6) + (seed>>2);

             return seed;
        }

        template <class T>
        inline std::size_t hash_value_unsigned(T val)
        {
             const int size_t_bits = std::numeric_limits<std::size_t>::digits;
             // ceiling(std::numeric_limits<T>::digits / size_t_bits) - 1
             const int length = (std::numeric_limits<T>::digits - 1)
                 / size_t_bits;

             std::size_t seed = 0;

             // Hopefully, this loop can be unrolled.
             for(unsigned int i = length * size_t_bits; i > 0; i -= size_t_bits)
             {
                 seed ^= (std::size_t) (val >> i) + (seed<<6) + (seed>>2);
             }
             seed ^= (std::size_t) val + (seed<<6) + (seed>>2);

             return seed;
        }
    }

    inline std::size_t hash_value(bool v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(char v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(unsigned char v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(signed char v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(short v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(unsigned short v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(int v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(unsigned int v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(long v)
    {
        return static_cast<std::size_t>(v);
    }

    inline std::size_t hash_value(unsigned long v)
    {
        return static_cast<std::size_t>(v);
    }

#if !defined(BOOST_NO_INTRINSIC_WCHAR_T)
    inline std::size_t hash_value(wchar_t v)
    {
        return static_cast<std::size_t>(v);
    }
#endif

#if !defined(BOOST_NO_LONG_LONG)
    inline std::size_t hash_value(boost::long_long_type v)
    {
        return hash_detail::hash_value_signed(v);
    }

    inline std::size_t hash_value(boost::ulong_long_type v)
    {
        return hash_detail::hash_value_unsigned(v);
    }
#endif

    // Implementation by Alberto Barbati and Dave Harris.
#if !BOOST_WORKAROUND(__DMC__, <= 0x848)
    template <class T> std::size_t hash_value(T* const& v)
#else
    template <class T> std::size_t hash_value(T* v)
#endif
    {
#if defined(__VMS) && __INITIAL_POINTER_SIZE == 64
    // for some reason ptrdiff_t on OpenVMS compiler with
    // 64 bit is not 64 bit !!!
        std::size_t x = static_cast<std::size_t>(
           reinterpret_cast<long long int>(v));
#else
        std::size_t x = static_cast<std::size_t>(
           reinterpret_cast<std::ptrdiff_t>(v));
#endif
        return x + (x >> 3);
    }

#if defined(BOOST_MSVC)
#pragma warning(push)
#if BOOST_MSVC <= 1400
#pragma warning(disable:4267) // 'argument' : conversion from 'size_t' to
                              // 'unsigned int', possible loss of data
                              // A misguided attempt to detect 64-bit
                              // incompatability.
#endif
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
    template <class T>
    inline void hash_combine(std::size_t& seed, T& v)
#else
    template <class T>
    inline void hash_combine(std::size_t& seed, T const& v)
#endif
    {
        boost::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

    template <class It>
    inline std::size_t hash_range(It first, It last)
    {
        std::size_t seed = 0;

        for(; first != last; ++first)
        {
            hash_combine(seed, *first);
        }

        return seed;
    }

    template <class It>
    inline void hash_range(std::size_t& seed, It first, It last)
    {
        for(; first != last; ++first)
        {
            hash_combine(seed, *first);
        }
    }

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
    template <class T>
    inline std::size_t hash_range(T* first, T* last)
    {
        std::size_t seed = 0;

        for(; first != last; ++first)
        {
            boost::hash<T> hasher;
            seed ^= hasher(*first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        return seed;
    }

    template <class T>
    inline void hash_range(std::size_t& seed, T* first, T* last)
    {
        for(; first != last; ++first)
        {
            boost::hash<T> hasher;
            seed ^= hasher(*first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
    }
#endif

#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
    template< class T, unsigned N >
    inline std::size_t hash_value(const T (&x)[N])
    {
        return hash_range(x, x + N);
    }

    template< class T, unsigned N >
    inline std::size_t hash_value(T (&x)[N])
    {
        return hash_range(x, x + N);
    }
#endif

    template <class Ch, class A>
    inline std::size_t hash_value(
        std::basic_string<Ch, std::BOOST_HASH_CHAR_TRAITS<Ch>, A> const& v)
    {
        return hash_range(v.begin(), v.end());
    }

    inline std::size_t hash_value(float v)
    {
        return boost::hash_detail::float_hash_value(v);
    }

    inline std::size_t hash_value(double v)
    {
        return boost::hash_detail::float_hash_value(v);
    }

    inline std::size_t hash_value(long double v)
    {
        return boost::hash_detail::float_hash_value(v);
    }

    //
    // boost::hash
    //
    
    // Define the specializations required by the standard. The general purpose
    // boost::hash is defined later in extensions.hpp if
    // BOOST_HASH_NO_EXTENSIONS is not defined.
    
    // BOOST_HASH_SPECIALIZE - define a specialization for a type which is
    // passed by copy.
    //
    // BOOST_HASH_SPECIALIZE_REF - define a specialization for a type which is
    // passed by copy.
    //
    // These are undefined later.

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1300)
#define BOOST_HASH_SPECIALIZE(type) \
    template <> struct hash<type> \
         : public std::unary_function<type, std::size_t> \
    { \
        std::size_t operator()(type v) const \
        { \
            return boost::hash_value(v); \
        } \
    };

#define BOOST_HASH_SPECIALIZE_REF(type) \
    template <> struct hash<type> \
         : public std::unary_function<type, std::size_t> \
    { \
        std::size_t operator()(type const& v) const \
        { \
            return boost::hash_value(v); \
        } \
    };
#else
#define BOOST_HASH_SPECIALIZE(type) \
    template <> struct hash<type> \
         : public std::unary_function<type, std::size_t> \
    { \
        std::size_t operator()(type v) const \
        { \
            return boost::hash_value(v); \
        } \
    }; \
    \
    template <> struct hash<const type> \
         : public std::unary_function<const type, std::size_t> \
    { \
        std::size_t operator()(const type v) const \
        { \
            return boost::hash_value(v); \
        } \
    };

#define BOOST_HASH_SPECIALIZE_REF(type) \
    template <> struct hash<type> \
         : public std::unary_function<type, std::size_t> \
    { \
        std::size_t operator()(type const& v) const \
        { \
            return boost::hash_value(v); \
        } \
    }; \
    \
    template <> struct hash<const type> \
         : public std::unary_function<const type, std::size_t> \
    { \
        std::size_t operator()(type const& v) const \
        { \
            return boost::hash_value(v); \
        } \
    };
#endif

    BOOST_HASH_SPECIALIZE(bool)
    BOOST_HASH_SPECIALIZE(char)
    BOOST_HASH_SPECIALIZE(signed char)
    BOOST_HASH_SPECIALIZE(unsigned char)
#if !defined(BOOST_NO_INTRINSIC_WCHAR_T)
    BOOST_HASH_SPECIALIZE(wchar_t)
#endif
    BOOST_HASH_SPECIALIZE(short)
    BOOST_HASH_SPECIALIZE(unsigned short)
    BOOST_HASH_SPECIALIZE(int)
    BOOST_HASH_SPECIALIZE(unsigned int)
    BOOST_HASH_SPECIALIZE(long)
    BOOST_HASH_SPECIALIZE(unsigned long)

    BOOST_HASH_SPECIALIZE(float)
    BOOST_HASH_SPECIALIZE(double)
    BOOST_HASH_SPECIALIZE(long double)

    BOOST_HASH_SPECIALIZE_REF(std::string)
#if !defined(BOOST_NO_STD_WSTRING)
    BOOST_HASH_SPECIALIZE_REF(std::wstring)
#endif

#if !defined(BOOST_NO_LONG_LONG)
    BOOST_HASH_SPECIALIZE(boost::long_long_type)
    BOOST_HASH_SPECIALIZE(boost::ulong_long_type)
#endif

#undef BOOST_HASH_SPECIALIZE
#undef BOOST_HASH_SPECIALIZE_REF

// Specializing boost::hash for pointers.

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

    template <class T>
    struct hash<T*>
        : public std::unary_function<T*, std::size_t>
    {
        std::size_t operator()(T* v) const
        {
#if !BOOST_WORKAROUND(__SUNPRO_CC, <= 0x590)
            return boost::hash_value(v);
#else
            std::size_t x = static_cast<std::size_t>(
                reinterpret_cast<std::ptrdiff_t>(v));

            return x + (x >> 3);
#endif
        }
    };

#else

    // For compilers without partial specialization, we define a
    // boost::hash for all remaining types. But hash_impl is only defined
    // for pointers in 'extensions.hpp' - so when BOOST_HASH_NO_EXTENSIONS
    // is defined there will still be a compile error for types not supported
    // in the standard.

    namespace hash_detail
    {
        template <bool IsPointer>
        struct hash_impl;

        template <>
        struct hash_impl<true>
        {
            template <class T>
            struct inner
                : public std::unary_function<T, std::size_t>
            {
                std::size_t operator()(T val) const
                {
#if !BOOST_WORKAROUND(__SUNPRO_CC, <= 590)
                    return boost::hash_value(val);
#else
                    std::size_t x = static_cast<std::size_t>(
                        reinterpret_cast<std::ptrdiff_t>(val));

                    return x + (x >> 3);
#endif
                }
            };
        };
    }

    template <class T> struct hash
        : public boost::hash_detail::hash_impl<boost::is_pointer<T>::value>
            ::BOOST_NESTED_TEMPLATE inner<T>
    {
    };

#endif
}

#undef BOOST_HASH_CHAR_TRAITS

#endif // BOOST_FUNCTIONAL_HASH_HASH_HPP

// Include this outside of the include guards in case the file is included
// twice - once with BOOST_HASH_NO_EXTENSIONS defined, and then with it
// undefined.

#if !defined(BOOST_HASH_NO_EXTENSIONS) \
    && !defined(BOOST_FUNCTIONAL_HASH_EXTENSIONS_HPP)
#include <boost/functional/hash/extensions.hpp>
#endif
