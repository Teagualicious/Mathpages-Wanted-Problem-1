#!/usr/bin/env node
"use strict";

/*
 * Dependency-free BigInt verifier. Every displayed quantity is derived from
 * the two decimal inputs X and Y; no expected binary strings, reversals,
 * supports, products, or histogram values are embedded.
 */

const DEFAULT_X = 353492451n;
const DEFAULT_Y = 364428675n;

function canonicalBinary(n) {
  if (n <= 0n) throw new Error("the MathPages formulation uses positive integers");
  return n.toString(2);
}

function reverseBinaryInteger(n) {
  const reversed = canonicalBinary(n).split("").reverse().join("");
  return BigInt(`0b${reversed}`);
}

function support(n) {
  const out = [];
  let value = n;
  let index = 0;
  while (value > 0n) {
    if ((value & 1n) === 1n) out.push(index);
    value >>= 1n;
    index += 1;
  }
  return out;
}

function distanceHistogram(points) {
  const hist = new Map();
  for (let i = 0; i < points.length; i += 1) {
    for (let j = i + 1; j < points.length; j += 1) {
      const d = points[j] - points[i];
      hist.set(d, (hist.get(d) || 0) + 1);
    }
  }
  return hist;
}

function sortedKeys(hist) {
  return [...hist.keys()].sort((a, b) => a - b);
}

function formatSupport(points) {
  return `{${points.join(", ")}}`;
}

function formatHistogram(hist) {
  return `{${sortedKeys(hist).map((d) => `${d}: ${hist.get(d)}`).join(", ")}}`;
}

function histogramsEqual(a, b) {
  const keys = new Set([...a.keys(), ...b.keys()]);
  for (const key of keys) {
    if ((a.get(key) || 0) !== (b.get(key) || 0)) return false;
  }
  return true;
}

function requireCondition(condition, message) {
  if (!condition) throw new Error(message);
}

function parseInputs(argv) {
  if (argv.length === 2) return [DEFAULT_X, DEFAULT_Y];
  if (argv.length === 4) {
    try {
      return [BigInt(argv[2]), BigInt(argv[3])];
    } catch (error) {
      throw new Error(`inputs must be decimal integers: ${error.message}`);
    }
  }
  throw new Error(`usage: ${argv[1]} [x y]`);
}

function main() {
  const [x, y] = parseInputs(process.argv);
  const bx = canonicalBinary(x);
  const by = canonicalBinary(y);
  const rx = reverseBinaryInteger(x);
  const ry = reverseBinaryInteger(y);
  const sx = support(x);
  const sy = support(y);
  const hx = distanceHistogram(sx);
  const hy = distanceHistogram(sy);
  const px = x * rx;
  const py = y * ry;
  const allDistances = [...new Set([...hx.keys(), ...hy.keys()])].sort((a, b) => a - b);
  const differingDistances = allDistances.filter(
    (d) => (hx.get(d) || 0) !== (hy.get(d) || 0),
  );

  requireCondition(bx.length === by.length, "x and y must have the same bit length here");
  requireCondition(x !== y, "x and y must be distinct");
  requireCondition(y !== rx && x !== ry, "x and y must not be reflections");
  requireCondition(px === py, "the reversal products are unequal");
  requireCondition(!histogramsEqual(hx, hy), "the point-distance multisets are equal");
  requireCondition((hx.get(3) || 0) !== (hy.get(3) || 0), "distance 3 does not distinguish them");
  requireCondition(
    [...hx.values()].reduce((a, b) => a + b, 0) === (sx.length * (sx.length - 1)) / 2,
    "x histogram has the wrong number of unordered pairs",
  );
  requireCondition(
    [...hy.values()].reduce((a, b) => a + b, 0) === (sy.length * (sy.length - 1)) / 2,
    "y histogram has the wrong number of unordered pairs",
  );

  console.log("MathPages Wanted Problem 1 counterexample verifier (JavaScript BigInt)");
  console.log(`x                  = ${x}`);
  console.log(`y                  = ${y}`);
  console.log(`bit length         = ${bx.length}`);
  console.log(`binary(x)          = ${bx}`);
  console.log(`binary(y)          = ${by}`);
  console.log(`reverse(x)         = ${rx}`);
  console.log(`binary(reverse(x)) = ${canonicalBinary(rx)}`);
  console.log(`reverse(y)         = ${ry}`);
  console.log(`binary(reverse(y)) = ${canonicalBinary(ry)}`);
  console.log(`x*reverse(x)       = ${px}`);
  console.log(`y*reverse(y)       = ${py}`);
  console.log(`support(x)         = ${formatSupport(sx)}`);
  console.log(`support(y)         = ${formatSupport(sy)}`);
  console.log(`distance histogram(x) = ${formatHistogram(hx)}`);
  console.log(`distance histogram(y) = ${formatHistogram(hy)}`);
  console.log(`differing distances    = [${differingDistances.join(", ")}]`);
  console.log(`multiplicity at d=3    = ${hx.get(3) || 0} versus ${hy.get(3) || 0}`);
  console.log("COUNTEREXAMPLE VERIFIED");
}

try {
  main();
} catch (error) {
  console.error(`VERIFICATION FAILED: ${error.message}`);
  process.exitCode = 1;
}
