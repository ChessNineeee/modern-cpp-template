//
// Created by wuzhengqi on 24-11-23.
//

#include <algorithm>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

using namespace std::literals;

bool contains(const auto& cont, std::string_view s)
{
    // str.find() (or str.contains(), since C++23) can be used as well
    return std::search(cont.begin(), cont.end(), s.begin(), s.end()) != cont.end();
}

int main()
{
    const auto str{"why waste time learning, when ignorance is instantaneous?"sv};
    assert(contains(str, "learning"));
    assert(not contains(str, "lemming"));

    const std::vector vec(str.begin(), str.end());
    assert(contains(vec, "learning"));
    assert(not contains(vec, "leaning"));

    // The C++17 overload with searchers demo:
    constexpr auto quote
    {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
        "do eiusmod tempor incididunt ut labore et dolore magna aliqua"sv
    };

    for (const auto word : {"pisci"sv, "Pisci"sv})
    {
        std::cout << "The string " << std::quoted(word) << ' ';
        const std::boyer_moore_searcher searcher(word.begin(), word.end());
        const auto it = std::search(quote.begin(), quote.end(), searcher);
        if (it == quote.end())
            std::cout << "not found\n";
        else
            std::cout << "found at offset " << std::distance(quote.begin(), it) << '\n';
    }
}