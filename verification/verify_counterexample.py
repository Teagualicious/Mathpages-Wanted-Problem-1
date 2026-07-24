#!/usr/bin/env python3
"""Dependency-free exact-integer verifier for the MathPages counterexample.

Every displayed quantity is derived from the two decimal inputs X and Y.
No binary strings, reversals, supports, products, or histogram values are
stored as expected constants.
"""

from __future__ import annotations

from collections import Counter
from itertools import combinations
from typing import Iterable
import sys

DEFAULT_X = 353_492_451
DEFAULT_Y = 364_428_675


def canonical_binary(n: int) -> str:
    if n <= 0:
        raise ValueError("the MathPages formulation uses positive integers")
    return format(n, "b")


def reverse_binary_integer(n: int) -> int:
    return int(canonical_binary(n)[::-1], 2)


def support(n: int) -> tuple[int, ...]:
    return tuple(i for i in range(n.bit_length()) if (n >> i) & 1)


def distance_histogram(points: Iterable[int]) -> Counter[int]:
    pts = tuple(points)
    return Counter(b - a for a, b in combinations(pts, 2))


def format_support(points: tuple[int, ...]) -> str:
    return "{" + ", ".join(map(str, points)) + "}"


def format_histogram(hist: Counter[int]) -> str:
    return "{" + ", ".join(f"{d}: {hist[d]}" for d in sorted(hist)) + "}"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def parse_inputs(argv: list[str]) -> tuple[int, int]:
    if len(argv) == 1:
        return DEFAULT_X, DEFAULT_Y
    if len(argv) == 3:
        try:
            return int(argv[1], 10), int(argv[2], 10)
        except ValueError as exc:
            raise SystemExit(f"inputs must be decimal integers: {exc}") from exc
    raise SystemExit(f"usage: {argv[0]} [x y]")


def main() -> None:
    x, y = parse_inputs(sys.argv)
    bx = canonical_binary(x)
    by = canonical_binary(y)
    rx = reverse_binary_integer(x)
    ry = reverse_binary_integer(y)
    sx = support(x)
    sy = support(y)
    hx = distance_histogram(sx)
    hy = distance_histogram(sy)
    px = x * rx
    py = y * ry
    differing_distances = tuple(
        d for d in sorted(set(hx) | set(hy)) if hx.get(d, 0) != hy.get(d, 0)
    )

    require(len(bx) == len(by), "x and y must have the same bit length here")
    require(x != y, "x and y must be distinct")
    require(y != rx and x != ry, "x and y must not be reflections")
    require(px == py, "the reversal products are unequal")
    require(hx != hy, "the point-distance multisets are equal")
    require(hx.get(3, 0) != hy.get(3, 0), "distance 3 does not distinguish them")
    require(sum(hx.values()) == len(sx) * (len(sx) - 1) // 2,
            "x histogram has the wrong number of unordered pairs")
    require(sum(hy.values()) == len(sy) * (len(sy) - 1) // 2,
            "y histogram has the wrong number of unordered pairs")

    print("MathPages Wanted Problem 1 counterexample verifier (Python)")
    print(f"x                  = {x}")
    print(f"y                  = {y}")
    print(f"bit length         = {len(bx)}")
    print(f"binary(x)          = {bx}")
    print(f"binary(y)          = {by}")
    print(f"reverse(x)         = {rx}")
    print(f"binary(reverse(x)) = {canonical_binary(rx)}")
    print(f"reverse(y)         = {ry}")
    print(f"binary(reverse(y)) = {canonical_binary(ry)}")
    print(f"x*reverse(x)       = {px}")
    print(f"y*reverse(y)       = {py}")
    print(f"support(x)         = {format_support(sx)}")
    print(f"support(y)         = {format_support(sy)}")
    print(f"distance histogram(x) = {format_histogram(hx)}")
    print(f"distance histogram(y) = {format_histogram(hy)}")
    print(f"differing distances    = {list(differing_distances)}")
    print(f"multiplicity at d=3    = {hx.get(3, 0)} versus {hy.get(3, 0)}")
    print("COUNTEREXAMPLE VERIFIED")


if __name__ == "__main__":
    main()
