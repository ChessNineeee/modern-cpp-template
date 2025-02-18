//
// Created by wuzhengqi on 24-11-23.
//
#if PARALLEL
#include <execution>
#define SEQ std::execution::seq,
#define PAR std::execution::par,
#else
#define SEQ
#define PAR
#endif

#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Given binary_op as the actual binary operation:
//
// The result is non-deterministic if the binary_op is not associative or not commutative (such as floating-point addition).
// If any of the following values is not convertible to T, the program is ill-formed:
// binary_op(init, *first)
// binary_op(*first, init)
// binary_op(init, init)
// binary_op(*first, *first)
// If any of the following conditions is satisfied, the behavior is undefined:
// T is not MoveConstructible.
// binary_op modifies any element of [first, last).
// binary_op invalidates any iterator or subrange of [first, last].
int main()
{
    std::cout.imbue(std::locale("en_US.UTF-8"));
    std::cout << std::fixed << std::setprecision(1);

    auto eval = [](auto fun)
    {
        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto [name, result] = fun();
        const auto t2 = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::setw(28) << std::left << name << "sum: "
                  << result << '\t' << "time: " << ms.count() << " ms\n";
    };

    {
        const std::vector<double> v(100'000'007, 0.1);

        eval([&v]{ return std::pair{"std::accumulate (double)",
            std::accumulate(v.cbegin(), v.cend(), 0.0)}; });
        eval([&v]{ return std::pair{"std::reduce (seq, double)",
            std::reduce(SEQ v.cbegin(), v.cend())}; });
        eval([&v]{ return std::pair{"std::reduce (par, double)",
            std::reduce(PAR v.cbegin(), v.cend())}; });
    }

    {
        const std::vector<long> v(100'000'007, 1);

        eval([&v]{ return std::pair{"std::accumulate (long)",
            std::accumulate(v.cbegin(), v.cend(), 0l)}; });
        eval([&v]{ return std::pair{"std::reduce (seq, long)",
            std::reduce(SEQ v.cbegin(), v.cend())}; });
        eval([&v]{ return std::pair{"std::reduce (par, long)",
            std::reduce(PAR v.cbegin(), v.cend())}; });
    }
}