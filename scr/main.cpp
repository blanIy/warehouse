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
    warehouse.addZone(StorageZone("Зона А (Крупногабарит)", 4000.0, 60.0));
    warehouse.addZone(StorageZone("Зона B (Спецхранение)", 800.0, 15.0));

    std::vector<PerishableProduct> perishables;
    perishables.emplace_back("SKU-PER-01", "Молоко_Пастеризованное", "Продукты", 1.2, 0.001, "25.10.2026", 4.0);
    perishables.emplace_back("SKU-PER-02", "Йогурт_Натуральный", "Продукты", 0.5, 0.0005, "15.11.2026", 2.0);

    std::vector<FragileProduct> fragiles;
    fragiles.emplace_back("SKU-FRG-01", "Зеркало_Настенное", "Интерьер", 6.0, 0.04, 2, "Пенопластовый_короб");
    fragiles.emplace_back("SKU-FRG-02", "Ваза_Хрустальная", "Посуда", 2.5, 0.015, 1, "Воздушно-пузырьковая_пленка");

    std::vector<ChemicalProduct> chemicals;
    chemicals.emplace_back("SKU-CHM-01", "Серная_Кислота_ХЧ", "Химреагенты", 30.0, 0.02, "Класс_8_Едкие", true);
    chemicals.emplace_back("SKU-CHM-02", "Растворитель_Органический", "Растворители", 18.0, 0.025, "Класс_3_Огнеопасно", true);

    std::vector<Product> catalog;
    for (const auto& item : perishables) catalog.push_back(item);
    for (const auto& item : fragiles) catalog.push_back(item);
    for (const auto& item : chemicals) catalog.push_back(item);
    catalog.emplace_back("SKU-STD-01", "Холодильник_Атлант", "Бытовая_техника", 70.0, 1.2);
    catalog.emplace_back("SKU-STD-02", "Станок_промышленный", "Оборудование", 1800.0, 25.0);

    if (auto* zoneB = warehouse.getZone(1); zoneB != nullptr) {
        *zoneB += perishables[0];
        *zoneB += fragiles[0];
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
            handleRegisterProduct(catalog, perishables, fragiles, chemicals);
            break;
        case 5:
            handleLogisticsAnalysis(catalog);
            break;
        case 6:
            showInheritedFields(perishables, fragiles, chemicals);
            break;
        case 7:
            showDerivedFields(perishables, fragiles, chemicals);
            break;
        case 8:
            demonstrateInheritedSetter(perishables, fragiles, chemicals);
            break;
        case 9:
            demonstrateInheritedGetter(perishables, fragiles, chemicals);
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