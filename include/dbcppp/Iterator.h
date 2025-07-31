#pragma once

#include <iterator>

namespace dbcppp
{
    template <class T, class P, typename F>
    class Iterator final
    {
    public:
        using self_t            = Iterator<T, P, F>;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = value_type*;
        using reference         = value_type&;

        constexpr Iterator(P parent, F get, difference_type i) noexcept
            : _parent(parent)
            , _get(std::move(get))
            , _i(i)
        {}
        inline reference operator*() const
        {
            return (_parent->*_get)(static_cast<size_t>(_i));
        }
        inline pointer operator->() const
        {
            return &(_parent->*_get)(static_cast<size_t>(_i));
        }
        inline reference operator[](difference_type o) const
        {
            return *(*this + o);
        }
        constexpr self_t& operator++() noexcept
        {
            ++_i;
            return *this;
        }
        constexpr self_t& operator--() noexcept
        {
            --_i;
            return *this;
        }
        constexpr self_t operator++(int) noexcept
        {
            self_t old = *this;
            ++_i;
            return std::move(old);
        }
        constexpr self_t operator--(int) noexcept
        {
            self_t old = *this;
            --_i;
            return std::move(old);
        }
        constexpr self_t operator+(difference_type o) const noexcept
        {
            return {_parent, _get, _i + o};
        }
        constexpr self_t operator-(difference_type o) const noexcept
        {
            return {_parent, _get, _i - o};
        }
        constexpr difference_type operator-(const self_t& rhs) const noexcept
        {
            return _i - rhs._i;
        }
        constexpr self_t& operator+=(difference_type o) noexcept
        {
            _i += o;
            return *this;
        }
        constexpr self_t& operator-=(difference_type o) noexcept
        {
            _i -= o;
            return *this;
        }

        constexpr bool operator==(const self_t& rhs) const noexcept
        {
            return _i == rhs._i;
        }
        constexpr bool operator!=(const self_t& rhs) const noexcept
        {
            return !(*this == rhs);
        }
        constexpr bool operator<(const self_t& rhs) const noexcept
        {
            return _i < rhs._i;
        }
        constexpr bool operator>(const self_t& rhs) const noexcept
        {
            return _i > rhs._i;
        }
        constexpr bool operator<=(const self_t& rhs) const noexcept
        {
            return _i <= rhs._i;
        }
        constexpr bool operator>=(const self_t& rhs) const noexcept
        {
            return _i >= rhs._i;
        }

    private:
        P _parent;
        F _get;
        difference_type _i;
    };
    template<class T, class P, typename F>
    class Iterable final
    {
    public:
        using value_type = const T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = Iterator<T, P, F>;
        using const_iterator = Iterator<const T, P, F>;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;

        constexpr Iterable(P parent, F get, size_t size) noexcept
            : _parent(std::move(parent))
            , _get(std::move(get))
            , _size(std::move(size))
        {}
        inline reference operator[](std::size_t o) const
        {
            return (_parent->*_get)(o);
        }
        constexpr iterator begin() noexcept
        {
            return iterator(_parent, _get, 0);
        }
        constexpr iterator end() noexcept
        {
            return iterator(_parent, _get, _size);
        }
        constexpr const_iterator cbegin() const noexcept
        {
            return const_iterator(_parent, _get, 0);
        }
        constexpr const_iterator cend() const noexcept
        {
            return const_iterator(_parent, _get, static_cast<difference_type>(_size));
        }
        constexpr const_iterator begin() const noexcept
        {
            return cbegin();
        }
        constexpr const_iterator end() const noexcept
        {
            return cend();
        }
        constexpr size_type size() const noexcept
        {
            return _size;
        }
        constexpr bool empty() const noexcept
        {
            return _size == 0;
        }

    private:
        P _parent;
        F _get;
        size_t _size;
    };
}
#define DBCPPP_MAKE_ITERABLE(ClassName, Name, Type)                                                       \
    inline Iterable<const Type, const ClassName*, decltype(&ClassName::Name##_Get)> Name() const noexcept \
    {                                                                                                     \
        return {this, &ClassName::Name##_Get, Name##_Size()};                                             \
    }
