// MIT License
//
// Copyright (c) 2018 Michał Siedlaczek
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//! \file       static-ordering-eval.hpp
//! \author     Michal Siedlaczek
//! \copyright  MIT License

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct entry {
    long id;
    long rank;
};

struct rank_comparator {
    bool operator()(entry lhs, entry rhs) const { return lhs.rank > rhs.rank; }
};

std::unordered_map<std::string, long> load_idmap(std::istream& in)
{
    std::unordered_map<std::string, long> idmap;
    long current_id = 0;
    std::string title;
    while (std::getline(in, title)) { idmap[title] = current_id++; }
    return idmap;
}

std::vector<entry> load_judgements(std::istream& in,
    std::unordered_map<std::string, long>& idmap)
{
    std::unordered_map<long, long> judgements;
    std::string title;
    std::string rank;
    while (in >> title) {
        if (in >> rank) {
            if (auto pos = idmap.find(title); pos != idmap.end()) {
                long id = pos->second;
                judgements[id] = std::max(judgements[id], std::stol(rank));
            }
        } else {
            throw std::runtime_error("corrupted judgements file");
        }
    }
    std::vector<entry> vec;
    for (const auto& [id, rank] : judgements) { vec.push_back({id, rank}); }
    std::sort(vec.begin(), vec.end(), rank_comparator{});
    return vec;
}

double eval_ordering(std::istream& jin, std::istream& sin)
{
    long pairs_judged = 0;
    long pairs_intersection = 0;
    auto idmap = load_idmap(sin);
    auto judgements = load_judgements(jin, idmap);
    for (auto left = judgements.begin();
         left != std::prev(judgements.end());
         left++)
    {
        auto right = std::next(left);
        for (; right != judgements.end() && left->rank == right->rank; right++)
        { /* Skip same rank. */ }
        for (; right != judgements.end(); right++) {
            pairs_judged++;
            pairs_intersection += left->id < right->id ? 1 : 0;
        }
    }
    return (double)pairs_intersection / pairs_judged;
}
