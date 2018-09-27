//
//  tuple_print.h
//  ChrisTest
//
//  Created by Harward, Christopher on 8/31/18.
//  Copyright © 2018 Harward, Christopher. All rights reserved.
//

#pragma once
#include <tuple>
#include <ostream>
#include <istream>

template<size_t I, size_t N, typename T>
struct tuple_element
{
    template<typename U>
    tuple_element(U&& u) : m_t(std::forward<U>(u)) {}

    T m_t;
};

template<typename T, size_t I, size_t N>
constexpr std::ostream& operator<<(std::ostream &os, tuple_element<I, N, T> const &t)
{
    if constexpr (I + 1 == N)
    {
        return os << t.m_t;
    }
    else
    {
        return os << t.m_t << ' ';
    }
}

template<typename T, size_t... I>
constexpr std::ostream& tuple_output_op(std::ostream &os, T const &t, std::index_sequence<I...>)
{
    (os << ... << tuple_element<I, std::tuple_size<T>::value, typename std::tuple_element<I, T>::type>(std::get<I>(t)));
    return os;
}

template<typename... T>
constexpr std::ostream& operator<<(std::ostream &os, std::tuple<T...> const &t)
{
    os << '[';
    return tuple_output_op(os, t, std::make_index_sequence<std::tuple_size<std::tuple<T...>>::value>()) << ']';
}

template<typename T, size_t I, size_t N>
constexpr std::istream& read_tuple_element(std::istream &is, T &t)
{
    if constexpr (I == N)
    {
        return is;
    }
    
    is >> std::get<I>(t);
    
    if constexpr (I + 1 != N)
    {
        if (is.peek() != ' ')
        {
            return is;
        }
        is.get();
    }
    return is;
}

template<typename T, size_t... I>
constexpr std::istream& read_tuple(std::istream &is, T &t, std::index_sequence<I...>)
{
    (read_tuple_element<T, I, std::tuple_size<T>::value>(is, t), ...);
    return is;
}

template<typename... T>
constexpr std::istream& operator>>(std::istream &is, std::tuple<T...> &t)
{
    if (is.peek() != '[')
    {
        return is;
    }
    is.get();
    
    read_tuple(is, t, std::make_index_sequence<std::tuple_size<std::tuple<T...>>::value>());
    
    if (is.peek() != ']')
    {
        return is;
    }
    is.get();
    return is;
}
