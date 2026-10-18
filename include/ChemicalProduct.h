#pragma once
#include "Product.h"

class ChemicalProduct : public Product {
private:
    std::string hazardClass;
    bool requiresVentilation = false;

public:
    ChemicalProduct(std::string_view sku, std::string_view name, std::string_view category,
        double weight, double volume, std::string_view hazardClass, bool requiresVentilation);
    ChemicalProduct(const ChemicalProduct&) = default;
    ChemicalProduct& operator=(const ChemicalProduct&) = default;
    ChemicalProduct(ChemicalProduct&&) noexcept = default;
    ChemicalProduct& operator=(ChemicalProduct&&) noexcept = default;
    ~ChemicalProduct() override = default;

    std::string getHazardClass() const;
    bool getRequiresVentilation() const;

    void setHazardClass(std::string_view newHazardClass);
    void setRequiresVentilation(bool newVentilation);

    double calculateDailyStorageCost() const override;

    double calculateVentilationRequirement() const;
};