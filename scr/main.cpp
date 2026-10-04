#include "WarehouseUI.h"
#include <iostream>
#include <vector>
#include <clocale>

#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
    setlocale(LC_ALL, "Russian");

#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    Warehouse warehouse("Центральный логистический комплекс");
    warehouse.addZone(StorageZone("Зона А (Крупногабарит)", 2000.0, 50.0));
    warehouse.addZone(StorageZone("Зона B (Мелкие товары)", 300.0, 5.0));

    std::vector<Product> catalog;
    catalog.emplace_back("SKU-001", "Холодильник_Атлант", "Бытовая_техника", 70.0, 1.2);
    catalog.emplace_back("SKU-002", "Станок_промышленный", "Оборудование", 1800.0, 25.0);
    catalog.emplace_back("SKU-003", "Коробка_микросхем", "Электроника", 15.0, 0.2);
    catalog.emplace_back("SKU-004", "Паллет_кирпича", "Стройматериалы", 1200.0, 1.5);
    catalog.emplace_back("SKU-005", "Набор_датчиков", "Электроника", 5.0, 0.1);

    auto* zoneA = warehouse.getZone(0);
    if (zoneA != nullptr) {
        *zoneA += catalog[0];
    }
    auto* zoneB = warehouse.getZone(1);
    if (zoneB != nullptr) {
        *zoneB += catalog[2];
    }

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Пожалуйста, введите корректное число!\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            warehouse.showInventory();
            break;
        case 2:
            handlePlacement(warehouse, catalog);
            break;
        case 3:
            handleIssue(warehouse);
            break;
        case 4:
            handleModifyProduct(warehouse);
            break;
        case 5:
            handleRegisterProduct(catalog);
            break;
        case 6:
            handleLogisticsAnalysis(catalog);
            break;
        case 0:
            std::cout << "Смена завершена. База данных склада сохранена.\n";
            break;
        default:
            std::cout << "Неверный пункт меню!\n";
            break;
        }
    }

    return 0;
}