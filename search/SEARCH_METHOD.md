# Search methodology and audit

## Scope

This directory contains two different programs:

1. `search_binary_exactlen.cpp` is the **original source**, preserved unchanged. It produced `search_len29_out.txt`.
2. `audit_exhaustive.cpp` is an independent audit implementation written for this repository after reviewing the original source.

The self-contained counterexample proof does not depend on either program.

## Encoded point sets

For an exact binary length `L`, an integer `n` represents the support of its `1`-bits. Both programs enumerate exactly

```text
2^(L-1) <= n < 2^L, with n odd.
```

Thus the most significant and least significant bits are both `1`. This is the standard translation normalization for a nonempty finite point set on an integer line: subtract the minimum coordinate so the least coordinate is 0, and let `L = max(S)-min(S)+1`. Every normalized set of span `L-1` appears exactly once.

The one-point set has `L=1` and cannot form a counterexample with a distinct normalized set. For each `L >= 2`, the search therefore contains `2^(L-2)` configurations.

## Binary reversal and reflection

`reverse_bits(n,L)` reverses exactly `L` bits. Because both endpoint bits are `1`, this is also canonical binary digit reversal. Geometrically it reflects the normalized point set across the midpoint of `[0,L-1]`.

Neither program removes reflected copies during generation. When comparing a collision pair, both skip the direct pair `n` and `reverse_bits(n,L)`, because reflection preserves all pairwise distances and cannot be a counterexample.

## Distance signature

For each distance `d=1,...,L-1`,

```cpp
popcount(n & (n >> d))
```

is the number of unordered support pairs separated by `d`. The vector of these counts is therefore the complete distance-multiset histogram.

## Original program

The original program:

1. generates every normalized exact-`L` integer;
2. computes `n * reverse_bits(n,L)` using `uint64_t`;
3. sorts records by product, then popcount, then integer value;
4. considers product-collision classes;
5. subdivides each collision class by equal popcount;
6. compares non-reflection pairs only inside one equal-popcount subgroup.

This is exhaustive for the **narrower equal-cardinality question**, because popcount is the number of represented points. It is **not exhaustive for the MathPages problem as stated**, because two sets with different point counts automatically have different distance-multiset cardinalities and may still have equal reversal products.

That limitation explains why the original program reports no counterexample through 28 bits and finds the included equal-popcount pair at 29 bits.

### Original local rerun

Compiler command:

```bash
g++ -O3 -std=c++17 -DNDEBUG -Wall -Wextra -Wpedantic   search/search_binary_exactlen.cpp -o search_binary_exactlen
```

For every `L` from 2 through 29:

```bash
/usr/bin/time -f 'elapsed=%e user=%U sys=%S maxrss_kb=%M exit=%x'   ./search_binary_exactlen "$L"
```

Environment reported during the rerun:

- CPU allocation: 5 virtual CPUs, AMD EPYC 9V74
- memory: 5.9 GiB available to the container
- OS/kernel: Linux x86-64, kernel 6.12.13
- compiler: g++ 14.2.0

Selected timings:

- `L=28`: 67,108,864 records; 7.87 s elapsed; 855,280 KiB maximum RSS
- `L=29`: 134,217,728 records; 16.08 s elapsed; 1,707,248 KiB maximum RSS
- all invocations `L=2,...,29`: 31.55 s total elapsed

The rerun's `L=29` standard output is byte-for-byte identical to `search_len29_out.txt`. See `original_rerun_2_29.txt` for complete output and timings.

## Independent all-collision audit

`audit_exhaustive.cpp` removes the popcount subdivision. It sorts by product and compares every non-reflection pair in each product collision class. Thus different-cardinality pairs are included.

Compiler command:

```bash
g++ -O3 -std=c++20 -DNDEBUG -Wall -Wextra -Wpedantic   search/audit_exhaustive.cpp -o audit_exhaustive
```

The audit reports no counterexample for exact lengths 2 through 20 and then reports at `L=21`:

```text
x = 1100125
y = 1269125
reverse(x) = 1528417
reverse(y) = 1324889
x*reverse(x) = y*reverse(y) = 1681449752125
popcount(x) = 10
popcount(y) = 11
```

The optional-input modes of both small verifiers reproduce this pair from its decimal inputs.

## Coverage and claim discipline

For each fixed `L`, the loop bounds enumerate every normalized support with exact length `L`. Sorting places all equal products contiguously. The audit program compares every unordered pair in each collision class except a set and its direct reflection, and its distance signature is complete. Consequently, its run through `L=20` is a reproducible computational coverage claim for all normalized configurations through 20 bits.

Nevertheless, this repository does not present computational minimality as a peer-reviewed theorem. The search code, compiler assumptions, normalization argument, logs, and hashes are published precisely so reviewers can audit or rerun them.

What can be stated without relying on exhaustive search is simpler: the 29-bit pair proved in `proof/counterexample.pdf` is a valid counterexample, and the explicit 21-bit audit pair shows that **29-bit minimality is false** for the original MathPages statement.
