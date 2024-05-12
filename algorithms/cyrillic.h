#include <codecvt>
#include <locale>
#include <string>

std::wstring string_to_wstring(const std::string& str);
std::string wstring_to_string(const std::wstring& wstr);
std::string toRussianLowercase(const std::string& str);
std::string capitalize(const std::string& str);