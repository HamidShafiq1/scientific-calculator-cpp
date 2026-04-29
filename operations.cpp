#include "calculator.h"
#include <cmath>
#include <stdexcept>
#include <string>

namespace calc {

// ── Basic Arithmetic ──────────────────────────────────────────────────────────

double add(double a, double b)      { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }

double divide(double a, double b) {
    if (b == 0.0)
        throw std::domain_error("Division by zero");
    return a / b;
}

double modulo(double a, double b) {
    if (b == 0.0)
        throw std::domain_error("Modulo by zero");
    return std::fmod(a, b);
}

double power(double base, double exp) {
    return std::pow(base, exp);
}

// ── Trigonometry ──────────────────────────────────────────────────────────────

double sine(double rad)   { return std::sin(rad); }
double cosine(double rad) { return std::cos(rad); }

double tangent(double rad) {
    double c = std::cos(rad);
    if (std::abs(c) < 1e-15)
        throw std::domain_error("tan is undefined at this angle");
    return std::tan(rad);
}

double arcsine(double x) {
    if (x < -1.0 || x > 1.0)
        throw std::domain_error("asin domain: x must be in [-1, 1]");
    return std::asin(x);
}

double arccosine(double x) {
    if (x < -1.0 || x > 1.0)
        throw std::domain_error("acos domain: x must be in [-1, 1]");
    return std::acos(x);
}

double arctangent(double x)             { return std::atan(x); }
double arctangent2(double y, double x)  { return std::atan2(y, x); }

// ── Hyperbolic ────────────────────────────────────────────────────────────────

double sinh_val(double x) { return std::sinh(x); }
double cosh_val(double x) { return std::cosh(x); }
double tanh_val(double x) { return std::tanh(x); }

// ── Logarithms & Exponentials ─────────────────────────────────────────────────

double natural_log(double x) {
    if (x <= 0.0)
        throw std::domain_error("ln domain: x must be > 0");
    return std::log(x);
}

double log_base10(double x) {
    if (x <= 0.0)
        throw std::domain_error("log10 domain: x must be > 0");
    return std::log10(x);
}

double log_base(double x, double base) {
    if (x <= 0.0)
        throw std::domain_error("log domain: x must be > 0");
    if (base <= 0.0 || base == 1.0)
        throw std::domain_error("log domain: base must be > 0 and != 1");
    return std::log(x) / std::log(base);
}

double exp_val(double x)  { return std::exp(x); }

double sqrt_val(double x) {
    if (x < 0.0)
        throw std::domain_error("sqrt domain: x must be >= 0");
    return std::sqrt(x);
}

double cbrt_val(double x) { return std::cbrt(x); }

double nth_root(double x, double n) {
    if (n == 0.0)
        throw std::domain_error("nth_root: n cannot be zero");
    if (x < 0.0 && std::fmod(n, 2.0) == 0.0)
        throw std::domain_error("nth_root: even root of negative number");
    return std::pow(std::abs(x), 1.0 / n) * (x < 0 ? -1 : 1);
}

// ── Combinatorics ─────────────────────────────────────────────────────────────

double factorial(double n) {
    if (n < 0.0 || std::floor(n) != n)
        throw std::domain_error("factorial: n must be a non-negative integer");
    if (n > 170.0)
        throw std::overflow_error("factorial: n too large (> 170)");
    double result = 1.0;
    for (int i = 2; i <= static_cast<int>(n); ++i)
        result *= i;
    return result;
}

double permutation(double n, double r) {
    if (r > n)
        throw std::domain_error("permutation: r cannot exceed n");
    return factorial(n) / factorial(n - r);
}

double combination(double n, double r) {
    if (r > n)
        throw std::domain_error("combination: r cannot exceed n");
    return factorial(n) / (factorial(r) * factorial(n - r));
}

double gcd_val(double a, double b) {
    long long ia = static_cast<long long>(std::abs(a));
    long long ib = static_cast<long long>(std::abs(b));
    while (ib) { ia %= ib; std::swap(ia, ib); }
    return static_cast<double>(ia);
}

double lcm_val(double a, double b) {
    double g = gcd_val(a, b);
    if (g == 0.0) return 0.0;
    return std::abs(a * b) / g;
}

// ── Rounding & Misc ───────────────────────────────────────────────────────────

double floor_val(double x) { return std::floor(x); }
double ceil_val(double x)  { return std::ceil(x); }
double round_val(double x) { return std::round(x); }
double absolute(double x)  { return std::abs(x); }
double sign_val(double x)  { return (x > 0) - (x < 0); }

// ── Angle Conversion ─────────────────────────────────────────────────────────

double deg_to_rad(double deg) { return deg * constants::PI / 180.0; }
double rad_to_deg(double rad) { return rad * 180.0 / constants::PI; }

} // namespace calc
