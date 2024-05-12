#include "cyrillic.h"

std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wstring_to_string(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

std::string toRussianLowercase(const std::string& str) {
    std::wstring wstr {string_to_wstring(str)};
    std::locale ru {"ru_RU.UTF-8"};
    std::for_each(wstr.begin(), wstr.end(), [&ru](wchar_t& c) { c = std::tolower(c, ru); });
    return wstring_to_string(wstr);
}

std::string capitalize(const std::string& str) {
    std::wstring wstr {string_to_wstring(str)};
    std::locale ru {"ru_RU.UTF-8"};
    std::for_each(wstr.begin(), wstr.end(), [&ru](wchar_t& c) { c = std::tolower(c, ru); });
    if (!wstr.empty()) {
        wstr[0] = std::toupper(wstr[0], ru);
        for (std::size_t i {1}; i != wstr.size(); ++i) {
            if (!std::isalpha(wstr[i - 1], ru))
                wstr[i] = std::toupper(wstr[i], ru);
        }
    }

    return wstring_to_string(wstr);
}