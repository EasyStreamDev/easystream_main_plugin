/*
** EPITECH PROJECT, 2022
** Common usings to shorten code
** File description:
** common_using
*/

#ifndef COMMON_USING_HPP_
#define COMMON_USING_HPP_

// #include <boost/shared_ptr.hpp>
// #include <boost/make_shared.hpp>
// #include <boost/make_unique.hpp>
#include <iostream>
#include <memory>

// SHARED PTR

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Shared<T> CreateShared(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// UNIQUE PTR

template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Unique<T> CreateUnique(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif /* !COMMON_USING_HPP_ */