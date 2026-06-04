module;
#include <windows.h>
#include <print>

export module sys.error_sentinel;

import sys.error_info;
import dast.string;

export enum class error_type {
	open_file
};

export namespace sys
{
	template <error_type>
	struct error_sentinel;
}

template <error_type ErrorType>
struct sys::error_sentinel {
	constexpr error_sentinel() noexcept = default;
	static constexpr const char* message_info[] = {
		"Can't open file, Reason: "
	};
	constexpr ~error_sentinel() {
		if (auto error = GetLastError(); error) {
			auto mess  = message_info [
				static_cast<std::size_t>(ErrorType)
			];
			auto info  = dast::cstring(mess);
			info	  += sys::detailed_error<char>(error);
			std::println("{}", info.const_string());
		}
	}
};