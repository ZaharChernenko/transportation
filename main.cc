#include "auth/auth.h"
#include "business_logic/autocomplete.h"
#include "business_logic/business_handler.h"
#include "models/User.h"
#include <cstdint>
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <string>

using namespace drogon;
using Callback = std::function<void(const HttpResponsePtr&)>;

int main() {
    setlocale(LC_ALL, "Russian");
    app().registerHandler("/", &rootHandler);
    app().registerHandler("/register", &registerUser, {drogon::HttpMethod::Post});
    app().registerHandler("/login", &loginUser, {drogon::HttpMethod::Post});
    app().registerHandler("/index.html", &indexHandler, {drogon::HttpMethod::Get});
    app().registerHandler("/login.html", &loginHandler, {drogon::HttpMethod::Get});
    app().registerHandler("/register.html", &registerHandler, {drogon::HttpMethod::Get});
    app().registerHandler("/autocomplete/departure/{city_name}", &autocompleteIndexInputs, {drogon::HttpMethod::Get});
    app().registerHandler("/autocomplete/arrival/{city_name}", &autocompleteIndexInputs, {drogon::HttpMethod::Get});
    app().registerHandler("/count?departure={departure}&arrival={arrival}&car={car}&insurance={insurance}",
                          &countHandler, {drogon::HttpMethod::Get});
    app().registerHandler("/delete/{id}", &deleteHandler, {drogon::HttpMethod::Delete});
    app().registerHandler("/add/{order_id}", &addHandler, {drogon::HttpMethod::Get});
    app().registerHandler("/profile.html", &getOrdersHandler, {drogon::HttpMethod::Get});
    app().registerHandler("/logout", &logoutHandler, {drogon::HttpMethod::Get});

    drogon::app().loadConfigFile("../config.json");
    app().run();
    return EXIT_SUCCESS;
}
