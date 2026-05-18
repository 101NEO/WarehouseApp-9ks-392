#include "Menu.h"

#include "FileIO.h"

#include <iomanip>
#include <iostream>
#include <limits>

namespace {
const std::string kDataPath = "data/products.txt";
}

Menu::Menu(Warehouse& warehouse) : warehouse_(warehouse) {}

void Menu::run() {
    int choice = -1;
    while (choice != 0) {
        showMainMenu();
        choice = readInt("Выберите пункт: ");
        switch (choice) {
        case 1:
            printProducts(warehouse_.all());
            break;
        case 2:
            addProduct();
            break;
        case 3:
            editProduct();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            searchMenu();
            break;
        case 6:
            sortMenu();
            break;
        case 7:
            analyticsMenu();
            break;
        case 8:
            saveProducts();
            break;
        case 0:
            std::cout << "Завершение работы.\n";
            break;
        default:
            std::cout << "Нет такого пункта меню.\n";
            break;
        }
    }
}

void Menu::showMainMenu() const {
    std::cout << "\n=== Учет товаров на складе ===\n"
              << "1. Показать все товары\n"
              << "2. Добавить товар\n"
              << "3. Редактировать товар\n"
              << "4. Удалить товар\n"
              << "5. Поиск и фильтрация\n"
              << "6. Сортировка\n"
              << "7. Аналитика\n"
              << "8. Сохранить изменения\n"
              << "0. Выход\n";
}

void Menu::addProduct() {
    const int id = readInt("ID: ");
    const std::string name = readLine("Название: ");
    const std::string category = readLine("Категория: ");
    const double price = readDouble("Цена: ");
    const int quantity = readInt("Количество: ");

    std::string error;
    if (warehouse_.addProduct(Product(id, name, category, price, quantity), error)) {
        std::cout << "Товар добавлен.\n";
    } else {
        std::cout << "Ошибка: " << error << '\n';
    }
}

void Menu::editProduct() {
    const int id = readInt("ID товара для редактирования: ");
    const std::string name = readLine("Новое название: ");
    const std::string category = readLine("Новая категория: ");
    const double price = readDouble("Новая цена: ");
    const int quantity = readInt("Новое количество: ");

    std::string error;
    if (warehouse_.updateProduct(id, name, category, price, quantity, error)) {
        std::cout << "Товар обновлен.\n";
    } else {
        std::cout << "Ошибка: " << error << '\n';
    }
}

void Menu::deleteProduct() {
    std::cout << "1. Удалить по ID\n2. Удалить по названию\n";
    const int choice = readInt("Выберите способ: ");
    if (choice == 1) {
        const int id = readInt("ID: ");
        std::cout << (warehouse_.removeById(id) ? "Товар удален.\n" : "Товар не найден.\n");
    } else if (choice == 2) {
        const std::string name = readLine("Название: ");
        const int removed = warehouse_.removeByName(name);
        std::cout << "Удалено записей: " << removed << '\n';
    } else {
        std::cout << "Нет такого способа удаления.\n";
    }
}

void Menu::searchMenu() {
    std::cout << "1. По названию\n2. По категории\n3. По диапазону цен\n";
    const int choice = readInt("Выберите фильтр: ");
    if (choice == 1) {
        printProducts(warehouse_.searchByName(readLine("Часть названия: ")));
    } else if (choice == 2) {
        printProducts(warehouse_.filterByCategory(readLine("Категория: ")));
    } else if (choice == 3) {
        double from = readDouble("Цена от: ");
        double to = readDouble("Цена до: ");
        if (from > to) {
            std::swap(from, to);
        }
        printProducts(warehouse_.filterByPriceRange(from, to));
    } else {
        std::cout << "Нет такого фильтра.\n";
    }
}

void Menu::sortMenu() {
    std::cout << "1. По цене\n2. По количеству\n3. По названию\n";
    const int fieldChoice = readInt("Выберите поле: ");
    const int orderChoice = readInt("1 - по возрастанию, 2 - по убыванию: ");
    const bool ascending = orderChoice != 2;

    SortField field = SortField::Name;
    if (fieldChoice == 1) {
        field = SortField::Price;
    } else if (fieldChoice == 2) {
        field = SortField::Quantity;
    } else if (fieldChoice != 3) {
        std::cout << "Нет такого поля сортировки.\n";
        return;
    }

    printProducts(warehouse_.sorted(field, ascending));
}

void Menu::analyticsMenu() {
    std::cout << "\nОбщая стоимость товаров: " << std::fixed << std::setprecision(2)
              << warehouse_.totalInventoryValue() << '\n';
    std::cout << "\nТовары с остатком <= 5:\n";
    printProducts(warehouse_.lowStock());
    std::cout << "\nТоп-3 самых дорогих позиции:\n";
    printProducts(warehouse_.topExpensive());
}

void Menu::saveProducts() const {
    if (FileIO::saveProducts(kDataPath, warehouse_.all())) {
        std::cout << "Данные сохранены в " << kDataPath << '\n';
    } else {
        std::cout << "Не удалось сохранить данные.\n";
    }
}

int Menu::readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value = 0;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Введите целое число.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double Menu::readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double value = 0;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Введите число.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string Menu::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

void Menu::printProducts(const std::vector<Product>& products) {
    if (products.empty()) {
        std::cout << "Список пуст.\n";
        return;
    }

    std::cout << std::left
              << std::setw(6) << "ID"
              << std::setw(24) << "Название"
              << std::setw(18) << "Категория"
              << std::setw(12) << "Цена"
              << std::setw(12) << "Кол-во"
              << "Стоимость\n";
    std::cout << std::string(84, '-') << '\n';
    for (const Product& product : products) {
        std::cout << std::left
                  << std::setw(6) << product.getId()
                  << std::setw(24) << product.getName()
                  << std::setw(18) << product.getCategory()
                  << std::setw(12) << std::fixed << std::setprecision(2) << product.getPrice()
                  << std::setw(12) << product.getQuantity()
                  << product.totalCost() << '\n';
    }
}
