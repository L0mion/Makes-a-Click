#pragma once

#include <string>

namespace Util {
	class UtilString {
	public:
		static std::wstring Std2W(std::wstring &ws, const std::string &s);
		static std::string W2Std(const std::wstring &ws, std::string &s);

		UtilString()	{ /*Do nothing.*/ }
		~UtilString()	{ /*Do nothing.*/ }
	protected:
	private:
	};
}