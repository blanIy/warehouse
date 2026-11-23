#include "ReportWriter.h"
#include "FileStorage.h"
#include "Logger.h"
#include <iomanip>
#include <locale>
#include <sstream>

std::string ReportWriter::build(const Warehouse& warehouse) {
    std::ostringstream out;
    out.imbue(std::locale::classic());
    out << std::fixed << std::setprecision(2);

    out << "ОТЧЕТ СКЛАДСКОЙ СИСТЕМЫ (WMS)\n";
    out << "Сформирован: " << Logger::timestamp() << "\n";
    out << "============================================================\n\n";

    size_t totalProducts = 0;
    double totalWeight = 0.0;
    double totalVolume = 0.0;
    double totalDailyCost = 0.0;

    out << "ЗОНЫ ХРАНЕНИЯ (" << warehouse.getZoneCount() << ")\n";
    out << "------------------------------------------------------------\n";

    for (int i = 0; i < warehouse.getZoneCount(); ++i) {
        const auto* zone = warehouse.getZone(i);
        if (zone == nullptr) {
            continue;
        }

        const double weightPct = zone->getMaxWeight() > 0.0
            ? (zone->getCurrentWeight() / zone->getMaxWeight()) * 100.0
            : 0.0;
        const double volumePct = zone->getMaxVolume() > 0.0
            ? (zone->getCurrentVolume() / zone->getMaxVolume()) * 100.0
            : 0.0;

        out << "Зона: " << zone->getName() << "\n";
        out << "  Масса: " << zone->getCurrentWeight() << " / " << zone->getMaxWeight()
            << " кг (" << weightPct << "%)\n";
        out << "  Объем: " << zone->getCurrentVolume() << " / " << zone->getMaxVolume()
            << " м3 (" << volumePct << "%)\n";

        out << "  Товаров в зоне: " << zone->getProductCount() << "\n";

        for (int j = 0; j < zone->getProductCount(); ++j) {
            const auto* p = zone->getProduct(j);
            if (p == nullptr) {
                continue;
            }
            const double cost = p->calculateDailyStorageCost();
            out << "    - [" << p->getProductType() << "] " << p->getSku()
                << " | " << p->getName()
                << " | " << p->getWeight() << " кг, " << p->getVolume() << " м3"
                << " | Тариф: " << cost << " руб./сут.\n";

            ++totalProducts;
            totalWeight += p->getWeight();
            totalVolume += p->getVolume();
            totalDailyCost += cost;
        }
        out << "\n";
    }

    out << "ИТОГОВАЯ СВОДКА\n";
    out << "------------------------------------------------------------\n";
    out << "Всего зон хранения: " << warehouse.getZoneCount() << "\n";
    out << "Всего единиц товара: " << totalProducts << "\n";
    out << "Общая масса грузов: " << totalWeight << " кг\n";
    out << "Общий объем грузов: " << totalVolume << " м3\n";
    out << "Суммарная стоимость хранения: " << totalDailyCost << " руб./сут.\n";

    return out.str();
}

void ReportWriter::write(std::string_view path, const Warehouse& warehouse) {
    FileStorage::writeAtomic(path, build(warehouse));
}