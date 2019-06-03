#pragma once

#include <algorithm>
#include <vector>

template<class TFileStringTypeName>
void SortChunk(std::vector<TFileStringTypeName>& strings) {
    bool end = false;
    while (!end) {
        size_t lastId = 0;
        size_t lastInd = 0;
        for (size_t i = 1; i < strings.size(); ++i) {
            if (lastId == strings[i].GetId()) {
                continue;
            }
            std::sort(strings.begin() + lastInd, strings.begin() + i);
            lastInd = i;
            lastId = strings[i].GetId();
        }
        std::sort(strings.begin() + lastInd, strings.end());

        lastId = strings[0].GetId();

        for (size_t i = 1; i < strings.size(); ++i) {
            if (strings[i - 1] < strings[i]) {
                strings[i - 1].SetId(lastId);
                ++lastId;
            } else {
                strings[i - 1].SetId(lastId);
            }
        }

        if (strings.size() > 1) {
            strings[strings.size() - 1].SetId(lastId);
        }

        if (strings[strings.size() - 1].GetId() == strings.size() - 1) {
            break;
        }

        end = true;
        for (auto& str: strings) {
            str.GetNextLetter();
            if (!str.IsEnd()) {
                end = false;
            }
        }
    }
}
