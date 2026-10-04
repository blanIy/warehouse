#pragma once
#include "StorageZone.h"
#include <string>
#include <vector>
#include <string_view>

class Warehouse {
private:
    std::string name;
    std::vector<StorageZone> zones;

public:
    explicit Warehouse(std::string_view name);

    std::string getName() const;
    void addZone(const StorageZone& zone);
    int getZoneCount() const;

    const StorageZone* getZone(int index) const;
    StorageZone* getZone(int index);

    bool placeProduct(int zoneIndex, const Product& product);
    bool issueProduct(std::string_view sku);

    void showInventory() const;
};