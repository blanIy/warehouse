#include "WarehouseUI.h"
#include <iostream>

void printMenu() {
    std::cout << "\n========================================\n"
        << "         СКЛАДСКАЯ СИСТЕМА УЧЕТА        \n"
        << "========================================\n"
        << "1. Показать состояние склада и зон\n"
        << "2. Принять товар в зону хранения\n"
        << "3. Выдать товар со склада (по артикулу)\n"
        << "4. Изменить категорию товара\n"
        << "0. Выход\n"
        << "========================================\n"
        << "Выберите действие: ";
}

void handlePlacement(Warehouse& warehouse, const std::vector<Product>& catalog) {
    std::cout << "\n--- Прием товара на склад ---\n";
    const auto catalogSize = static_cast<int>(catalog.size());
    for (auto i = 0; i < catalogSize; ++i) {
        std::cout << (i + 1) << ". ";
        catalog[static_cast<size_t>(i)].printInfo();
    }
    std::cout << "Выберите товар для размещения (1-" << catalogSize << "): ";
    int pChoice = 0;
    if (!(std::cin >> pChoice) || pChoice < 1 || pChoice > catalogSize) {
        std::cout << "Ошибка ввода товара!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    const auto& selectedProduct = catalog[static_cast<size_t>(pChoice - 1)];

    const auto zoneCount = warehouse.getZoneCount();
    std::cout << "\nВыберите зону для размещения:\n";
    for (auto i = 0; i < zoneCount; ++i) {
        if (const auto* z = warehouse.getZone(i); z != nullptr) {
            std::cout << (i + 1) << ". " << z->getName()
                << " [Свободно: " << (z->getMaxWeight() - z->getCurrentWeight()) << " кг, "
                << (z->getMaxVolume() - z->getCurrentVolume()) << " м3]\n";
        }
    }
    std::cout << "Ваш выбор (1-" << zoneCount << "): ";
    int zChoice = 0;
    if (!(std::cin >> zChoice) || zChoice < 1 || zChoice > zoneCount) {
        std::cout << "Ошибка выбора зоны!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    if (warehouse.placeProduct(zChoice - 1, selectedProduct)) {
        std::cout << "Успешно! Товар \"" << selectedProduct.getName() << "\" размещен в зоне.\n";
    }
    else {
        std::cout << "ОШИБКА: Превышены ограничения зоны по массе или объему!\n";
    }
}

void handleIssue(Warehouse& warehouse) {
    std::cout << "\nВведите артикул товара для выдачи со склада (например, SKU-001): ";
    std::string sku;
    std::cin >> sku;

    if (warehouse.issueProduct(sku)) {
        std::cout << "Успешно! Товар с артикулом " << sku << " выдан со склада.\n";
    }
    else {
        std::cout << "Ошибка: Товар с артикулом " << sku << " не найден на складе!\n";
    }
}

void handleModifyProduct(Warehouse& warehouse) {
    if (warehouse.getZoneCount() == 0) return;
    auto* zone = warehouse.getZone(0);
    if (zone == nullptr || zone->getProductCount() == 0) {
        std::cout << "\nВ первой зоне нет товаров для изменения!\n";
        return;
    }

    auto* product = zone->getProduct(0);
    if (product == nullptr) return;

    std::cout << "\nТекущий товар: ";
    product->printInfo();

    std::cout << "Введите новую категорию товара (с клавиатуры): ";
    std::string newCategory;
    std::cin >> newCategory;

    product->setCategory(newCategory);
    std::cout << "Успешно! Категория товара изменена на: \"" << product->getCategory() << "\"\n";
}