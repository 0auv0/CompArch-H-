#include "mem/cache/replacement_policies/nmru_rp.hh"

#include <cassert>
#include <memory>
#include <random>

#include "params/NMRURP.hh"
#include "sim/cur_tick.hh"

namespace gem5
{

namespace replacement_policy
{

NMRU::NMRU(const Params &p)
  : Base(p)
{
}

void
NMRU::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
{
    // Reset last touch timestamp
    std::static_pointer_cast<NMRUReplData>(
        replacement_data)->lastTouchTick = Tick(0);
}

void
NMRU::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Update last touch timestamp
    std::static_pointer_cast<NMRUReplData>(
        replacement_data)->lastTouchTick = curTick();
}

void
NMRU::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Set last touch timestamp
    std::static_pointer_cast<NMRUReplData>(
        replacement_data)->lastTouchTick = curTick();
}

ReplaceableEntry*
NMRU::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    // Visit all candidates to find victim
    ReplaceableEntry* last = candidates[0];
    for (const auto& candidate : candidates) {
        // Update victim entry if necessary
        if (std::static_pointer_cast<NMRUReplData>(
                    candidate->replacementData)->lastTouchTick >
                std::static_pointer_cast<NMRUReplData>(
                    last->replacementData)->lastTouchTick) {
            last = candidate;
        }
    }
    ReplaceableEntry *victim = candidates[0];

    if(candidates.size() > 1){
        int victimIndex;
        std::random_device rd;  // 用于获取种子
        std::mt19937 gen(rd()); // 使用Mersenne Twister算法生成随机数
        std::uniform_int_distribution<> dis(0, candidates.size()-1); // 定义分布范围
        do{
            victimIndex = dis(gen); 
        }while(candidates[victimIndex] == last);
        victim = candidates[victimIndex];
    }

    for (const auto& candidate : candidates) {
        if (std::static_pointer_cast<NMRUReplData>(
        candidate->replacementData)->lastTouchTick == Tick(0)) {
            victim = candidate;
            break;
        }
    }

    return victim;
}

std::shared_ptr<ReplacementData>
NMRU::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new NMRUReplData());
}

} // namespace replacement_policy
} // namespace gem5
