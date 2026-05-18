#include "Warehouse.h"

#include <algorithm>
#include <cctype>
#include <map>

bool Warehouse::addProduct(const Product& product, std::string& error) {
    if (product.getId() <= 0) {
        error = "ID должен быть положительным числом.";
        return false;
    }
    if (!isIdUnique(product.getId())) {
        error = "Товар с таким ID уже существует.";
        return false;
    }
    if (isBlank(product.getName()) || isBlank(product.getCategory())) {
        error = "Название и категория не должны быть пустыми.";
        return false;
    }
    if (product.getPrice() < 0 || product.getQuantity() < 0) {
        error = "Цена и количество не могут быть отрицательными.";
        return false;
    }

    products_.push_back(product);
    return true;
}

bool Warehouse::updateProduct(int id, const std::string& name, const std::string& category, double price, int quantity, std::string& error) {
    Product* product = findById(id);
    if (product == nullptr) {
        error = "Товар с таким ID не найден.";
        return false;
    }
    if (isBlank(name) || isBlank(category)) {
        error = "Название и категория не должны быть пустыми.";
        return false;
    }
    if (price < 0 || quantity < 0) {
        error = "Цена и количество не могут быть отрицательными.";
        return false;
    }

    product->setName(name);
    product->setCategory(category);
    product->setPrice(price);
    product->setQuantity(quantity);
    return true;
}

bool Warehouse::removeById(int id) {
    const auto oldSize = products_.size();
    products_.erase(std::remove_if(products_.begin(), products_.end(), [id](const Product& product) {
        return product.getId() == id;
    }), products_.end());
    return products_.size() != oldSize;
}

int Warehouse::removeByName(const std::string& name) {
    const auto oldSize = products_.size();
    products_.erase(std::remove_if(products_.begin(), products_.end(), [&name](const Product& product) {
        return equalsIgnoreCase(product.getName(), name);
    }), products_.end());
    return static_cast<int>(oldSize - products_.size());
}

Product* Warehouse::findById(int id) {
    auto it = std::find_if(products_.begin(), products_.end(), [id](const Product& product) {
        return product.getId() == id;
    });
    return it == products_.end() ? nullptr : &(*it);
}

const Product* Warehouse::findById(int id) const {
    auto it = std::find_if(products_.begin(), products_.end(), [id](const Product& product) {
        return product.getId() == id;
    });
    return it == products_.end() ? nullptr : &(*it);
}

std::vector<Product> Warehouse::searchByName(const std::string& query) const {
    std::vector<Product> result;
    const std::string needle = lowerCopy(query);
    std::copy_if(products_.begin(), products_.end(), std::back_inserter(result), [&needle](const Product& product) {
        return lowerCopy(product.getName()).find(needle) != std::string::npos;
    });
    return result;
}

std::vector<Product> Warehouse::filterByCategory(const std::string& category) const {
    std::vector<Product> result;
    std::copy_if(products_.begin(), products_.end(), std::back_inserter(result), [&category](const Product& product) {
        return equalsIgnoreCase(product.getCategory(), category);
    });
    return result;
}

std::vector<Product> Warehouse::filterByPriceRange(double minPrice, double maxPrice) const {
    std::vector<Product> result;
    std::copy_if(products_.begin(), products_.end(), std::back_inserter(result), [minPrice, maxPrice](const Product& product) {
        return product.getPrice() >= minPrice && product.getPrice() <= maxPrice;
    });
    return result;
}

std::vector<Product> Warehouse::sorted(SortField field, bool ascending) const {
    std::vector<Product> result = products_;
    std::sort(result.begin(), result.end(), [field, ascending](const Product& left, const Product& right) {
        int comparison = 0;
        switch (field) {
        case SortField::Price:
            comparison = (left.getPrice() > right.getPrice()) - (left.getPrice() < right.getPrice());
            break;
        case SortField::Quantity:
            comparison = (left.getQuantity() > right.getQuantity()) - (left.getQuantity() < right.getQuantity());
            break;
        case SortField::Name:
            comparison = lowerCopy(left.getName()).compare(lowerCopy(right.getName()));
            break;
        }
        return ascending ? comparison < 0 : comparison > 0;
    });
    return result;
}

double Warehouse::totalInventoryValue() const {
    double sum = 0.0;
    for (const Product& product : products_) {
        sum += product.totalCost();
    }
    return sum;
}

std::vector<Product> Warehouse::lowStock(int limit) const {
    std::vector<Product> result;
    std::copy_if(products_.begin(), products_.end(), std::back_inserter(result), [limit](const Product& product) {
        return product.getQuantity() <= limit;
    });
    return result;
}

std::vector<Product> Warehouse::topExpensive(int count) const {
    std::vector<Product> result = sorted(SortField::Price, false);
    if (static_cast<int>(result.size()) > count) {
        result.resize(count);
    }
    return result;
}

const std::vector<Product>& Warehouse::all() const {
    return products_;
}

void Warehouse::replaceAll(const std::vector<Product>& products) {
    products_ = products;
}

bool Warehouse::isIdUnique(int id) const {
    return findById(id) == nullptr;
}

std::string Warehouse::lowerCopy(const std::string& text) {
    static const std::map<std::string, std::string> russianLetters = {
        {"А", "а"}, {"Б", "б"}, {"В", "в"}, {"Г", "г"}, {"Д", "д"}, {"Е", "е"}, {"Ё", "ё"}, {"Ж", "ж"},
        {"З", "з"}, {"И", "и"}, {"Й", "й"}, {"К", "к"}, {"Л", "л"}, {"М", "м"}, {"Н", "н"}, {"О", "о"},
        {"П", "п"}, {"Р", "р"}, {"С", "с"}, {"Т", "т"}, {"У", "у"}, {"Ф", "ф"}, {"Х", "х"}, {"Ц", "ц"},
        {"Ч", "ч"}, {"Ш", "ш"}, {"Щ", "щ"}, {"Ъ", "ъ"}, {"Ы", "ы"}, {"Ь", "ь"}, {"Э", "э"}, {"Ю", "ю"},
        {"Я", "я"}
    };

    std::string result;
    for (std::size_t i = 0; i < text.size();) {
        const unsigned char ch = static_cast<unsigned char>(text[i]);
        if (ch < 128) {
            result.push_back(static_cast<char>(std::tolower(ch)));
            ++i;
            continue;
        }

        if (i + 1 < text.size()) {
            const std::string letter = text.substr(i, 2);
            const auto it = russianLetters.find(letter);
            result += it == russianLetters.end() ? letter : it->second;
            i += 2;
        } else {
            result.push_back(text[i]);
            ++i;
        }
    }
    return result;
}

bool Warehouse::equalsIgnoreCase(const std::string& left, const std::string& right) {
    return lowerCopy(left) == lowerCopy(right);
}

bool Warehouse::isBlank(const std::string& text) {
    return std::all_of(text.begin(), text.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
}
