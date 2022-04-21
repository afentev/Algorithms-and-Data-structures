# Algorithms and Data structures

## Following stuff is already implemented:

## Math algorithms
 - Long arithmetic [(cp-algorithms)](https://cp-algorithms.com/algebra/big-integer.html)
### Prime numbers and factorization
 - Sieve of Eratosthenes [(wiki)](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)
 - Sieve of Eratosthenes with linear complexity (aka Euler's sieve) [(wiki)](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes#Euler's_sieve)
 - Pollard's rho algorithm [(wiki)](https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm)

## Sorting algorithms
 - Radix sort [(wiki)](https://en.wikipedia.org/wiki/Radix_sort)
 - Merge sort [(wiki)](https://en.wikipedia.org/wiki/Merge_sort)
 - Quick sort [(wiki)](https://en.wikipedia.org/wiki/Quicksort)
 - Insertion sort [(wiki)](https://en.wikipedia.org/wiki/Insertion_sort)
 - Selection sort [(wiki)](https://en.wikipedia.org/wiki/Selection_sort)
 - Bubble sort [(wiki)](https://en.wikipedia.org/wiki/Bubble_sort)

### Other algorithms related to sortings
 - Quickselect *(selection algorithm)* [(wiki)](https://en.wikipedia.org/wiki/Quickselect)
 - Quickselect with _O(n)_ worst-case complexity. Uses Median of medians [(wiki)](https://en.wikipedia.org/wiki/Median_of_medians)

## Heaps:
 - Binary heap [(wiki)](https://en.wikipedia.org/wiki/Binary_heap)
 - Binomial heap [(wiki)](https://en.wikipedia.org/wiki/Binomial_heap)
 - Pairing heap [(wiki)](https://en.wikipedia.org/wiki/Pairing_heap)
 - Leftist heap [(wiki)](https://en.wikipedia.org/wiki/Leftist_tree)
 - Skew heap [(wiki)](https://en.wikipedia.org/wiki/Skew_heap)

## Search Trees:
 - van Emde Boas tree [(wiki)](https://en.wikipedia.org/wiki/Van_Emde_Boas_tree)
 - Splay Tree [(wiki)](https://en.wikipedia.org/wiki/Splay_tree)
 - AVL Tree [(wiki)](https://en.wikipedia.org/wiki/AVL_tree)
 - B-tree [(wiki)](https://en.wikipedia.org/wiki/B-tree)
 - Skip list [(wiki)](https://en.wikipedia.org/wiki/Skip_list)
 - X-fast trie [(wiki)](https://en.wikipedia.org/wiki/X-fast_trie)
 - Y-fast trie [(wiki)](https://en.wikipedia.org/wiki/Y-fast_trie)

## Data Structures for RMQ and similar problems:
 - Segment tree [(wiki)](https://en.wikipedia.org/wiki/Segment_tree)
 - Segment tree with lazy propagation [(cp-algorithms)](https://cp-algorithms.com/data_structures/segment_tree.html#range-updates-lazy-propagation)
 - Bottom-up Segment tree (aka ZKW Segment tree) [(codeforces)](https://codeforces.com/blog/entry/18051)
 - Fenwick tree [(wiki)](https://en.wikipedia.org/wiki/Fenwick_tree)
 - Sparse table [(cp-algorithms)](https://cp-algorithms.com/data_structures/sparse-table.html)
 - Sparse table with binary lifting [(peltorator)](https://peltorator.ru/posts/sparse_table/)
 - Disjoint Sparse Table [(codechef)](https://discuss.codechef.com/t/tutorial-disjoint-sparse-table/17404)
 - Sqrt decomposition [(cp-algorithms)](https://cp-algorithms.com/data_structures/sqrt_decomposition.html)

## Algorithms on graphs:
 - Topological sort [(wiki)](https://en.wikipedia.org/wiki/Topological_sorting)
 - Counting the number of triangles in a graph [(cs.stanford.edu)](https://cs.stanford.edu/~rishig/courses/ref/l1.pdf)

### Shortest paths:
 - Floyd-Warshall algorithm [(wiki)](https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm)
 - Dijkstra's algorithm [(wiki)](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
 - Bellman-Ford algorithm [(wiki)](https://en.wikipedia.org/wiki/Bellman–Ford_algorithm)
 - Shortest Path Faster Algorithm [(wiki)](https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm) 
 - Pallottino's algorithm [(neerc.ifmo)](https://neerc.ifmo.ru/wiki/index.php?title=%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%9B%D0%B5%D0%B2%D0%B8%D1%82%D0%B0)
 - Levit's algorithm (aka Pape-Levit's or D'Esopo-Pape) [(cp-algorithms)](https://cp-algorithms.com/graph/desopo_pape.html)
 - 0-1 BFS [(cp-algorithms)](https://cp-algorithms.com/graph/01_bfs.html)
 - 1-k BFS [(lksh)](https://archive.lksh.ru/2018/august/B'/notes/07.pdf)
 - Dial's algorithm (1-k BFS) [(codeforces)](https://codeforces.com/blog/entry/88408?locale=en)

### Algorithms and Data structures for trees:
 - Heavy-light decomposition [(cp-algorithms)](https://cp-algorithms.com/graph/hld.html)
 - Prim's algorithm [(wiki)](https://en.wikipedia.org/wiki/Prim%27s_algorithm)
 - Kruskal's algorithm [(wiki)](https://en.wikipedia.org/wiki/Kruskal's_algorithm)

### Least common ancestor (LCA) and Level ancestor problem (LA):
 - Solving LCA via RMQ
 - Binary Lifting for LCA [(cp-algorithms)](https://cp-algorithms.com/graph/lca_binary_lifting.html)
 - Binary Lifting for LA [(wiki)](https://en.wikipedia.org/wiki/Level_ancestor_problem#Jump_pointer_algorithm)
 - Longest Path Decomposition (LA) [(wiki)](https://en.wikipedia.org/wiki/Level_ancestor_problem#Stage_1:_long-path_decomposition)
 - Ladder Decomposition (LA) [(wiki)](https://en.wikipedia.org/wiki/Level_ancestor_problem#Stage_2:_extending_the_long_paths_into_ladders)
 - Ladder Decomposition and Binary Lifting combined [(wiki)](https://en.wikipedia.org/wiki/Level_ancestor_problem#Stage_3:_combining_the_two_approaches)
 - The Macro-Micro-Tree Algorithm [(mit.edu)](http://courses.csail.mit.edu/6.897/spring05/lec/lec19.pdf)
 - Farach-Colton and Bender Algorithm [(cp-algorithms)](https://cp-algorithms.com/graph/lca_farachcoltonbender.html)

## Other data structures:
 - Disjoint set union (Union find) [(cp-algorithms)](https://cp-algorithms.com/data_structures/disjoint_set_union.html)
 - Persistent stack
 - Queue with minimum 
 - Hashed array tree [(wiki)](https://en.wikipedia.org/wiki/Hashed_array_tree)
 - Dynamic array with _Θ(1)_ _push_back_ complexity (no _O(n)_ worst-case)

## Other algorithms
 - Longest increasing subsequence [(wiki)](https://en.wikipedia.org/wiki/Longest_increasing_subsequence)
 - Longest common subsequence [(wiki)](https://en.wikipedia.org/wiki/Longest_common_subsequence_problem)
