#include <iostream>
#include <variant>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <array>

struct T {
    T(std::string_view n) : name(n) {}
    T(const T& other) = default;
    std::string name;
};

int main() {
    std::vector<std::array<int, 4>> data(10, {1, 2, 3, 4});
    auto begin = data.data(), end = begin + data.size() * 4;
    for (auto it = begin; it!= end; it++) {
        std::cout << *it << " ";
    }
}
