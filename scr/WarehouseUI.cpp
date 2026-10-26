#include "WarehouseUI.h"
#include <iostream>
#include <utility>
#include "StandardProduct.h"

void printMenu() {
    std::cout << "\n======================================================\n"
        << "       СИСТЕМА УПРАВЛЕНИЯ СКЛАДОМ (WMS-ТЕРМИНАЛ)      \n"
        << "======================================================\n"
        << "1. Инвентаризация склада (Ведомость остатков)\n"
        << "2. Приемка груза в зону хранения\n"
        << "3. Отгрузка груза со склада\n"
        << "4. Регистрация нового груза в номенклатуре\n"
        << "5. Логистическая экспертиза и весовой контроль\n"
        << "6. Базовый реестр номенклатуры (Общие параметры)\n"
        << "7. Технологические регламенты хранения грузов\n"
        << "8. Актуализация параметров номенклатуры\n"
        << "9. Контрольная сверка учетных артикулов\n"
        << "10. Комплексный аудит и печать паспортов всех грузов\n"
        << "11. Реестровый аудит и критериальный отбор (Грузы / Зоны)\n"
        << "0. Завершить смену\n"
        << "======================================================\n"
        << "Выберите действие: ";
}

void demonstratePolymorphicReporting(const std::vector<std::unique_ptr<Product>>& catalog) {
    std::cout << "\n======================================================\n"
        << "    КОМПЛЕКСНЫЙ ТЕХНИЧЕСКИЙ АУДИТ И ПАСПОРТИЗАЦИЯ     \n"
        << "======================================================\n";
    for (const auto& item : catalog) {
        if (item != nullptr) {
            std::cout << "Отраслевая категория: " << item->getProductType() << "\n";
            item->printDetailedReport();
            std::cout << "------------------------------------------------------\n";
        }
    }
}

void handlePlacement(Warehouse& warehouse, const std::vector<std::unique_ptr<Product>>& catalog) {
    std::cout << "\n--- Приемка груза на склад ---\n";
    const auto catalogSize = static_cast<int>(catalog.size());
    for (auto i = 0; i < catalogSize; ++i) {
        std::cout << (i + 1) << ". " << *catalog[static_cast<size_t>(i)]
            << " [" << catalog[static_cast<size_t>(i)]->getProductType() << "]\n";
    }
    std::cout << "Выберите позицию для размещения (1-" << catalogSize << "): ";
    int pChoice = 0;
    if (!(std::cin >> pChoice) || pChoice < 1 || pChoice > catalogSize) {
        std::cout << "Ошибка выбора позиции!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    const auto& selectedProduct = *catalog[static_cast<size_t>(pChoice - 1)];

    const auto zoneCount = warehouse.getZoneCount();
    std::cout << "\nДоступные зоны размещения:\n";
    for (auto i = 0; i < zoneCount; ++i) {
        if (const auto* z = warehouse.getZone(i); z != nullptr) {
            std::cout << (i + 1) << ". " << z->getName()
                << " [Резерв грузоподъемности: " << (z->getMaxWeight() - z->getCurrentWeight()) << " кг]\n";
        }
    }
    std::cout << "Назначьте зону размещения (1-" << zoneCount << "): ";
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
    std::cout << "\nВведите артикул позиции для отгрузки: ";
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
        std::cout << "[ОТКАЗ В ОТГРУЗКЕ] Товар с артикулом " << sku << " не обнаружен на остатках склада!\n";
    }
}

