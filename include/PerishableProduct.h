#pragma once
#include "Product.h"

class PerishableProduct : public Product {
private:
    std::string expiryDate;
    double targetTemperature = 0.0;

public:
    PerishableProduct(std::string_view sku, std::string_view name, std::string_view category,
        double weight, double volume, std::string_view expiryDate, double targetTemperature);
    PerishableProduct(const PerishableProduct&) = default;
    PerishableProduct& operator=(const PerishableProduct&) = default;
    PerishableProduct(PerishableProduct&&) noexcept = default;
    PerishableProduct& operator=(PerishableProduct&&) noexcept = default;
    ~PerishableProduct() override = default;

    std::string getExpiryDate() const;
    double getTargetTemperature() const;

    void setExpiryDate(std::string_view newExpiryDate);
    void setTargetTemperature(double newTemperature);

    std::string getProductType() const override;
    void printDetailedReport() const override;
    double calculateDailyStorageCost() const override;
    std::unique_ptr<Product> clone() const override;

    double calculateCoolingCost(double ambientTemp) const;
};