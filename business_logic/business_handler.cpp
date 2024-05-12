#include "business_handler.h"

void countHandler(const drogon::HttpRequestPtr& request, Callback&& callback, const std::string& departure,
                  const std::string& arrival, const std::string& car, const std::string& insurance) {

    Json::Value json;
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    if (!user_id) {
        json["message"] = "Произведен выход из акканута";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    drogon::orm::DbClientPtr client {drogon::app().getDbClient("cities")};
    std::string departure_lowercase {toRussianLowercase(departure)};
    std::string arrival_lowercase {toRussianLowercase(arrival)};

    if (departure_lowercase == arrival_lowercase) {
        json["message"] = "Город отправления совпадает с городом прибытия";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    try {
#ifdef BUSINESS_HANDLER_DEBUG
        LOG_INFO << "Город отправки: " << departure << '\n';
#endif
        std::future<drogon::orm::Result> future_check_arrival {
            client->execSqlAsyncFuture("SELECT EXISTS(SELECT 1 FROM cities WHERE city = $1);", departure_lowercase)};
        bool is_arrival_correct {future_check_arrival.get().front()[0].as<bool>()};

        if (!is_arrival_correct) {
            json["message"] = "Невалидный город отправки";
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
#ifdef BUSINESS_HANDLER_DEBUG
        LOG_INFO << "Город прибытия: " << arrival << '\n';
#endif

        std::future<drogon::orm::Result> future_check_distance {client->execSqlAsyncFuture(
            "SELECT distance FROM '" + departure_lowercase + "' WHERE city = $1", arrival_lowercase)};

        drogon::orm::Result distance_result {future_check_distance.get()};
        if (distance_result.empty()) {
            json["message"] = "Невалидный город прибытия";
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }

        u_int32_t distance {distance_result.front()[0].as<u_int32_t>()};
#ifdef BUSINESS_HANDLER_DEBUG
        LOG_INFO << "Расстояние между городами: " << distance << '\n';
        LOG_INFO << "Машина: " << car << '\n';
#endif
        auto car_ptr {cars_unordered_map.find(car)};
        if (car_ptr == cars_unordered_map.end()) {
            json["message"] = "Неверно указана машина";
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }

#ifdef BUSINESS_HANDLER_DEBUG
        LOG_INFO << "Страховка: " << insurance << '\n';
#endif
        bool is_insurance {false};
        if (insurance == "true")
            is_insurance = true;
        else if (insurance != "false") {
            json["message"] = "Нет данных о страховке";
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
            response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        // all data is valid
        int price {static_cast<int>(countPrice(5000, (car_ptr->second).getCostCoef(), distance, is_insurance))};
        std::string car_name {(car_ptr->second).getCarName()};
        Order potential_order {arrival_lowercase, departure_lowercase, std::to_string(distance), std::to_string(price),
                               car_name};
        u_int32_t potential_order_id;
        request->session()->modify<std::map<u_int32_t, Order, std::greater<u_int32_t>>>(
            "ordersMap", [&arrival_lowercase, &departure_lowercase, &distance, &price, &car_name, &potential_order,
                          &potential_order_id](std::map<u_int32_t, Order, std::greater<u_int32_t>>& orders_map) {
                if (orders_map.empty())
                    potential_order_id = 0;
                else
                    potential_order_id = orders_map.begin()->first + 1;
                orders_map[potential_order_id] = potential_order;
#ifdef BUSINESS_HANDLER_DEBUG
                for (const auto pair : orders_map)
                    pair.second.print();
#endif
            });
        json["departure"] = capitalize(departure_lowercase);
        json["arrival"] = capitalize(arrival_lowercase);
        json["distance"] = distance;
        json["price"] = price;
        json["car_name"] = car_name;
        json["potential_order_id"] = potential_order_id;
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k200OK);
        callback(response);
    }

    catch (const drogon::orm::DrogonDbException& e) {
        LOG_ERROR << e.base().what() << '\n';
        json["message"] = "Проблема на стороне сервера, невозможно посчитать стоимость";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
        callback(response);
        return;
    }
}

void deleteHandler(const drogon::HttpRequestPtr& request, Callback&& callback, const std::string& id) {
    Json::Value json;
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    if (!user_id) {
        json["message"] = "Произведен выход из акканута";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    request->session()->modify<std::map<u_int32_t, Order, std::greater<u_int32_t>>>(
        "ordersMap", [&id, callback, &json](std::map<u_int32_t, Order, std::greater<u_int32_t>>& orders_map) {
            u_int32_t int_id = std::stoul(id);
            if (!orders_map.contains(int_id)) {
                json["message"] = "Невалидный id";
                drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
                response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
                callback(response);
                return;
            }
            orders_map.erase(int_id);
            json["message"] = "id успешно удален";
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
            response->setStatusCode(drogon::HttpStatusCode::k200OK);
            callback(response);
#ifdef BUSINESS_HANDLER_DEBUG
            for (const auto pair : orders_map)
                pair.second.print();
#endif
            return;
        });
}

void addHandler(const drogon::HttpRequestPtr& request, Callback&& callback, const std::string& order_id) {
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    if (!user_id) {
        Json::Value json;
        json["message"] = "Произведен выход из акканута";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    request->session()->modify<std::map<u_int32_t, Order, std::greater<u_int32_t>>>(
        "ordersMap", [&order_id, callback, &user_id](std::map<u_int32_t, Order, std::greater<u_int32_t>>& orders_map) {
            Json::Value json;
            u_int32_t int_id = std::stoul(order_id);
            if (!orders_map.contains(int_id)) {
                json["message"] = "Невалидный id";
                drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
                response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
                callback(response);
                return;
            }
            drogon::orm::DbClientPtr client {drogon::app().getDbClient("data")};
            Order order {orders_map[int_id]};
            client->execSqlAsync(
                order.getInsertQuery(std::to_string(user_id)),
                [callback, &orders_map, int_id](const drogon::orm::Result& result) {
                    Json::Value json;
                    orders_map.erase(int_id);
                    json["message"] = "Заказ добавлен";
                    drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
                    response->setStatusCode(drogon::HttpStatusCode::k200OK);
                    callback(response);
                    return;
                },
                [callback](const drogon::orm::DrogonDbException& e) {
                    LOG_ERROR << e.base().what();
                    Json::Value json;
                    json["message"] = "Невозможно добавить заказ, проблема на сервере";
                    drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
                    response->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
                    callback(response);
                    return;
                });
        });
}

void getOrdersHandler(const drogon::HttpRequestPtr& request, Callback&& callback) {
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    if (!user_id) {
        drogon::HttpResponsePtr response {drogon::HttpResponse::newRedirectionResponse("/login.html")};
        callback(response);
        return;
    }

    drogon::orm::DbClientPtr client {drogon::app().getDbClient("data")};
    client->execSqlAsync(
        "SELECT departure, arrival, distance, cost, car_name, order_time FROM orders WHERE user_id = $1;",
        [callback](const drogon::orm::Result& result) {
            std::vector<Order> orders(result.size());
            std::size_t i {0};
            for (const drogon::orm::Row& row : result) {
                orders[i++] = Order(row);
#ifdef BUSINESS_HANDLER_DEBUG
                orders[i - 1].print();
#endif
            }
            drogon::HttpViewData data;
            data.insert("orders", orders);
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpViewResponse("Profile.csp", data)};
            callback(response);
        },
        [callback](const drogon::orm::DrogonDbException& e) {}, std::to_string(user_id));
}

void logoutHandler(const drogon::HttpRequestPtr& request, Callback&& callback) {
    request->session()->erase("userId");
    drogon::HttpResponsePtr response {drogon::HttpResponse::newRedirectionResponse("/login.html")};
    callback(response);
}