void handleRegisterProduct(std::vector<std::unique_ptr<Product>>& catalog,
    std::vector<PerishableProduct>& perishables,
    std::vector<FragileProduct>& fragiles,
    std::vector<ChemicalProduct>& chemicals) {
    std::cout << "\n--- Паспортизация и регистрация груза ---\n"
        << "Выберите категорию учета:\n"
        << "1. Скоропортящийся груз\n"
        << "2. Хрупкий груз\n"
        << "3. Опасный химический груз\n"
        << "4. Стандартный груз\n"
        << "Категория: ";
    int type = 0;
    if (!(std::cin >> type) || type < 1 || type > 4) {
        std::cout << "Некорректный выбор категории!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    std::string sku;
    std::string name;
    std::string category;
    double weight = 0.0;
    double volume = 0.0;

    std::cout << "Введите артикул: ";
    std::cin >> sku;
    std::cout << "Введите наименование: ";
    std::cin >> name;
    std::cout << "Введите отраслевую группу: ";
    std::cin >> category;
    std::cout << "Введите массу (кг): ";
    std::cin >> weight;
    std::cout << "Введите объем (м3): ";
    std::cin >> volume;

    if (type == 1) {
        std::string expiry;
        double temp = 0.0;
        std::cout << "Введите срок годности (ДД.ММ.ГГГГ): ";
        std::cin >> expiry;
        std::cout << "Введите температуру хранения (C): ";
        std::cin >> temp;
        PerishableProduct p(sku, name, category, weight, volume, expiry, temp);
        perishables.push_back(p);
        catalog.push_back(p.clone());
        std::cout << "\n[УСПЕХ] Зарегистрирован: " << p.getProductType() << "\n";
    }
    else if (type == 2) {
        int layers = 1;
        std::string padding;
        std::cout << "Введите макс. число ярусов штабеля: ";
        std::cin >> layers;
        std::cout << "Введите тип упаковки: ";
        std::cin >> padding;
        FragileProduct f(sku, name, category, weight, volume, layers, padding);
        fragiles.push_back(f);
        catalog.push_back(f.clone());
        std::cout << "\n[УСПЕХ] Зарегистрирован: " << f.getProductType() << "\n";
    }
    else if (type == 3) {
        std::string hazard;
        int ventChoice = 0;
        std::cout << "Введите класс опасности ADR: ";
        std::cin >> hazard;
        std::cout << "Требуется принудительная вентиляция (1 - Да, 0 - Нет): ";
        std::cin >> ventChoice;
        ChemicalProduct c(sku, name, category, weight, volume, hazard, ventChoice == 1);
        chemicals.push_back(c);
        catalog.push_back(c.clone());
        std::cout << "\n[УСПЕХ] Зарегистрирован: " << c.getProductType() << "\n";
    }
    else {
        StandardProduct prod(sku, name, category, weight, volume, "Стандартная_тара");
        catalog.push_back(prod.clone());
        std::cout << "\n[УСПЕХ] Зарегистрирован: " << prod.getProductType() << "\n";
    }
}

void compareTwoProducts(const Product& p1, const Product& p2) {
    std::cout << "\n1. Сверка идентичности номенклатурных партий:\n";
    if (p1 == p2) {
        std::cout << "   -> Артикулы совпадают (партии идентичны).\n";
    }
    else {
        std::cout << "   -> Артикулы различаются (разные номенклатурные позиции).\n";
    }

    std::cout << "2. Контроль порядка штабелирования по весу:\n";
    if (p1 < p2) {
        std::cout << "   -> Позиция \"" << p1.getName() << "\" легче позиции \"" << p2.getName() << "\".\n";
    }
    else if (p1 > p2) {
        std::cout << "   -> Позиция \"" << p1.getName() << "\" тяжелее позиции \"" << p2.getName() << "\".\n";
    }
    else {
        std::cout << "   -> Позиции имеют равную массу.\n";
    }

    std::cout << "3. Расчет удельной плотности груза:\n";
    std::cout << "   -> Плотность первого груза: " << calculateDensity(p1) << " кг/м3\n";
    std::cout << "   -> Плотность второго груза: " << calculateDensity(p2) << " кг/м3\n";
}

void rankAndAnalyzeAll(const std::vector<std::unique_ptr<Product>>& catalog) {
    std::cout << "\n======================================================\n"
        << "       ВЕДОМОСТЬ РАНЖИРОВАНИЯ ВСЕХ ГРУЗОВ ПО МАССЕ    \n"
        << "======================================================\n";

    std::vector<const Product*> items;
    for (const auto& p : catalog) {
        if (p != nullptr) items.push_back(p.get());
    }

    for (size_t i = 0; i < items.size(); ++i) {
        for (size_t j = i + 1; j < items.size(); ++j) {
            if (*items[i] < *items[j]) {
                std::swap(items[i], items[j]);
            }
        }
    }

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << (i + 1) << ". " << *items[i] << " | Плотность: " << calculateDensity(*items[i]) << " кг/м3\n";
    }
}

