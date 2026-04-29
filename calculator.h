#pragma once
#include <string>
#include <stdexcept>
#include <cmath>

namespace calc {

// ── Basic Arithmetic ──────────────────────────────────────────────────────────
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);       // throws std::domain_error on /0
double modulo(double a, double b);       // throws std::domain_error on mod 0
double power(double base, double exp);

// ── Trigonometry (radians by default) ────────────────────────────────────────
double sine(double rad);
double cosine(double rad);
double tangent(double rad);              // throws on undefined (pi/2 + n*pi)
double arcsine(double x);               // throws if |x| > 1
double arccosine(double x);             // throws if |x| > 1
double arctangent(double x);
double arctangent2(double y, double x); // atan2

// ── Hyperbolic ────────────────────────────────────────────────────────────────
double sinh_val(double x);
double cosh_val(double x);
double tanh_val(double x);

// ── Logarithms & Exponentials ─────────────────────────────────────────────────
double natural_log(double x);           // throws if x <= 0
double log_base10(double x);            // throws if x <= 0
double log_base(double x, double base); // throws if x <= 0 or base <= 0, base != 1
double exp_val(double x);
double sqrt_val(double x);             // throws if x < 0
double cbrt_val(double x);
double nth_root(double x, double n);   // throws on invalid domain

// ── Combinatorics & Number Theory ─────────────────────────────────────────────
double factorial(double n);            // throws if n < 0 or non-integer
double permutation(double n, double r);
double combination(double n, double r);
double gcd_val(double a, double b);
double lcm_val(double a, double b);

// ── Rounding & Misc ───────────────────────────────────────────────────────────
double floor_val(double x);
double ceil_val(double x);
double round_val(double x);
double absolute(double x);
double sign_val(double x);

// ── Angle Conversion ─────────────────────────────────────────────────────────
double deg_to_rad(double deg);
double rad_to_deg(double rad);

// ── Constants ────────────────────────────────────────────────────────────────
namespace constants {
    constexpr double PI    = M_PI;
    constexpr double E     = M_E;
    constexpr double PHI   = 1.6180339887498948482; // golden ratio
    constexpr double SQRT2 = M_SQRT2;
    constexpr double LN2   = M_LN2;
    constexpr double LN10  = M_LN10;
}

// ── Expression Parser ─────────────────────────────────────────────────────────
// Parses and evaluates infix expressions like "sin(pi/2) + log(100)"
// Supports: +, -, *, /, ^, %, unary minus, parentheses
// Functions: sin, cos, tan, asin, acos, atan, sinh, cosh, tanh,
//            log, log10, log2, exp, sqrt, cbrt, abs, ceil, floor, round, fact
// Constants: pi, e, phi, sqrt2, ln2, ln10
double evaluate(const std::string& expression);

} // namespace calc
