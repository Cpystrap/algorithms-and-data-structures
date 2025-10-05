# OGR — Gardens

**Course:** Algorithms and Data Structures (lab), Assignment 2  
**Memory limit:** 256 MB  

## Problem (short)

A long alley is divided into `n` segments, each initially containing a tree of height `1`. There are `m` operations. Two types of operations are possible:

- `N a b k` — add `k` to the height of every tree in the contiguous segment `[a, b]` (1-based indices).
- `C a b` — query the subarray `[a, b]`: find the length of the longest contiguous subsequence inside `[a, b]` whose tree heights form a **non-decreasing** sequence.

Write a program that processes the `m` operations and answers every `C` query.

## Input

- First line: two integers `n m` (1 ≤ n ≤ 500000, 1 ≤ m ≤ 500000) — number of segments and number of operations.
- Next `m` lines: each line describes one operation, either:
  - `N a b k` with `1 ≤ a ≤ b ≤ n` and `1 ≤ k ≤ 1000` — range add, or
  - `C a b` with `1 ≤ a ≤ b ≤ n` — query for longest contiguous non-decreasing subarray inside `[a, b]`.

There is guaranteed to be at least one `C` operation.

## Output

For each `C` operation print a single line with one integer — the length of the longest contiguous non-decreasing subsequence entirely inside the queried interval.

## Example

**Input**
6 8
N 4 5 2
N 2 4 3
C 1 3
C 3 6
N 1 2 3
C 1 6
N 5 6 4
C 4 6

**Output**
3
2
2
2

**Explanation**

- After the first two `N` operations the heights are: `1, 4, 4, 6, 3, 1`.
  - Query `C 1 3` — `[1,4,4]` is entirely non-decreasing → answer `3`.
  - Query `C 3 6` — `[4,6,3,1]` has longest contiguous non-decreasing subsequence `4,6` → answer `2`.
- After `N 1 2 3` heights become `4,7,4,6,3,1`.
  - Query `C 1 6` — e.g. `4,7` is a best contiguous non-decreasing subsequence → `2`.
- After `N 5 6 4` the final query `C 4 6` also yields `2`.