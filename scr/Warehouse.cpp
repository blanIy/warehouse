#include "Warehouse.h"
#include <iostream>

Warehouse::Warehouse(std::string_view name) : name(name) {}

std::string Warehouse::getName() const { return name; }

void Warehouse::addZone(const StorageZone& zone) {
    zones.add(zone);
}

int Warehouse::getZoneCount() const {
    return static_cast<int>(zones.getSize());
}

const StorageZone* Warehouse::getZone(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < zones.getSize()) {
        return &zones[static_cast<size_t>(index)];
    }
    return nullptr;
}

StorageZone* Warehouse::getZone(int index) {
    if (index >= 0 && static_cast<size_t>(index) < zones.getSize()) {
        return &zones[static_cast<size_t>(index)];
    }
    return nullptr;
}

bool Warehouse::placeProduct(int zoneIndex, const Product& product) {
    if (zoneIndex >= 0 && static_cast<size_t>(zoneIndex) < zones.getSize()) {
        return zones[static_cast<size_t>(zoneIndex)].addProduct(product);
    }
    return false;
}

bool Warehouse::issueProduct(std::string_view sku) {
    for (auto& zone : zones) {
        if (zone.removeProduct(sku)) {
            return true;
        }
    }
    return false;
}

void Warehouse::showInventory() const {
    std::cout << "\n========================================\n"
        << "     ÑÎÑÒÎßÍÈÅ ÑÊËÀÄÀ: \"" << name << "\"\n"
        << "========================================\n";
    if (zones.isEmpty()) {
        std::cout << "Íà ñêëàäå íåò çîí õðàíåíèÿ.\n";
        return;
    }
    for (const auto& zone : zones) {
        std::cout << zone;
    }
}