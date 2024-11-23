//
// Created by wuzhengqi on 24-11-23.
//
#if PARALLEL
#include <execution>
#define PAR std::execution::par,
#else
#define PAR
#endif

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <numeric>
#include <vector>

//  Applies transform to each pair of elements from the ranges [first1, last1) and the range of std::distance(first1, last1) elements starting from first2 and reduces the results (possibly permuted and aggregated in unspecified manner) along with the initial value init over reduce.
//  The result is non-deterministic if the reduce is not associative or not commutative (such as floating-point addition).
//  If any of the following values is not convertible to T, the program is ill-formed:
// reduce(init, init)
// reduce(init, transform(*first1, *first2))
// reduce(transform(*first1, *first2), init)
// reduce(transform(*first1, *first2), transform(*first1, *first2))
//  Given last2 as the std::distance(first1, last1)th next iterator of first2, if any of the following conditions is satisfied, the behavior is undefined:
// T is not MoveConstructible.
// transform or reduce modifies any element of [first1, last1) or [first2, last2).
// transform or reduce invalidates any iterator or subrange of [first1, last1] or [first2, last2].
// 5) Applies transform to each element in the range [first, last) and reduces the results (possibly permuted and aggregated in unspecified manner) along with the initial value init over reduce.
//  The result is non-deterministic if the reduce is not associative or not commutative (such as floating-point addition).
//  If any of the following values is not convertible to T, the program is ill-formed:
// reduce(init, init)
// reduce(init, transform(*first))
// reduce(transform(*first), init)
// reduce(transform(*first), transform(*first))
//  If any of the following conditions is satisfied, the behavior is undefined:
// T is not MoveConstructible.
// transform or reduce modifies any element of [first, last).
// transform or reduce invalidates any iterator or subrange of [first, last].
// to parallelize non-associate accumulative operation, you'd better choose
// transform_reduce instead of reduce; e.g., a + b * b != b + a * a
void print_sum_squared(long const num)
{
    std::cout.imbue(std::locale{"en_US.UTF8"});
    std::cout << "num = " << num << '\n';

    // create an immutable vector filled with pattern: 1,2,3,4, 1,2,3,4 ...
    const std::vector<long> v{[n = num * 4] {
        std::vector<long> v;
        v.reserve(n);
        std::generate_n(std::back_inserter(v), n,
            [i = 0]() mutable { return 1 + i++ % 4; });
        return v;
    }()};

    auto squared_sum = [](auto sum, auto val) { return sum + val * val; };

    auto sum1 = std::accumulate(v.cbegin(), v.cend(), 0L, squared_sum);
    std::cout << "accumulate(): " << sum1 << '\n';

    auto sum2 = std::reduce(PAR v.cbegin(), v.cend(), 0L, squared_sum);
    std::cout << "reduce(): " << sum2 << '\n';

    auto sum3 = std::transform_reduce(PAR v.cbegin(), v.cend(), 0L, std::plus{},
                                      [](auto val) { return val * val; });
    std::cout << "transform_reduce(): " << sum3 << "\n\n";
}

int main()
{
    print_sum_squared(1);
    print_sum_squared(1'000);
    print_sum_squared(1'000'000);
}