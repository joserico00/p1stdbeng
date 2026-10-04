#include "PartGenerator.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <string>

namespace bufman {

namespace {



constexpr std::array<const char*, 6> kMaterial = {
    "iron", "gold", "cobalt", "wood", "steel","gem"
};
}


std::vector<Part> generate_parts(std::size_t count, int first_pid) {

    std::vector<Part> parts;
    parts.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        Part part{};
        part.part_id = first_pid + static_cast<int>(i);
        part.part_weight = 1 + (part.part_id % 180);

        const std::string name = "P" + std::to_string(part.part_id);
        name.copy(part.part_name, std::min(name.size(), sizeof(part.part_name) - 1));

        part.part_color = part.part_id % 6;

        const std::string  mat = kMaterial[i % kMaterial.size()];
        mat.copy(part.part_material, std::min(mat.size(), sizeof(part.part_material) - 1));
        part.part_price = part.part_id % 500;
        parts.push_back(part);
    }
    return parts;
}

}   