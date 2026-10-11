#pragma once
#include "Product.h"

class PerishableProduct : public Product {
private:
    std::string expiryDate;
    double targetTemperature = 0.0;

public:
    PerishableProduct(std::string_view sku, std::string_view name, std::string_view category,
        double weight, double volume, std::string_view expiryDate, double targetTemperature);

    std::string getExpiryDate() const;
    double getTargetTemperature() const;

    void setExpiryDate(std::string_view newExpiryDate);
    void setTargetTemperature(double newTemperature);
};