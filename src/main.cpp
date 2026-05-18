#include "FileIO.h"
#include "Menu.h"
#include "Warehouse.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    const std::string dataPath = "data/products.txt";
    Warehouse warehouse;
    warehouse.replaceAll(FileIO::loadProducts(dataPath));

    std::cout << "Загружено товаров: " << warehouse.all().size() << '\n';
    Menu menu(warehouse);
    menu.run();

    return 0;
}
