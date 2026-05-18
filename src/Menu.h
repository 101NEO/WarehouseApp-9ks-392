#pragma once

#include "Warehouse.h"

#include <string>
#include <vector>

class Menu {
public:
    explicit Menu(Warehouse& warehouse);
    void run();

private:
    Warehouse& warehouse_;

    void showMainMenu() const;
    void addProduct();
    void editProduct();
    void deleteProduct();
    void searchMenu();
    void sortMenu();
    void analyticsMenu();
    void saveProducts() const;

    static int readInt(const std::string& prompt);
    static double readDouble(const std::string& prompt);
    static std::string readLine(const std::string& prompt);
    static void printProducts(const std::vector<Product>& products);
};
