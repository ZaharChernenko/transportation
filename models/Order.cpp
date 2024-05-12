#include "Order.h"

Order::Order() : arrival {}, departure {}, distance {}, price {}, car_name {}, order_data {} {}

Order::Order(const std::string& arrival, const std::string& departure, const std::string& distance,
             const std::string& price, const std::string& car_name, const std::string& order_data)
    : arrival {arrival}, departure {departure}, distance {distance}, price {price}, car_name {car_name},
      order_data {order_data} {}

Order::Order(const drogon::orm::Row& row)
    : arrival {row["arrival"].as<std::string>()}, departure {row["departure"].as<std::string>()},
      distance {row["distance"].as<std::string>()}, price {row["cost"].as<std::string>()},
      car_name {row["car_name"].as<std::string>()}, order_data {row["order_time"].as<std::string>()} {}

bool Order::operator==(const Order& other) const {
    return arrival == other.arrival && departure == other.departure && distance == other.distance &&
           price == other.price && car_name == other.car_name;
}

Order& Order::operator=(const Order& other) {
    arrival = other.arrival;
    departure = other.departure;
    distance = other.distance;
    price = other.price;
    car_name = other.car_name;
    order_data = other.order_data;
    return *this;
}

std::size_t Order::OrderHash::operator()(const Order& order) const {
    std::size_t h1 = std::hash<std::string> {}(order.arrival);
    std::size_t h2 = std::hash<std::string> {}(order.departure);
    std::size_t h3 = std::hash<std::string> {}(order.distance);
    std::size_t h4 = std::hash<std::string> {}(order.price);
    std::size_t h5 = std::hash<std::string> {}(order.car_name);
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
}

std::string Order::getInsertQuery(const std::string& user_id) const {
    return "INSERT INTO orders (departure, arrival, distance, cost, car_name, order_time, user_id) VALUES ('" +
           departure + SQL_SEP + arrival + SQL_SEP + distance + SQL_SEP + price + SQL_SEP + car_name + "', " +
           "datetime('now')" + ", " + user_id + ");";
}

std::string Order::getArrival() const { return arrival; }
std::string Order::getDeparture() const { return departure; }
std::string Order::getDistance() const { return distance; }
std::string Order::getPrice() const { return price; }
std::string Order::getCarName() const { return car_name; }
std::string Order::getOrderData() const { return order_data; }

#ifdef DEBUG_ORDER
void Order::print() const {
    LOG_INFO << departure << ' ' << arrival << ' ' << distance << ' ' << price << ' ' << car_name << '\n';
}
#endif