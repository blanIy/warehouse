#include "WarehouseUI.h"
#include <iostream>
#include <utility>
#include "LimitExceededException.h"
#include "ObjectNotFoundException.h"
#include "DuplicateIdException.h"
#include "OutOfRangeException.h"
#include "InvalidDataException.h"
#include "InvalidOperationException.h"
#include "RelationException.h"
#include "FileStorage.h"
#include "Logger.h"
#include "ReportWriter.h"
namespace {
    const std::string STATE_FILE_PATH = "data/warehouse_state.txt";
    const std::string LOG_FILE_PATH = "data/warehouse.log";
    const std::string REPORT_FILE_PATH = "data/report.txt";
}

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
        << "11. Сводный аудит и селекция складских реестров\n"
        << "12. Регламентный стресс-тест отказоустойчивости и исключений\n"
        << "13. Сохранить состояние склада в файл\n"
        << "14. Загрузить и объединить данные из файла\n"
        << "15. Сформировать текстовый отчет\n"
        << "16. Просмотреть журнал операций\n"
        << "0. Завершить смену\n"
        << "======================================================\n"
        << "Выберите действие: ";
}

void demonstratePolymorphicReporting(const Collection<std::shared_ptr<Product>>& catalog) {
    std::cout << "\n======================================================\n"
        << "    КОМПЛЕКСНЫЙ ТЕХНИЧЕСКИЙ АУДИТ И ПАСПОРТИЗАЦИЯ     \n"
        << "======================================================\n";
    for (size_t i = 0; i < catalog.getSize(); ++i) {
        const auto& item = catalog[i];
        if (item != nullptr) {
            std::cout << "Отраслевая категория: " << item->getProductType() << "\n";
            item->printDetailedReport();
            std::cout << "------------------------------------------------------\n";
        }
    }
}

void handlePlacement(Warehouse& warehouse, const Collection<std::shared_ptr<Product>>& catalog) {
    std::cout << "\n--- Приемка груза на склад ---\n";
    const auto catalogSize = static_cast<int>(catalog.getSize());
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
        Logger::append(LOG_FILE_PATH, "Приемка груза " + selectedProduct.getSku() + " в зону " + targetZone->getName());
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
                Logger::append(LOG_FILE_PATH, "Отгрузка груза " + sku + " из зоны " + zone->getName());
                found = true;
                break;
            }
        }
    }
    if (!found) {
        std::cout << "[ОТКАЗ В ОТГРУЗКЕ] Товар с артикулом " << sku << " не обнаружен на остатках склада!\n";
    }
}

void handleRegisterProduct(Collection<std::shared_ptr<Product>>& catalog,
    Collection<PerishableProduct>& perishables,
    Collection<FragileProduct>& fragiles,
    Collection<ChemicalProduct>& chemicals) {
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
        perishables.add(p);
        catalog.add(std::make_shared<PerishableProduct>(p));
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
        fragiles.add(f);
        catalog.add(std::make_shared<FragileProduct>(f));
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
        chemicals.add(c);
        catalog.add(std::make_shared<ChemicalProduct>(c));
        std::cout << "\n[УСПЕХ] Зарегистрирован: " << c.getProductType() << "\n";
    }
    else {
        StandardProduct prod(sku, name, category, weight, volume, "Стандартная_тара");
        catalog.add(std::make_shared<StandardProduct>(prod));
        std::cout << "\n[УСПЕХ] Зарегистрирован: " << prod.getProductType() << "\n";
    }
    Logger::append(LOG_FILE_PATH, "Зарегистрирован новый груз в номенклатуре: " + sku + " (" + name + ")");
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

