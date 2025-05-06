#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include <format>
#include <string>
#include <filesystem>

template <>
struct std::formatter<std::filesystem::path> : std::formatter<std::string> {
    auto format(const std::filesystem::path& p, std::format_context& ctx) const {
        return format_to(ctx.out(), "{}", p.string());
    }
};

#endif
