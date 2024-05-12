#pragma once

#include <string>
#include <unordered_map>

class Car;
constexpr double PRICE_PER_KM {50};
extern std::unordered_map<std::string, const Car> cars_unordered_map;
// extern - чтобы избежать переопределения

class Car {
  private:
    std::string car_name;
    double cost_coeff;

  public:
    Car(const std::string& car_name, double cost_coeff) : car_name {car_name}, cost_coeff {cost_coeff} {};
    double getCostCoef() const { return cost_coeff; };
    std::string getCarName() const { return car_name; };
};
