#pragma once
#include "Product.h"

class StandardProduct : public Product {
private:
    std::string packageType;

public:
    StandardProduct() = default;
    StandardProduct(std::string_view sku, std::string_view name, std::string_view category,
        double weight, double volume, std::string_view packageType = "Стандартная_тара");
    StandardProduct(const StandardProduct&) = default;
    StandardProduct& operator=(const StandardProduct&) = default;
    StandardProduct(StandardProduct&&) noexcept = default;
    StandardProduct& operator=(StandardProduct&&) noexcept = default;
    ~StandardProduct() override = default;

    std::string getPackageType() const;
    void setPackageType(std::string_view newPackageType);

    std::string getProductType() const override;
    void printDetailedReport() const override;
    double calculateDailyStorageCost() const override;
    std::unique_ptr<Product> clone() const override;

    double calculateHandlingFee() const;
};