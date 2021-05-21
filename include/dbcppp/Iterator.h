
#pragma once

#include <functional>

namespace dbcppp
{
    template <class T>
    class Iterator
    {
    public:
        using self_t            = Iterator<T>;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = const value_type*;
        using reference         = const value_type&;
        using get_t             = std::function<reference (std::size_t)>;

        Iterator(get_t get, std::size_t i)
            : _get(get)
            , _i(i)
        {}
        reference operator*() const
        {
            return _get(_i);
        }
        pointer operator->() const
        {
            return &_get(_i);
        }
        self_t& operator++()
        {
            _i++;
            return *this;
        }
        self_t operator+(std::size_t o)
        {
            return {_get, _i + o};
        }
        self_t operator-(std::size_t o)
        {
            return {_get, _i + o};
        }
        difference_type operator-(const self_t& rhs)
        {
            return _i - rhs._i;
        }
        self_t& operator+=(std::size_t o)
        {
            _i += o;
            return *this;
        }
        self_t& operator-=(std::size_t o)
        {
            _i -= o;
            return *this;
        }

        bool operator==(const self_t& rhs) const
        {
            return _i == rhs._i;
        }
        bool operator!=(const self_t& rhs) const
        {
            return !(*this == rhs);
        }

    private:
        get_t _get;
        std::size_t _i;
    };
    template <class Iterator>
    class Iterable
    {
    public:
        Iterable(Iterator begin, Iterator end)
            : _begin(begin)
            , _end(end)
        {}
        Iterator& begin()
        {
            return _begin;
        }
        Iterator& end()
        {
            return _end;
        }

    private:
        Iterator _begin;
        Iterator _end;
    };
}
#define DBCPPP_MAKE_ITERABLE(ClassName, Name, Type)                                 \
    auto Name() const                                                               \
    {                                                                               \
        auto get = std::bind(&ClassName::Name##_Get, this, std::placeholders::_1);  \
        Iterator<Type> begin(get, 0);                                               \
        Iterator<Type> end(get, Name##_Size());                                     \
        return Iterable(begin, end);                                                \
    }
