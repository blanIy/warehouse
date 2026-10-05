#include "WarehouseUI.h"
#include <iostream>
#include <utility>

void printMenu() {
    std::cout << "\n========================================\n"
        << "   СИСТЕМА УПРАВЛЕНИЯ СКЛАДОМ (WMS-23)  \n"
        << "========================================\n"
        << "1. Инвентаризация склада (Ведомость остатков)\n"
        << "2. Приемка груза в зону хранения\n"
        << "3. Отгрузка товара со склада (по артикулу)\n"
        << "4. Корректировка номенклатурной категории\n"
        << "5. Регистрация нового товара в каталоге\n"
        << "6. Логистический анализ и явное сравнение грузов\n"
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
    const auto catalogSize = static_cast<int>(catalog.size());
    if (catalogSize < 2) {
        std::cout << "\nВ каталоге недостаточно позиций для анализа!\n";
        return;
    }

    std::cout << "\n======================================================\n"
        << "          ЯВНОЕ СРАВНЕНИЕ ДВУХ ВЫБРАННЫХ ГРУЗОВ       \n"
        << "======================================================\n";

    for (auto i = 0; i < catalogSize; ++i) {
        std::cout << (i + 1) << ". " << catalog[static_cast<size_t>(i)] << "\n";
    }

    std::cout << "\nВыберите первый груз (1-" << catalogSize << "): ";
    int c1 = 0;
    std::cin >> c1;
    std::cout << "Выберите второй груз (1-" << catalogSize << "): ";
    int c2 = 0;
    std::cin >> c2;

    if (c1 < 1 || c1 > catalogSize || c2 < 1 || c2 > catalogSize) {
        std::cout << "Ошибка ввода номеров позиций!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    const auto& p1 = catalog[static_cast<size_t>(c1 - 1)];
    const auto& p2 = catalog[static_cast<size_t>(c2 - 1)];

    std::cout << "\n1. Явная проверка равенства (operator==):\n";
    if (p1 == p2) {
        std::cout << "   -> Артикулы совпадают (партии идентичны).\n";
    }
    else {
        std::cout << "   -> Артикулы различаются (разные номенклатурные позиции).\n";
    }

    std::cout << "2. Явная проверка отношения (operator< и operator>):\n";
    if (p1 < p2) {
        std::cout << "   -> Груз \"" << p1.getName() << "\" ЛЕГЧЕ, чем \"" << p2.getName() << "\".\n";
    }
    else if (p1 > p2) {
        std::cout << "   -> Груз \"" << p1.getName() << "\" ТЯЖЕЛЕЕ, чем \"" << p2.getName() << "\".\n";
    }
    else {
        std::cout << "   -> Грузы имеют одинаковую массу.\n";
    }

    std::cout << "3. Расчет плотности (дружественная функция calculateDensity):\n";
    std::cout << "   -> Плотность первого груза: " << calculateDensity(p1) << " кг/м3\n";
    std::cout << "   -> Плотность второго груза: " << calculateDensity(p2) << " кг/м3\n";

    std::cout << "\n======================================================\n"
        << "       ЯВНОЕ СРАВНЕНИЕ И РАНЖИРОВАНИЕ ВСЕХ ГРУЗОВ     \n"
        << "======================================================\n";

    auto items = catalog;
    for (size_t i = 0; i < items.size(); ++i) {
        for (size_t j = i + 1; j < items.size(); ++j) {
            if (items[i] < items[j]) {
                std::swap(items[i], items[j]);
            }
        }
    }

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << (i + 1) << ". " << items[i] << " | Плотность: " << calculateDensity(items[i]) << " кг/м3\n";
    }

    Product heaviest = items[0];
    Product lightest = items[0];
    for (const auto& item : items) {
        if (item > heaviest) {
            heaviest = item;
        }
        if (item < lightest) {
            lightest = item;
        }
    }

    std::cout << "\nИтоги явного анализа:\n"
        << "Самый тяжелый груз (определен через operator>): " << heaviest.getName() << " (" << heaviest.getWeight() << " кг)\n"
        << "Самый легкий груз  (определен через operator<): " << lightest.getName() << " (" << lightest.getWeight() << " кг)\n"
        << "======================================================\n";
}