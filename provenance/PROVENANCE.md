# Provenance

## Record

- Date prepared: 2026-07-23
- Product: OpenAI ChatGPT
- Model: GPT-5.6 Pro
- Repository owner alias: `Teagualicious`
- Target repository: `Teagualicious/Mathpages-Wanted-Problem-1`

No email address, physical location, authentication material, or other private personal information is included.

## Original prompt

> https://mathworld.wolfram.com/UnsolvedProblems.html  
> Pick an unsolved math problem and solve it by providing a counterexample.

The user later required that the result not be copied from existing work and requested a public peer-review and reproducibility repository.

## What the model generated

During the conversation, GPT-5.6 Pro generated the 29-bit pair

```text
x = 353492451
y = 364428675
```

and the original exhaustive source `search_binary_exactlen.cpp`. The raw output `search_len29_out.txt` was produced by that source. Those two search artifacts are preserved byte-for-byte in this repository.

For this package, the model also generated:

- the self-contained mathematical proof;
- independent Python and JavaScript BigInt verifiers;
- the independent all-collision audit source;
- search-method documentation;
- CI and release workflows;
- citation, license, hashes, and this provenance record.

## What was checked locally

Before committing, the following were run locally:

- both exact-integer verifiers on the 29-bit pair;
- both verifiers on the audit's 21-bit pair;
- SHA-256 integrity checks;
- compilation of both C++ search programs with warnings enabled;
- the original program for every exact length 2 through 29;
- the independent audit program for every exact length 2 through 29;
- byte comparison of the reproduced and original 29-bit raw output;
- LaTeX compilation of the proof and PDF rendering inspection.

The two small verifiers are independent implementations in different languages. This is not the same as independent human peer review.

## Correction produced by audit

The original search groups product collisions by equal popcount. That restriction was not made explicit in the earlier conversational claim of 29-bit minimality. The audit program removes this restriction and finds a 21-bit pair with different popcounts. Therefore:

- the 29-bit counterexample remains valid;
- 29-bit minimality for the MathPages statement is false;
- the original source may support only a narrower equal-cardinality search claim, subject to review.

This correction is retained prominently rather than hidden.

## Historical novelty

No historical novelty or priority claim is made. Exact searches for the decimal pair, common product, and binary strings returned no indexed result during preparation, but such searches are not a comprehensive literature review. Establishing priority requires review of relevant mathematical literature, archives, correspondence, and confirmation from the MathPages maintainer.

## Transcript

A privacy-preserving summary is in [`conversation-transcript-redacted.md`](conversation-transcript-redacted.md). It omits account data, credentials, and unrelated conversation details.
