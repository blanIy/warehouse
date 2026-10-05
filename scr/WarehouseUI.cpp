#include "WarehouseUI.h"
#include <iostream>
#include <algorithm>

void printMenu() {
    std::cout << "\n========================================\n"
        << "   СИСТЕМА УПРАВЛЕНИЯ СКЛАДОМ (WMS-23)  \n"
        << "========================================\n"
        << "1. Инвентаризация склада (Ведомость остатков)\n"
        << "2. Приемка груза в зону хранения\n"
        << "3. Отгрузка товара со склада (по артикулу)\n"
        << "4. Корректировка номенклатурной категории\n"
        << "5. Регистрация нового товара в каталоге\n"
        << "6. Логистический анализ и ранжирование всех грузов\n"
        << "0. Завершить смену\n"
        << "========================================\n"
        << "Выберите действие: ";
}

void handlePlacement(Warehouse& warehouse, const std::vector<Product>& catalog) {
    std::cout << "\n--- Приемка груза на склад ---\n";
    const auto catalogSize = static_cast<int>(catalog.size());
    for (auto i = 0; i < catalogSize; ++i) {
        std::cout << (i + 1) << ". " << catalog[static_cast<size_t>(i)] << "\n";
    }
    std::cout << "Выберите позицию для размещения (1-" << catalogSize << "): ";
    int pChoice = 0;
    if (!(std::cin >> pChoice) || pChoice < 1 || pChoice > catalogSize) {
        std::cout << "Ошибка выбора позиции!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    const auto& selectedProduct = catalog[static_cast<size_t>(pChoice - 1)];

    const auto zoneCount = warehouse.getZoneCount();
    std::cout << "\nДоступные зоны размещения:\n";
    for (auto i = 0; i < zoneCount; ++i) {
        if (const auto* z = warehouse.getZone(i); z != nullptr) {
            std::cout << (i + 1) << ". " << z->getName()
                << " [Резерв: " << (z->getMaxWeight() - z->getCurrentWeight()) << " кг, "
                << (z->getMaxVolume() - z->getCurrentVolume()) << " м3]\n";
        }
    }
    std::cout << "Назначьте зону хранения (1-" << zoneCount << "): ";
    int zChoice = 0;
    if (!(std::cin >> zChoice) || zChoice < 1 || zChoice > zoneCount) {
        std::cout << "Ошибка выбора зоны!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    auto* targetZone = warehouse.getZone(zChoice - 1);
    if (targetZone != nullptr) {
        *targetZone += selectedProduct;
    }
}

void handleIssue(Warehouse& warehouse) {
    std::cout << "\nВведите артикул позиции для отгрузки (например, SKU-001): ";
    std::string sku;
    std::cin >> sku;

    bool found = false;
    for (auto i = 0; i < warehouse.getZoneCount(); ++i) {
        auto* zone = warehouse.getZone(i);
        if (zone != nullptr) {
            int beforeCount = zone->getProductCount();
            *zone -= sku;
            if (zone->getProductCount() < beforeCount) {
                found = true;
                break;
            }
        }
    }
    if (!found) {
        std::cout << "[ОТКАЗ] Товар с артикулом " << sku << " не обнаружен ни в одной из зон склада!\n";
    }
}

void handleModifyProduct(Warehouse& warehouse) {
    if (warehouse.getZoneCount() == 0) return;
    auto* zone = warehouse.getZone(0);
    if (zone == nullptr || zone->getProductCount() == 0) {
        std::cout << "\nВ первой зоне нет товаров для корректировки!\n";
        return;
    }

    auto* product = zone->getProduct(0);
    if (product == nullptr) return;

    std::cout << "\nКарточка выбранного товара:\n" << *product << "\n";
    std::cout << "Введите уточненную категорию: ";
    std::string newCategory;
    std::cin >> newCategory;

    product->setCategory(newCategory);
    std::cout << "[УСПЕХ] Номенклатурная категория обновлена: \"" << product->getCategory() << "\"\n";
}

void handleRegisterProduct(std::vector<Product>& catalog) {
    std::cout << "\n--- Паспортизация нового товара в системе ---\n";
    Product newProduct;
    std::cin >> newProduct;
    catalog.push_back(newProduct);
    std::cout << "\n[УСПЕХ] Товар зарегистрирован в реестре:\n" << newProduct << "\n";
}

void handleLogisticsAnalysis(const std::vector<Product>& catalog) {
    if (catalog.empty()) {
        std::cout << "\nКаталог товаров пуст!\n";
        return;
    }

    std::cout << "\n======================================================\n"
        << "       ЛОГИСТИЧЕСКИЙ АНАЛИЗ И СРАВНЕНИЕ ВСЕХ ГРУЗОВ   \n"
        << "======================================================\n";

    auto sortedItems = catalog;
    std::sort(sortedItems.rbegin(), sortedItems.rend());

    std::cout << "Ранжирование всех грузов по массе (от тяжелых к легким):\n\n";
    const auto total = static_cast<int>(sortedItems.size());
    for (auto i = 0; i < total; ++i) {
        const auto& p = sortedItems[static_cast<size_t>(i)];
        std::cout << (i + 1) << ". " << p << "\n"
            << "   - Плотность: " << calculateDensity(p) << " кг/м3\n";

        if (i == 0) {
            std::cout << "   - Рекомендация: Нижний ярус (Самый тяжелый груз)\n";
        }
        else if (i == total - 1) {
            std::cout << "   - Рекомендация: Верхний ярус стеллажа (Самый легкий)\n";
        }
        else {
            std::cout << "   - Рекомендация: Средний ярус стеллажа\n";
        }
        std::cout << "\n";
    }

    std::cout << "------------------------------------------------------\n"
        << "Сводные данные экспертизы:\n"
        << "Самый тяжелый груз: " << sortedItems.front().getName() << " (" << sortedItems.front().getWeight() << " кг)\n"
        << "Самый легкий груз:  " << sortedItems.back().getName() << " (" << sortedItems.back().getWeight() << " кг)\n"
        << "======================================================\n";
}