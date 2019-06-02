#pragma once

#include <algorithm>
#include <vector>

template<class TFileStringTypeName>
void SortChunk(std::vector<TFileStringTypeName>& strings) {
    bool end = false;
    while (!end) {
        std::sort(strings.begin(), strings.end());

        size_t lastId = strings[0].GetId();

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
