//this file is generated by program(drogon_ctl) automatically,don't modify it!
#include "Profile.h"
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

#include "../models/Order.h"
#include "../algorithms/cyrillic.h"

using namespace drogon;
std::string Profile::genText(const DrTemplateData& Profile_view_data)
{
	drogon::OStringStream Profile_tmp_stream;
	std::string layoutName{""};
    std::vector<Order> orders=Profile_view_data.get<std::vector<Order>>("orders");
	Profile_tmp_stream << "<head>\n";
	Profile_tmp_stream << "    <meta charset=\"UTF-8\">\n";
	Profile_tmp_stream << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	Profile_tmp_stream << "    <title>Профиль</title>\n";
	Profile_tmp_stream << "    <link rel=\"stylesheet\" href=\"styles/style.css\">\n";
	Profile_tmp_stream << "    <link rel=\"stylesheet\" href=\"styles/profile_style.css\">\n";
	Profile_tmp_stream << "</head>\n";
	Profile_tmp_stream << "<body>\n";
	Profile_tmp_stream << "    <header class=\"index_header\">\n";
	Profile_tmp_stream << "        <a class=\"index_header_a\" href=\"index.html\">\n";
	Profile_tmp_stream << "            <img class=\"index_header_logo\" src=\"img/logo.svg\">\n";
	Profile_tmp_stream << "        </a>\n";
	Profile_tmp_stream << "        <h1>ПеревозОчка</h1>\n";
	Profile_tmp_stream << "        <button class=\"index_header_profile_btn\"><a class=\"index_header_profile_btn_a\" href=\"logout\">Выйти</a></button>\n";
	Profile_tmp_stream << "    </header>\n";
Profile_tmp_stream<<"\n";
	Profile_tmp_stream << "    <div class=\"container\">\n";
	Profile_tmp_stream << "        <h2 class=\"container_header\">Ваши заказы:</h2>\n";
	Profile_tmp_stream << "        <table class=\"orders_table\">\n";
	Profile_tmp_stream << "            <tr>\n";
	Profile_tmp_stream << "                <th>Пункт отправления</th>\n";
	Profile_tmp_stream << "                <th>Пункт прибытия</th>\n";
	Profile_tmp_stream << "                <th>Дистанция</th>\n";
	Profile_tmp_stream << "                <th>Машина</th>\n";
	Profile_tmp_stream << "                <th>Стоимость заказа</th>\n";
	Profile_tmp_stream << "                <th>Дата создания заказа</th>\n";
	Profile_tmp_stream << "            </tr>\n";
	Profile_tmp_stream << "            ";
 for (const Order& order : orders) { 
	Profile_tmp_stream << "            <tr>\n";
	Profile_tmp_stream << "                <td>";
Profile_tmp_stream<<capitalize(order.getDeparture());
	Profile_tmp_stream << "</td>\n";
	Profile_tmp_stream << "                <td>";
Profile_tmp_stream<<capitalize(order.getArrival());
	Profile_tmp_stream << "</td>\n";
	Profile_tmp_stream << "                <td>";
Profile_tmp_stream<<order.getDistance();
	Profile_tmp_stream << "</td>\n";
	Profile_tmp_stream << "                <td>";
Profile_tmp_stream<<order.getPrice();
	Profile_tmp_stream << "</td>\n";
	Profile_tmp_stream << "                <td>";
Profile_tmp_stream<<order.getCarName();
	Profile_tmp_stream << "</td>\n";
	Profile_tmp_stream << "                <td>";
Profile_tmp_stream<<order.getOrderData();
	Profile_tmp_stream << "</td>\n";
	Profile_tmp_stream << "            </tr>\n";
	Profile_tmp_stream << "            ";
 } 
	Profile_tmp_stream << "        </table>\n";
	Profile_tmp_stream << "    </div>\n";
	Profile_tmp_stream << "</body>\n";
	Profile_tmp_stream << "</html>\n";
if(layoutName.empty())
{
std::string ret{std::move(Profile_tmp_stream.str())};
return ret;
}else
{
auto templ = DrTemplateBase::newTemplate(layoutName);
if(!templ) return "";
HttpViewData data = Profile_view_data;
auto str = std::move(Profile_tmp_stream.str());
if(!str.empty() && str[str.length()-1] == '\n') str.resize(str.length()-1);
data[""] = std::move(str);
return templ->genText(data);
}
}