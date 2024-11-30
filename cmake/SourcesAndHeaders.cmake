set(sources
        src/tmp.cpp
)

set(exe_sources
        src/main.cpp
        ${sources}
)

set(headers
        include/project/tmp.hpp
)

set(test_sources
        src/tmp_test.cpp
        src/solution.cpp
        src/asio_proxy.cpp
        src/stl/algorithms/heap/make_heap_and_pop_heap.cpp
        src/stl/algorithms/heap/push_heap.cpp
        src/stl/algorithms/heap/sort_heap.cpp
        src/stl/algorithms/sort/sort.cpp
        src/stl/algorithms/sort/partial_sort.cpp
        src/stl/algorithms/sort/nth_element.cpp
        src/stl/algorithms/sort/max_element.cpp
        src/stl/algorithms/sort/min_element.cpp
        src/stl/algorithms/sort/min_max_element.cpp
        src/stl/algorithms/sort/inplace_merge.cpp
        src/stl/algorithms/partition/partition.cpp
        src/stl/algorithms/partition/partition_point.cpp
        src/stl/algorithms/rune/stable_sort.cpp
        src/stl/algorithms/rune/stable_partition.cpp
        src/stl/algorithms/rune/is_sorted.cpp
        src/stl/algorithms/rune/is_sorted_until.cpp
        src/stl/algorithms/rune/is_partitioned.cpp
        src/stl/algorithms/rune/is_heap.cpp
        src/stl/algorithms/rune/is_heap_until.cpp
        src/stl/algorithms/queries/count.cpp
        src/stl/algorithms/queries/accumulate.cpp
        src/stl/algorithms/queries/reduce.cpp
        src/stl/algorithms/queries/transform_reduce.cpp
        src/stl/algorithms/queries/partial_sum.cpp
        src/stl/algorithms/queries/inclusive_scan.cpp
        src/stl/algorithms/queries/exclusive_scan.cpp
        src/stl/algorithms/queries/transform_inclusive_scan.cpp
        src/stl/algorithms/queries/transform_exclusive_scan.cpp
        src/stl/algorithms/queries/inner_product.cpp
        src/stl/algorithms/queries/adjacent_difference.cpp
        src/stl/algorithms/properties/all_any_none_of.cpp
        src/stl/algorithms/properties/equal.cpp
        src/stl/algorithms/properties/is_permutation.cpp
        src/stl/algorithms/search/find_xxx.cpp
        src/stl/algorithms/search/adjacent_find.cpp
        src/stl/algorithms/search/equal_range.cpp
        src/stl/algorithms/search/lower_bound.cpp
        src/stl/algorithms/search/upper_bound.cpp
        src/stl/algorithms/search/binary_search.cpp
        src/stl/algorithms/search/search.cpp
        src/stl/algorithms/search/find_end.cpp
        src/stl/algorithms/search/find_first_of.cpp
        src/stl/algorithms/sorted_collections/collection_difference.cpp
        src/stl/algorithms/sorted_collections/collection_intersection.cpp
        src/stl/algorithms/sorted_collections/collection_union.cpp
        src/stl/algorithms/sorted_collections/collection_symmetric_difference.cpp
        src/stl/algorithms/sorted_collections/collection_includes.cpp
        src/stl/algorithms/move/erase_if.cpp
        src/stl/algorithms/move/remove.cpp
        src/stl/algorithms/move/replace_if.cpp
        src/stl/algorithms/move/unique.cpp
        src/state_machine/modern_cpp_way.cpp
        src/state_machine/tcp_connection_state.cpp
        src/container/container.cpp
        src/memalloc/allocator.cpp
        src/monkey/lexer.cpp
)
