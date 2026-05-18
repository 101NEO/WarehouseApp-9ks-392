#include "FileIO.h"

#include <fstream>
#include <sstream>

std::vector<Product> FileIO::loadProducts(const std::string& path) {
    std::vector<Product> products;
    std::ifstream file(path);
    if (!file.is_open()) {
        return products;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        const std::vector<std::string> parts = split(line, ',');
        if (parts.size() != 5) {
            continue;
        }

        try {
            const int id = std::stoi(parts[0]);
            const double price = std::stod(parts[3]);
            const int quantity = std::stoi(parts[4]);
            products.emplace_back(id, parts[1], parts[2], price, quantity);
        } catch (...) {
            continue;
        }
    }

    return products;
}

bool FileIO::saveProducts(const std::string& path, const std::vector<Product>& products) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    for (const Product& product : products) {
        file << product.getId() << ','
             << product.getName() << ','
             << product.getCategory() << ','
             << product.getPrice() << ','
             << product.getQuantity() << '\n';
    }

    return true;
}

std::vector<std::string> FileIO::split(const std::string& line, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream stream(line);
    std::string part;
    while (std::getline(stream, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}
