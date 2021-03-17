#pragma once

#include "imgui.h"
#include <algorithm>

// So that ImVec2 can be used with std::map
namespace std {
template <>
struct less<ImVec2> {
    bool operator()(const ImVec2& lhs, const ImVec2& rhs) const
    {
        if (lhs.y == rhs.y)
            return (lhs.x < rhs.x);
        else
            return (lhs.y < rhs.y);
    }
};
}