void rankAndAnalyzeAll(const Collection<std::shared_ptr<Product>>& catalog) {
    std::cout << "\n======================================================\n"
        << "       ВЕДОМОСТЬ РАНЖИРОВАНИЯ ВСЕХ ГРУЗОВ ПО МАССЕ    \n"
        << "======================================================\n";

    std::vector<const Product*> items;
    for (size_t i = 0; i < catalog.getSize(); ++i) {
        if (catalog[i] != nullptr) items.push_back(catalog[i].get());
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

void handleLogisticsAnalysis(const Collection<std::shared_ptr<Product>>& catalog) {
    const auto catalogSize = static_cast<int>(catalog.getSize());
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

void showInheritedFields(const Collection<PerishableProduct>& perishables,
    const Collection<FragileProduct>& fragiles,
    const Collection<ChemicalProduct>& chemicals) {
    std::cout << "\n======================================================\n"
        << "       БАЗОВЫЙ РЕЕСТР НОМЕНКЛАТУРЫ (ОБЩИЕ ПАРАМЕТРЫ)  \n"
        << "======================================================\n";

    std::cout << "[Группа: Скоропортящиеся грузы]\n";
    for (size_t i = 0; i < perishables.getSize(); ++i) {
        const auto& p = perishables[i];
        std::cout << "Артикул: " << p.getSku() << " | Наименование: " << p.getName()
            << " | Группа: " << p.getCategory() << " | Масса: " << p.getWeight()
            << " кг | Объем: " << p.getVolume() << " м3\n";
    }

    std::cout << "\n[Группа: Хрупкие и стеклянные изделия]\n";
    for (size_t i = 0; i < fragiles.getSize(); ++i) {
        const auto& f = fragiles[i];
        std::cout << "Артикул: " << f.getSku() << " | Наименование: " << f.getName()
            << " | Группа: " << f.getCategory() << " | Масса: " << f.getWeight()
            << " кг | Объем: " << f.getVolume() << " м3\n";
    }

    std::cout << "\n[Группа: Химические реактивы и материалы]\n";
    for (size_t i = 0; i < chemicals.getSize(); ++i) {
        const auto& c = chemicals[i];
        std::cout << "Артикул: " << c.getSku() << " | Наименование: " << c.getName()
            << " | Группа: " << c.getCategory() << " | Масса: " << c.getWeight()
            << " кг | Объем: " << c.getVolume() << " м3\n";
    }
    std::cout << "======================================================\n";
}

void showDerivedFields(const Collection<PerishableProduct>& perishables,
    const Collection<FragileProduct>& fragiles,
    const Collection<ChemicalProduct>& chemicals) {
    std::cout << "\n======================================================\n"
        << "    ТЕХНОЛОГИЧЕСКИЕ РЕГЛАМЕНТЫ И УСЛОВИЯ ХРАНЕНИЯ     \n"
        << "======================================================\n";

    std::cout << "[Регламент: Холодильная цепь]\n";
    for (size_t i = 0; i < perishables.getSize(); ++i) {
        const auto& p = perishables[i];
        std::cout << "Груз: " << p.getName()
            << " | Срок годности: " << p.getExpiryDate()
            << " | Температурный режим: " << p.getTargetTemperature() << " C"
            << " | Суточный тариф: " << p.calculateDailyStorageCost() << " руб.\n"
            << "   -> Расход на охлаждение: " << p.calculateCoolingCost(22.0) << " руб./сутки\n";
    }

    std::cout << "\n[Регламент: Хрупкие грузы и штабелирование]\n";
    for (size_t i = 0; i < fragiles.getSize(); ++i) {
        const auto& f = fragiles[i];
        std::cout << "Груз: " << f.getName()
            << " | Макс. высота штабеля: " << f.getMaxStackLayers() << " яруса"
            << " | Упаковка: " << f.getPaddingType()
            << " | Суточный тариф: " << f.calculateDailyStorageCost() << " руб.\n"
            << "   -> Допустимая нагрузка на нижний ярус: " << f.calculateMaxStackWeight() << " кг\n";
    }

    std::cout << "\n[Регламент: Опасные химические вещества (ADR)]\n";
    for (size_t i = 0; i < chemicals.getSize(); ++i) {
        const auto& c = chemicals[i];
        std::cout << "Груз: " << c.getName()
            << " | Классификация опасности: " << c.getHazardClass()
            << " | Принудительная вентиляция: " << (c.getRequiresVentilation() ? "Обязательно" : "Не требуется")
            << " | Суточный тариф: " << c.calculateDailyStorageCost() << " руб.\n"
            << "   -> Расход принудительной вытяжки: " << c.calculateVentilationRequirement() << " м3/час\n";
    }
    std::cout << "======================================================\n";
}

void demonstrateInheritedSetter(Collection<PerishableProduct>& perishables,
    Collection<FragileProduct>& fragiles,
    Collection<ChemicalProduct>& chemicals) {
    if (perishables.isEmpty() || fragiles.isEmpty() || chemicals.isEmpty()) return;

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

void demonstrateInheritedGetter(const Collection<PerishableProduct>& perishables,
    const Collection<FragileProduct>& fragiles,
    const Collection<ChemicalProduct>& chemicals) {
    std::cout << "\n======================================================\n"
        << "           СВЕРКА НОМЕНКЛАТУРНЫХ АРТИКУЛОВ             \n"
        << "======================================================\n";

    for (size_t i = 0; i < perishables.getSize(); ++i) {
        std::cout << perishables[i].getName() << " -> Артикул: " << perishables[i].getSku() << "\n";
    }
    for (size_t i = 0; i < fragiles.getSize(); ++i) {
        std::cout << fragiles[i].getName() << " -> Артикул: " << fragiles[i].getSku() << "\n";
    }
    for (size_t i = 0; i < chemicals.getSize(); ++i) {
        std::cout << chemicals[i].getName() << " -> Артикул: " << chemicals[i].getSku() << "\n";
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
    if (const auto* matchedZone = findFirstMatch(zoneCollection, [](const StorageZone& z) {
        return z.getMaxWeight() >= 1000.0;
        }); matchedZone != nullptr) {
        std::cout << "Назначена зона: " << matchedZone->getName() << " (Лимит: " << matchedZone->getMaxWeight() << " кг)\n";
    }

    std::cout << "\n6. Вывод зоны на регламентное техобслуживание:\n";
    if (zoneCollection.removeAt(2)) {
        std::cout << "Стеллаж списан на обслуживание. Активных зон в реестре: " << zoneCollection.getSize() << "\n";
    }
    std::cout << "======================================================\n";
}

void printExceptionsMenu() {
    std::cout << "\n======================================================\n"
        << "       РЕВИЗИЯ РЕГЛАМЕНТОВ И НЕШТАТНЫХ СИТУАЦИЙ       \n"
        << "======================================================\n"
        << "1. Ввод некорректных физических параметров груза\n"
        << "2. Превышение лимита грузоподъемности зоны хранения\n"
        << "3. Регистрация груза с дублирующимся идентификатором\n"
        << "4. Списание отсутствующего груза по артикулу\n"
        << "5. Адресация зоны хранения по недопустимому индексу\n"
        << "6. Выполнение недопустимой технологической операции\n"
        << "7. Ликвидация зоны с привязанными материальными ценностями\n"
        << "0. Возврат в главное меню\n"
        << "======================================================\n"
        << "Выберите пункт меню: ";
}

void handleExceptionsMenu(Warehouse& warehouse, const Collection<std::shared_ptr<Product>>& catalog) {
    int subChoice = -1;
    while (subChoice != 0) {
        printExceptionsMenu();
        if (!(std::cin >> subChoice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (subChoice) {
        case 1: {
            std::cout << "Введите массу груза (кг): ";
            double weight = 0.0;
            std::cin >> weight;
            try {
                StandardProduct prod("SKU-TMP", "Тестовый_груз", "Категория", 10.0, 0.5);
                prod.setWeight(weight);
            }
            catch (const InvalidDataException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append("data/warehouse.log", std::string("ОШИБКА: ") + ex.what());
            }
            break;
        }
        case 2: {
            std::cout << "Лимит зоны B составляет 800.00 кг.\n";
            std::cout << "Введите массу размещаемой партии (кг): ";
            double weight = 0.0;
            std::cin >> weight;
            try {
                StandardProduct prod("SKU-TMP", "Тяжелая_партия", "Сырье", weight, 1.0);
                auto* zone = warehouse.getZone(1);
                zone->addProduct(prod);
            }
            catch (const LimitExceededException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append("data/warehouse.log", std::string("ОШИБКА: ") + ex.what());
            }
            break;
        }
        case 3: {
            std::cout << "Введите артикул для регистрации: ";
            std::string sku;
            std::cin >> sku;
            try {
                for (size_t i = 0; i < catalog.getSize(); ++i) {
                    if (catalog[i]->getSku() == sku) {
                        throw DuplicateIdException("груз с данным артикулом уже зарегистрирован", sku);
                    }
                }
                std::cout << "Артикул свободен для регистрации.\n";
            }
            catch (const DuplicateIdException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append("data/warehouse.log", std::string("ОШИБКА: ") + ex.what());
            }
            break;
        }
        case 4: {
            std::cout << "Введите артикул груза для списания: ";
            std::string sku;
            std::cin >> sku;
            try {
                warehouse.issueProduct(sku);
            }
            catch (const ObjectNotFoundException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append("data/warehouse.log", std::string("ОШИБКА: ") + ex.what());
            }
            break;
        }
        case 5: {
            std::cout << "Всего доступно зон: " << warehouse.getZoneCount() << "\n";
            std::cout << "Введите индекс зоны для инспекции: ";
            int index = 0;
            std::cin >> index;
            try {
                warehouse.getZone(index);
            }
            catch (const OutOfRangeException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append("data/warehouse.log", std::string("ОШИБКА: ") + ex.what());
            }
            break;
        }
        case 6: {
            std::cout << "Введите код запрашиваемой регламентной операции: ";
            int opCode = 0;
            std::cin >> opCode;
            try {
                if (opCode != 1) {
                    throw InvalidOperationException("прямое изменение класса ADR для взрывоопасных веществ запрещено", "reclassifyHazard");
                }
            }
            catch (const InvalidOperationException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append("data/warehouse.log", std::string("ОШИБКА: ") + ex.what());
            }
            break;
        }
        case 7: {
            std::cout << "Введите индекс зоны для ликвидации: ";
            int zoneIdx = 0;
            std::cin >> zoneIdx;
            try {
                const auto* zone = warehouse.getZone(zoneIdx);
                if (zone->getProductCount() > 0) {
                    throw RelationException("нельзя удалить зону хранения, в ней находятся материальные ценности", zone->getName());
                }
            }
            catch (const RelationException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append(LOG_FILE_PATH, std::string("ОШИБКА: ") + ex.what());
            }
            catch (const WarehouseException& ex) {
                std::cout << ex.what() << "\n";
                Logger::append(LOG_FILE_PATH, std::string("ОШИБКА: ") + ex.what());
            }
            break;
        }
        case 0:
            break;
        default:
            std::cout << "Неверный пункт подменю!\n";
            break;
        }
    }
}
void handleSaveState(const Warehouse& warehouse) {
    try {
        FileStorage::save(STATE_FILE_PATH, warehouse);
        Logger::append(LOG_FILE_PATH, "Состояние склада сохранено в файл: " + STATE_FILE_PATH);
        std::cout << "Состояние склада успешно сохранено в файл \"" << STATE_FILE_PATH << "\".\n";
    }
    catch (const std::exception& e) {
        Logger::append(LOG_FILE_PATH, std::string("ОШИБКА сохранения состояния: ") + e.what());
        std::cerr << "Ошибка сохранения состояния: " << e.what() << "\n";
    }
}

void handleLoadState(Warehouse& warehouse) {
    try {
        FileStorage::loadAndMerge(STATE_FILE_PATH, warehouse);
        Logger::append(LOG_FILE_PATH, "Выполнена загрузка и слияние данных из файла: " + STATE_FILE_PATH);
        std::cout << "Данные из файла \"" << STATE_FILE_PATH << "\" успешно загружены и объединены с текущим складом.\n";
    }
    catch (const std::exception& e) {
        Logger::append(LOG_FILE_PATH, std::string("ОШИБКА загрузки состояния: ") + e.what());
        std::cerr << "Ошибка загрузки состояния: " << e.what() << "\n";
    }   
}

void handleGenerateReport(const Warehouse& warehouse) {
    try {
        ReportWriter::write(REPORT_FILE_PATH, warehouse);
        Logger::append(LOG_FILE_PATH, "Сформирован текстовый отчет: " + REPORT_FILE_PATH);
        std::cout << "Текстовый отчет успешно сформирован: \"" << REPORT_FILE_PATH << "\".\n";
    }
    catch (const std::exception& e) {
        Logger::append(LOG_FILE_PATH, std::string("ОШИБКА формирования отчета: ") + e.what());
        std::cerr << "Ошибка формирования отчета: " << e.what() << "\n";
    }
}

void handleShowJournal() {
    try {
        const std::string content = Logger::readAll(LOG_FILE_PATH);
        std::cout << "\n=== ЖУРНАЛ ОПЕРАЦИЙ (" << LOG_FILE_PATH << ") ===\n";
        if (content.empty()) {
            std::cout << "Журнал пуст.\n";
        }
        else {
            std::cout << content;
        }
        std::cout << "==========================================\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка чтения журнала: " << e.what() << "\n";
    }
}