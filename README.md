# A counterexample to MathPages Wanted Problem 1

This repository is a public-review and reproducibility package for the following pair:

```text
x = 353492451
y = 364428675
```

It contains a self-contained proof, two dependency-free exact-integer verifiers, the original exhaustive-search source and raw output unchanged, an independent audit search, provenance, hashes, CI, citation metadata, and release automation.

## Claim status

| Claim | Status |
|---|---|
| **Counterexample validity** | **Established by the proof and independently implemented Python and JavaScript BigInt verifiers.** |
| **29-bit minimality for the MathPages problem as stated** | **False.** Auditing found that the original search filters by equal popcount. The included all-collision audit finds the 21-bit pair `1100125, 1269125`. |
| **Historical novelty** | **Not established or claimed.** Exact-string web searches are not a literature review; priority requires broader review and confirmation from the MathPages maintainer. |

The 29-bit pair remains useful because both point sets contain 16 points, so its failure is not merely a difference in cardinality.

## The problem

[MathPages Wanted List, Problem 1](https://www.mathpages.com/home/mwlist.htm) asks readers to

> “prove or disprove that the equality xx' = yy' implies that x and y have the same multi-set of point-to point distances.”

For a positive integer `n`, let `n'` be the integer obtained by reversing the canonical binary digits of `n`. A `1` in bit position `i` represents a point at coordinate `i`. The distance multiset contains one entry `|i-j|` for each unordered pair of distinct represented points.

Background: [Generating Functions for Point Set Distances](https://www.mathpages.com/home/kmath390/kmath390.htm).

## Counterexample, derived from the decimal inputs

| Quantity | Derived value |
|---|---:|
| `x` | `353492451` |
| binary `x` | `10101000100011101110111100011` |
| `x'` | `418869525` |
| binary `x'` | `11000111101110111000100010101` |
| `y` | `364428675` |
| binary `y` | `10101101110001011110110000011` |
| `y'` | `406299573` |
| binary `y'` | `11000001101111010001110110101` |
| `xx' = yy'` | `148067215041455775` |

The products are exactly

```text
353492451 * 418869525 = 148067215041455775
364428675 * 406299573 = 148067215041455775
```

The supports of the `1`-bits are

```text
Sx = {0, 1, 5, 6, 7, 8, 10, 11, 12, 14, 15, 16, 20, 24, 26, 28}
Sy = {0, 1, 7, 8, 10, 11, 12, 13, 15, 19, 20, 21, 23, 24, 26, 28}
```

At distance 3, `Sx` has the pairs

```text
(5,8), (7,10), (8,11), (11,14), (12,15)
```

and `Sy` has

```text
(7,10), (8,11), (10,13), (12,15), (20,23), (21,24), (23,26)
```

Thus distance 3 has multiplicity `5` for `x` and `7` for `y`. Therefore the distance multisets differ even though `xx' = yy'`.

The complete exact histograms are printed by both verifiers. Their differing distances are `[3, 4, 5, 6, 7, 10, 11, 12, 13, 14]`.

## One-command verification

Requirements: Python 3, Node.js with `BigInt`, GNU `make`, and `sha256sum`.

```bash
make verify
```

Without `make`:

```bash
python3 verification/verify_counterexample.py
node verification/verify_counterexample.js
sha256sum -c search/SHA256SUMS.txt
```

Both verifiers accept an optional decimal pair, so the audit's 21-bit pair can also be checked directly:

```bash
python3 verification/verify_counterexample.py 1100125 1269125
node verification/verify_counterexample.js 1100125 1269125
```

## Proof

The concise mathematical note is available as:

- [`proof/counterexample.tex`](proof/counterexample.tex)
- [`proof/counterexample.pdf`](proof/counterexample.pdf)

The proof does not depend on the search program.

## Search artifacts and audit

- [`search/search_binary_exactlen.cpp`](search/search_binary_exactlen.cpp) is the original search source, preserved byte-for-byte.
- [`search/search_len29_out.txt`](search/search_len29_out.txt) is its original raw standard output, preserved byte-for-byte.
- [`search/original_rerun_2_29.txt`](search/original_rerun_2_29.txt) records a local rerun.
- [`search/audit_exhaustive.cpp`](search/audit_exhaustive.cpp) independently compares **all** product collisions rather than grouping by popcount.
- [`search/audit_rerun_2_29.txt`](search/audit_rerun_2_29.txt) records the audit output, including the 21-bit counterexample.
- [`search/SEARCH_METHOD.md`](search/SEARCH_METHOD.md) defines the search space, normalization, reflection handling, collision logic, commands, environment, runtime, coverage argument, and limitations.

The original program's equal-popcount filter is appropriate only for the narrower question of equal-cardinality point sets. It is not complete for the MathPages statement as written. Accordingly, this repository makes no 29-bit minimality claim.

## Reproducibility and provenance

- [`search/SHA256SUMS.txt`](search/SHA256SUMS.txt) authenticates the critical source, output, proof, and verifier files.
- [`provenance/PROVENANCE.md`](provenance/PROVENANCE.md) records how the pair and package were produced and checked.
- [`provenance/conversation-transcript-redacted.md`](provenance/conversation-transcript-redacted.md) contains a privacy-preserving transcript summary.
- [`.github/workflows/verify.yml`](.github/workflows/verify.yml) runs both verifiers and integrity checks on pushes and pull requests.

## Citation and license

GitHub reads [`CITATION.cff`](CITATION.cff) to provide citation metadata. The repository is released under the [MIT License](LICENSE), permitting copying, execution, modification, and redistribution subject to its terms.