void handleLogisticsAnalysis(const std::vector<std::unique_ptr<Product>>& catalog) {
    const auto catalogSize = static_cast<int>(catalog.size());
    if (catalogSize < 2) {
        std::cout << "\nВ каталоге недостаточно позиций для анализа!\n";
        return;
    }

    std::cout << "\n======================================================\n"
        << "        ЛОГИСТИЧЕСКАЯ ЭКСПЕРТИЗА ДВУХ ВЫБРАННЫХ ПОЗИЦИЙ \n"
        << "======================================================\n";

    for (auto i = 0; i < catalogSize; ++i) {
        std::cout << (i + 1) << ". " << *catalog[static_cast<size_t>(i)] << "\n";
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

    compareTwoProducts(*catalog[static_cast<size_t>(c1 - 1)], *catalog[static_cast<size_t>(c2 - 1)]);
    rankAndAnalyzeAll(catalog);
}

void showInheritedFields(const std::vector<PerishableProduct>& perishables,
    const std::vector<FragileProduct>& fragiles,
    const std::vector<ChemicalProduct>& chemicals) {
    std::cout << "\n======================================================\n"
        << "       БАЗОВЫЙ РЕЕСТР НОМЕНКЛАТУРЫ (ОБЩИЕ ПАРАМЕТРЫ)  \n"
        << "======================================================\n";

    std::cout << "[Группа: Скоропортящиеся грузы]\n";
    for (const auto& p : perishables) {
        std::cout << "Артикул: " << p.getSku() << " | Наименование: " << p.getName()
            << " | Группа: " << p.getCategory() << " | Масса: " << p.getWeight()
            << " кг | Объем: " << p.getVolume() << " м3\n";
    }

    std::cout << "\n[Группа: Хрупкие и стеклянные изделия]\n";
    for (const auto& f : fragiles) {
        std::cout << "Артикул: " << f.getSku() << " | Наименование: " << f.getName()
            << " | Группа: " << f.getCategory() << " | Масса: " << f.getWeight()
            << " кг | Объем: " << f.getVolume() << " м3\n";
    }

    std::cout << "\n[Группа: Химические реактивы и материалы]\n";
    for (const auto& c : chemicals) {
        std::cout << "Артикул: " << c.getSku() << " | Наименование: " << c.getName()
            << " | Группа: " << c.getCategory() << " | Масса: " << c.getWeight()
            << " кг | Объем: " << c.getVolume() << " м3\n";
    }
    std::cout << "======================================================\n";
}

void showDerivedFields(const std::vector<PerishableProduct>& perishables,
    const std::vector<FragileProduct>& fragiles,
    const std::vector<ChemicalProduct>& chemicals) {
    std::cout << "\n======================================================\n"
        << "    ТЕХНОЛОГИЧЕСКИЕ РЕГЛАМЕНТЫ И УСЛОВИЯ ХРАНЕНИЯ     \n"
        << "======================================================\n";

    std::cout << "[Регламент: Холодильная цепь]\n";
    for (const auto& p : perishables) {
        std::cout << "Груз: " << p.getName()
            << " | Срок годности: " << p.getExpiryDate()
            << " | Температурный режим: " << p.getTargetTemperature() << " C"
            << " | Суточный тариф: " << p.calculateDailyStorageCost() << " руб.\n"
            << "   -> Расход на охлаждение: " << p.calculateCoolingCost(22.0) << " руб./сутки\n";
    }

    std::cout << "\n[Регламент: Хрупкие грузы и штабелирование]\n";
    for (const auto& f : fragiles) {
        std::cout << "Груз: " << f.getName()
            << " | Макс. высота штабеля: " << f.getMaxStackLayers() << " яруса"
            << " | Упаковка: " << f.getPaddingType()
            << " | Суточный тариф: " << f.calculateDailyStorageCost() << " руб.\n"
            << "   -> Допустимая нагрузка на нижний ярус: " << f.calculateMaxStackWeight() << " кг\n";
    }

    std::cout << "\n[Регламент: Опасные химические вещества (ADR)]\n";
    for (const auto& c : chemicals) {
        std::cout << "Груз: " << c.getName()
            << " | Классификация опасности: " << c.getHazardClass()
            << " | Принудительная вентиляция: " << (c.getRequiresVentilation() ? "Обязательно" : "Не требуется")
            << " | Суточный тариф: " << c.calculateDailyStorageCost() << " руб.\n"
            << "   -> Расход принудительной вытяжки: " << c.calculateVentilationRequirement() << " м3/час\n";
    }
    std::cout << "======================================================\n";
}

void demonstrateInheritedSetter(std::vector<PerishableProduct>& perishables,
    std::vector<FragileProduct>& fragiles,
    std::vector<ChemicalProduct>& chemicals) {
    if (perishables.empty() || fragiles.empty() || chemicals.empty()) return;

    std::cout << "\n======================================================\n"
        << "      АКТУАЛИЗАЦИЯ И РЕВИЗИЯ УЧЕТНЫХ ХАРАКТЕРИСТИК    \n"
        << "======================================================\n";

    perishables[0].setCategory("Премиальные_молочные_продукты");
    fragiles[0].setWeight(6.8);
    chemicals[0].setName("Концентрированная_серная_кислота");

    std::cout << "Успешно обновлены характеристики грузов:\n"
        << "1. Обновлена категория груза \"" << perishables[0].getName() << "\": " << perishables[0].getCategory() << "\n"
        << "2. Уточнен фактический вес \"" << fragiles[0].getName() << "\": " << fragiles[0].getWeight() << " кг\n"
        << "3. Обновлено номенклатурное название: " << chemicals[0].getName() << "\n";
    std::cout << "======================================================\n";
}

void demonstrateInheritedGetter(const std::vector<PerishableProduct>& perishables,
    const std::vector<FragileProduct>& fragiles,
    const std::vector<ChemicalProduct>& chemicals) {
    std::cout << "\n======================================================\n"
        << "           СВЕРКА НОМЕНКЛАТУРНЫХ АРТИКУЛОВ             \n"
        << "======================================================\n";

    for (const auto& p : perishables) {
        std::cout << p.getName() << " -> Учетный номер: " << p.getSku() << "\n";
    }
    for (const auto& f : fragiles) {
        std::cout << f.getName() << " -> Учетный номер: " << f.getSku() << "\n";
    }
    for (const auto& c : chemicals) {
        std::cout << c.getName() << " -> Учетный номер: " << c.getSku() << "\n";
    }
    std::cout << "======================================================\n";
}
void demonstrateTemplateCollections() {
    std::cout << "\n======================================================\n"
        << "     СВОДНЫЙ АУДИТ И СЕЛЕКЦИЯ СКЛАДСКИХ РЕЕСТРОВ      \n"
        << "======================================================\n";

    Collection<StandardProduct> productCollection;
    productCollection.add(StandardProduct("SKU-STD-01", "Холодильник_Атлант", "Бытовая_техника", 70.0, 1.2, "Коробка"));
    productCollection.add(StandardProduct("SKU-STD-02", "Станок_промышленный", "Оборудование", 1800.0, 25.0, "Поддон"));
    productCollection.add(StandardProduct("SKU-STD-03", "Паллет_цемента", "Стройматериалы", 1000.0, 1.0, "Мешки"));

    std::cout << "1. Реестр стандартных грузов [Учтено позиций: " << productCollection.getSize() << "]:\n";
    productCollection.print();

    std::cout << "\n2. Карточка позиции по индексу [1]:\n" << productCollection.get(1) << "\n";

    std::cout << "\n3. Критериальная выборка крупнотоннажных грузов (масса более 500 кг):\n";
    auto heavyProducts = filterCollection(productCollection, [](const StandardProduct& p) {
        return p.getWeight() > 500.0;
        });
    heavyProducts.print();

    std::cout << "\n------------------------------------------------------\n";

    Collection<StorageZone> zoneCollection;
    zoneCollection.add(StorageZone("Зона А (Крупногабарит)", 4000.0, 60.0));
    zoneCollection.add(StorageZone("Зона B (Спецхранение)", 800.0, 15.0));
    zoneCollection.add(StorageZone("Зона C (Легкие грузы)", 300.0, 5.0));

    std::cout << "4. Реестр активных зон и стеллажей [Всего зон: " << zoneCollection.getSize() << "]:\n";
    zoneCollection.print();

    std::cout << "\n5. Автоподбор зоны под тяжеловесный груз (лимит несущей способности от 1000 кг):\n";
    const auto* matchedZone = findFirstMatch(zoneCollection, [](const StorageZone& z) {
        return z.getMaxWeight() >= 1000.0;
        });
    if (matchedZone != nullptr) {
        std::cout << "Назначена зона: " << matchedZone->getName() << " (Лимит: " << matchedZone->getMaxWeight() << " кг)\n";
    }

    std::cout << "\n6. Вывод зоны на регламентное техобслуживание:\n";
    zoneCollection.removeAt(2);
    std::cout << "Стеллаж списан на обслуживание. Активных зон в реестре: " << zoneCollection.getSize() << "\n";
    std::cout << "======================================================\n";
}