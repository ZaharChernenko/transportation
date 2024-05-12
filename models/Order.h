/*
CREATE TABLE orders (
  id INTEGER PRIMARY KEY,
  departure TEXT NOT NULL,
  arrival TEXT NOT NULL,
  distance INTEGER NOT NULL,
  cost INTEGER NOT NULL,
  car_name TEXT NOT NULL,
  order_time DATETIME NOT NULL,
  user_id INTEGER,
  FOREIGN KEY (user_id) REFERENCES user(id)
);
*/

#pragma once

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <exception>
#include <string>

#define SQL_SEP "', '"
#define DEBUG_ORDER

class Order {
  public:
    Order();
    Order(const drogon::orm::Row& row);
    Order(const std::string& arrival, const std::string& departure, const std::string& distance,
          const std::string& price, const std::string& car_name, const std::string& order_data = "");

    bool operator==(const Order& other) const;
    Order& operator=(const Order& other);

    struct OrderHash {
        std::size_t operator()(const Order& order) const;
    };
    std::string getInsertQuery(const std::string& user_id) const;
    std::string getArrival() const;
    std::string getDeparture() const;
    std::string getDistance() const;
    std::string getPrice() const;
    std::string getCarName() const;
    std::string getOrderData() const;

#ifdef DEBUG_ORDER
    void print() const;
#endif

  protected:
    std::string arrival, departure, distance, price, car_name, order_data;
};
