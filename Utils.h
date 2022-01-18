#pragma once
#define DELETE_ALL_COPY_CONSTRUTION(T) T(const T&) = delete; T(const T&&) = delete; T &operator = (const T &) = delete;