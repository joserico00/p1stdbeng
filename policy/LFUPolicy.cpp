#include "LFUPolicy.h"
#include <algorithm>
#include <limits>
#include <vector>
namespace bufman {

void LFUPolicy::init(std::size_t pool_size) {
    slots_.clear();
    buckets_.clear();
    min_freq_ = 0;
}
void LFUPolicy::on_access(std::size_t frame) {
    auto  node = slots_.find(frame);
    if (node != slots_.end()) 
    {
        auto count=node->second.count;
        auto pos= node->second.pos;
        buckets_[count].erase(pos);
        if (buckets_[count].empty()){
            buckets_.erase(count);
            if(min_freq_== count){
                min_freq_++;
            }

        }
        buckets_[count+1].push_front(frame);
        node->second.count=count+1;
        node->second.pos=buckets_[count+1].begin();

  






    }
}
void LFUPolicy::on_load(std::size_t frame) {
    auto  node = slots_.find(frame);
    if (node != slots_.end()){
        auto count=node->second.count;
        auto pos= node->second.pos;

        buckets_[count].erase(pos);
        if (buckets_[count].empty()){
            buckets_.erase(count);
        }

        
    }
    buckets_[1].push_front(frame);
    slots_[frame] = Slot{1,buckets_[1].begin()};

    min_freq_=1;

}
void LFUPolicy::on_remove(std::size_t frame) {
    auto  node = slots_.find(frame);
    if (node != slots_.end()){
        auto count=node->second.count;
        auto pos= node->second.pos;
        buckets_[count].erase(pos);
        if (buckets_[count].empty()){
            buckets_.erase(count);

        }
        slots_.erase(frame);
        if (slots_.empty()){
            min_freq_=0;
        }
        else{
        min_freq_ = std::numeric_limits<std::size_t>::max();  
        for (const auto& [count, bucket] : buckets_) {
            min_freq_ = std::min(min_freq_, count);   
        }      
}


}




}
std::optional<std::size_t> LFUPolicy::pick_victim(
        const std::vector<std::size_t>& candidates) const {
        std::vector<std::size_t> counts;
        for (const auto& [count,bucket] : buckets_){
            counts.push_back(count);

        }
        std::sort(counts.begin(), counts.end());

        for(std::size_t count :counts){
            const auto& bucket = buckets_.find(count)->second;
            
            for (auto it = bucket.rbegin(); it != bucket.rend(); ++it) {
                if (std::find(candidates.begin(), candidates.end(), *it) != candidates.end()) {
                    return *it;
                }
            }
            
        }
        return std::nullopt;
}


}