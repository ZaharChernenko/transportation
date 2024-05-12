#include "business.h"

double countPrice(double default_price, double coeff, double distance, bool is_ensurance) {
    double price {default_price};
    price += distance * PRICE_PER_KM;
    price *= coeff;
    if (is_ensurance)
        price *= 1.03;
    return price;
}