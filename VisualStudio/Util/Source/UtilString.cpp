#include <sstream>

#include "UtilString.h"

namespace Util {
	std::wstring UtilString::Std2W(std::wstring &ws, const std::string &s)
	{
		std::wstring wsTmp(s.begin(), s.end());
		ws = wsTmp;
		return ws;
	}

	std::string UtilString::W2Std(const std::wstring &ws, std::string &s)
	{
		std::string sTmp(ws.begin(), ws.end());
		s = sTmp;
		return s;
	}

	std::string UtilString::Int2Std( const int n ) {
		std::stringstream ss;
		ss << n;
		return ss.str();
	}

	std::string	UtilString::F2Std( const float f ) {
		std::stringstream ss;
		ss << f;
		return ss.str();
	}
}