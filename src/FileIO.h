#pragma once

#include "Product.h"

#include <string>
#include <vector>

class FileIO {
public:
    static std::vector<Product> loadProducts(const std::string& path);
    static bool saveProducts(const std::string& path, const std::vector<Product>& products);

private:
    static std::vector<std::string> split(const std::string& line, char delimiter);
};
