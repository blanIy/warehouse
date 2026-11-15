#include "Warehouse.h"
#include <iostream>
#include "OutOfRangeException.h"
#include "ObjectNotFoundException.h"

Warehouse::Warehouse(std::string_view name) : name(name) {}

std::string Warehouse::getName() const { return name; }

void Warehouse::addZone(const StorageZone& zone) {
    zones.add(zone);
}

int Warehouse::getZoneCount() const {
    return static_cast<int>(zones.getSize());
}

bool Warehouse::placeProduct(int zoneIndex, const Product& product) {
    if (zoneIndex >= 0 && static_cast<size_t>(zoneIndex) < zones.getSize()) {
        return zones[static_cast<size_t>(zoneIndex)].addProduct(product);
    }
    return false;
}

const StorageZone* Warehouse::getZone(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= zones.getSize()) {
        throw OutOfRangeException("обращение к несуществующей зоне хранения", static_cast<size_t>(index), zones.getSize());
    }
    return &zones[static_cast<size_t>(index)];
}

StorageZone* Warehouse::getZone(int index) {
    if (index < 0 || static_cast<size_t>(index) >= zones.getSize()) {
        throw OutOfRangeException("обращение к несуществующей зоне хранения", static_cast<size_t>(index), zones.getSize());
    }
    return &zones[static_cast<size_t>(index)];
}

bool Warehouse::issueProduct(std::string_view sku) {
    for (size_t i = 0; i < zones.getSize(); ++i) {
        auto& zone = zones[i];
        for (int p = 0; p < zone.getProductCount(); ++p) {
            if (zone.getProduct(p)->getSku() == sku) {
                return zone.removeProduct(sku);
            }
        }
    }
    throw ObjectNotFoundException("груз не числится ни в одной зоне склада", sku);
}

void Warehouse::showInventory() const {
    std::cout << "\n========================================\n"
        << "     СОСТОЯНИЕ СКЛАДА: \"" << name << "\"\n"
        << "========================================\n";
    if (zones.isEmpty()) {
        std::cout << "На складе нет зон хранения.\n";
        return;
    }
    for (const auto& zone : zones) {
        std::cout << zone;
    }
}