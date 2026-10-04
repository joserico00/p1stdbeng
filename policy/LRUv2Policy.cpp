
#include "LRUv2Policy.h"

#include <algorithm>

namespace bufman {

void LRUv2Policy::init(std::size_t ) {
    nodes_.clear();
    order_.clear();
}

void LRUv2Policy::touch(std::size_t frame) {
    auto  node = nodes_.find(frame);
    if (node != nodes_.end()){
        order_.erase(node->second);
    }
    order_.push_front(frame);
    nodes_[frame]= order_.begin();

}

void LRUv2Policy::on_access(std::size_t frame) {
    touch(frame);
}

void LRUv2Policy::on_load(std::size_t frame) {
    touch(frame);
}

void LRUv2Policy::on_remove(std::size_t frame) {
    auto  node = nodes_.find(frame);
    if (node != nodes_.end()){
        order_.erase(node->second);
        nodes_.erase(frame);
    }
}

std::optional<std::size_t> LRUv2Policy::pick_victim(
        const std::vector<std::size_t>& candidates) const {
    for (auto it = order_.rbegin(); it != order_.rend(); ++it) {
        if (std::find(candidates.begin(), candidates.end(), *it) != candidates.end()) {
            return *it;
        }
    }
    return std::nullopt;
}

}
