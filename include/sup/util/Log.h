#pragma once

// SUP logging shim.
//
// Phase 1 of the modernization (see MODERNIZATION_PLAN.md): legacy printf-style logging
// routed to spdlog through CommonLibF4's logging backend (REX::LOG uses the spdlog default
// logger, so everything lands in the same log file configured by F4SE::Init).
//
// The ~1900 legacy _MESSAGE / _DMESSAGE / _ERROR call sites keep their printf-style format
// strings for now. As each module is migrated (Phase 5) the shim macros are retired in favor
// of direct REX::INFO / REX::DEBUG / REX::WARN / REX::CRITICAL (std::format) calls.
//
// The old `bDebugMode` gating is gone: debug messages are filtered by the spdlog logger level
// instead (see sup::log::set_debug_enabled, wired to the INI setting in the plugin entry).

#include <spdlog/spdlog.h>

#include <cstdarg>
#include <cstdio>
#include <source_location>
#include <string>

namespace sup::log
{
	// printf-style format + va_list -> std::string (vsnprintf)
	[[nodiscard]] inline std::string vformat(const char* a_fmt, va_list a_args)
	{
		va_list copy;
		va_copy(copy, a_args);
		const int size = std::vsnprintf(nullptr, 0, a_fmt, copy);
		va_end(copy);

		if (size < 0) {
			return {};
		}

		std::string result(static_cast<std::size_t>(size), '\0');
		std::vsnprintf(result.data(), result.size() + 1, a_fmt, a_args);
		return result;
	}

	inline void log(
		const spdlog::level::level_enum a_level,
		const std::source_location a_loc,
		const char* a_fmt,
		...)
	{
		auto* logger = spdlog::default_logger_raw();
		if (!logger || !logger->should_log(a_level)) {
			return;
		}

		va_list args;
		va_start(args, a_fmt);
		const std::string msg = vformat(a_fmt, args);
		va_end(args);

		// pass the message as data ("{}") so braces in user data are never parsed as fmt
		logger->log(
			spdlog::source_loc{ a_loc.file_name(), static_cast<std::int32_t>(a_loc.line()), a_loc.function_name() },
			a_level,
			"{}",
			msg);
	}

	// Mirrors the old INI `bDebugMode` toggle: false = Info and above, true = Debug and above.
	// When debug is enabled we also flush on debug so _DMESSAGE lines survive a hard crash
	// (CommonLibF4's F4SE::Init sets flush_on(info) by default, which would lose them).
	inline void set_debug_enabled(const bool a_enabled) noexcept
	{
		const auto level = a_enabled ? spdlog::level::debug : spdlog::level::info;
		spdlog::set_level(level);
		spdlog::flush_on(level);
	}
}

// Legacy macros — printf-style, level-filtered by spdlog.
#define _MESSAGE(...) ::sup::log::log(spdlog::level::info, std::source_location::current(), __VA_ARGS__)
#define _DMESSAGE(...) ::sup::log::log(spdlog::level::debug, std::source_location::current(), __VA_ARGS__)
#define _ERROR(...) ::sup::log::log(spdlog::level::err, std::source_location::current(), __VA_ARGS__)
