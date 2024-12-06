#pragma once
#include <fmt/format.h>
#include <stdexcept>

template <typename T>
concept exception_type = std::is_base_of_v<std::exception, T>;

template <exception_type Exception> struct FormattedException : Exception {
    template <typename... T>
    explicit FormattedException(fmt::format_string<T...> fmt, T&&... args)
        : Exception (fmt::vformat(fmt, fmt::make_format_args(std::forward<T>(args)...))) {}
};

struct OSHookError final : FormattedException<std::runtime_error> {
    using FormattedException::FormattedException;
};


struct ContextInitializationError final : FormattedException<std::runtime_error> {
    using FormattedException::FormattedException;
};