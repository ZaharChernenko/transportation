#include "autocomplete.h"

void autocompleteIndexInputs(const drogon::HttpRequestPtr& req, Callback&& callback, const std::string& city_name) {
    drogon::orm::DbClientPtr client = drogon::app().getDbClient("cities");
    std::string city_name_lower {toRussianLowercase(city_name)};
    client->execSqlAsync(
        "SELECT city FROM cities WHERE city LIKE '" + city_name_lower + "%';",
        [callback](const drogon::orm::Result& result) { // & causes seg fault
            Json::Value cities_array {Json::arrayValue};
            for (const drogon::orm::Row& row : result)
                cities_array.append(capitalize(row["city"].as<std::string>()));

            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(cities_array)};
            callback(response);
        },

        [callback](const drogon::orm::DrogonDbException& e) {
            LOG_ERROR << e.base().what() << '\n';
            Json::Value json;
            json["message"] = "Неполадки на сервере";
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
            response->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
            callback(response);
        });
}