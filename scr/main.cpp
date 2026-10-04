#include "WarehouseUI.h"
#include <iostream>
#include <vector>
#include <clocale>

#ifdef _WIN32
#include <windows.h>
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
    catalog.emplace_back("SKU-001", "Холодильник Атлант", "Бытовая техника", 70.0, 1.2);
    catalog.emplace_back("SKU-002", "Станок промышленный", "Оборудование", 1800.0, 25.0);
    catalog.emplace_back("SKU-003", "Коробка микросхем", "Электроника", 15.0, 0.2);
    catalog.emplace_back("SKU-004", "Паллет кирпича", "Стройматериалы", 1200.0, 1.5);
    catalog.emplace_back("SKU-005", "Набор датчиков", "Электроника", 5.0, 0.1);

    warehouse.placeProduct(0, catalog[0]);
    warehouse.placeProduct(1, catalog[2]);

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
        case 0:
            std::cout << "Завершение работы складской системы.\n";
            break;
        default:
            std::cout << "Неверный пункт меню!\n";
            break;
        }
    }

    return 0;
}