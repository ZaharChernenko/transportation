//this file is generated by program(drogon_ctl) automatically,don't modify it!
#include "Login.h"
#include <drogon/utils/OStringStream.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <list>
#include <deque>
#include <queue>
using namespace drogon;
std::string Login::genText(const DrTemplateData& Login_view_data)
{
	drogon::OStringStream Login_tmp_stream;
	std::string layoutName{""};
	Login_tmp_stream << "<!DOCTYPE html>\n";
	Login_tmp_stream << "<html lang=\"ru\">\n";
	Login_tmp_stream << "<head>\n";
	Login_tmp_stream << "    <meta charset=\"UTF-8\">\n";
	Login_tmp_stream << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	Login_tmp_stream << "    <title>Вход</title>\n";
	Login_tmp_stream << "    <link rel=\"stylesheet\" href=\"styles/auth_style.css\">\n";
	Login_tmp_stream << "    <link rel=\"stylesheet\" href=\"styles/style.css\">\n";
	Login_tmp_stream << "    <script defer src=\"scripts/login.js\"></script>\n";
	Login_tmp_stream << "</head>\n";
	Login_tmp_stream << "<body>\n";
	Login_tmp_stream << "    <div class=\"container\">\n";
	Login_tmp_stream << "        <h1>Вход</h1>\n";
	Login_tmp_stream << "        <form action=\"#\" method=\"post\" id=\"loginForm\">\n";
	Login_tmp_stream << "            <label for=\"email\">Email:</label>\n";
	Login_tmp_stream << "            <input type=\"email\" id=\"email\" name=\"email\" required>\n";
Login_tmp_stream<<"\n";
	Login_tmp_stream << "            <label for=\"password\">Пароль:</label>\n";
	Login_tmp_stream << "            <input type=\"password\" id=\"password\" name=\"password\" required>\n";
Login_tmp_stream<<"\n";
	Login_tmp_stream << "            <button type=\"submit\">Войти</button>\n";
	Login_tmp_stream << "        </form>\n";
	Login_tmp_stream << "        <p>Нет аккаунта? <a href=\"register.html\">Создать аккаунт</a></p>\n";
	Login_tmp_stream << "    </div>\n";
	Login_tmp_stream << "</body>\n";
	Login_tmp_stream << "</html>\n";
if(layoutName.empty())
{
std::string ret{std::move(Login_tmp_stream.str())};
return ret;
}else
{
auto templ = DrTemplateBase::newTemplate(layoutName);
if(!templ) return "";
HttpViewData data = Login_view_data;
auto str = std::move(Login_tmp_stream.str());
if(!str.empty() && str[str.length()-1] == '\n') str.resize(str.length()-1);
data[""] = std::move(str);
return templ->genText(data);
}
}
