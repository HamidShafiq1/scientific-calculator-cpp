#include "calculator.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>

// ── Minimal test harness ──────────────────────────────────────────────────────

static int pass_count = 0;
static int fail_count = 0;

static void check(const std::string& label, double got, double expected, double tol = 1e-9) {
    bool ok = std::abs(got - expected) <= tol;
    if (ok) {
        std::cout << "  [PASS]  " << label << "\n";
        pass_count++;
    } else {
        std::cout << "  [FAIL]  " << label
                  << "  got=" << got << "  expected=" << expected << "\n";
        fail_count++;
    }
}

static void check_throws(const std::string& label, const std::string& expr) {
    try {
        calc::evaluate(expr);
        std::cout << "  [FAIL]  " << label << "  (no exception thrown)\n";
        fail_count++;
    } catch (...) {
        std::cout << "  [PASS]  " << label << "  (exception as expected)\n";
        pass_count++;
    }
}

static void section(const std::string& name) {
    std::cout << "\n  --- " << name << " ---\n";
}

// ── Test suites ───────────────────────────────────────────────────────────────

void test_arithmetic() {
    section("Basic Arithmetic");
    check("2 + 3",       calc::evaluate("2+3"),       5.0);
    check("10 - 4",      calc::evaluate("10-4"),      6.0);
    check("3 * 7",       calc::evaluate("3*7"),       21.0);
    check("15 / 4",      calc::evaluate("15/4"),      3.75);
    check("10 % 3",      calc::evaluate("10%3"),      1.0);
    check("2 ^ 10",      calc::evaluate("2^10"),      1024.0);
    check("-(3+2)",      calc::evaluate("-(3+2)"),    -5.0);
    check("2^3^2 = 512", calc::evaluate("2^3^2"),     512.0); // right-assoc
}

void test_trig() {
    section("Trigonometry");
    check("sin(0)",        calc::evaluate("sin(0)"),                0.0);
    check("cos(0)",        calc::evaluate("cos(0)"),                1.0);
    check("sin(pi/2)",     calc::evaluate("sin(pi/2)"),             1.0);
    check("cos(pi)",       calc::evaluate("cos(pi)"),              -1.0);
    check("tan(pi/4)",     calc::evaluate("tan(pi/4)"),             1.0);
    check("asin(1)*2/pi",  calc::evaluate("asin(1)*2/pi"),          1.0);
    check("acos(0)*2/pi",  calc::evaluate("acos(0)*2/pi"),          1.0);
    check("atan(1,1)/pi",  calc::evaluate("atan(1,1)/pi"),          0.25);
}

void test_hyp() {
    section("Hyperbolic");
    check("sinh(0)",  calc::evaluate("sinh(0)"),  0.0);
    check("cosh(0)",  calc::evaluate("cosh(0)"),  1.0);
    check("tanh(0)",  calc::evaluate("tanh(0)"),  0.0);
}

void test_log_exp() {
    section("Logarithms & Exponentials");
    check("log(e)",      calc::evaluate("log(e)"),       1.0);
    check("log10(100)",  calc::evaluate("log10(100)"),   2.0);
    check("log2(8)",     calc::evaluate("log2(8)"),      3.0);
    check("exp(0)",      calc::evaluate("exp(0)"),       1.0);
    check("exp(1) = e",  calc::evaluate("exp(1)"),       calc::constants::E);
    check("sqrt(9)",     calc::evaluate("sqrt(9)"),      3.0);
    check("cbrt(27)",    calc::evaluate("cbrt(27)"),     3.0);
    check("nrt(32,5)",   calc::evaluate("nrt(32,5)"),    2.0);
}

void test_combinatorics() {
    section("Combinatorics");
    check("fact(5)",   calc::evaluate("fact(5)"),    120.0);
    check("fact(0)",   calc::evaluate("fact(0)"),    1.0);
    check("p(5,2)",    calc::evaluate("p(5,2)"),     20.0);
    check("c(5,2)",    calc::evaluate("c(5,2)"),     10.0);
    check("gcd(12,8)", calc::evaluate("gcd(12,8)"),  4.0);
    check("lcm(4,6)",  calc::evaluate("lcm(4,6)"),  12.0);
}

void test_misc() {
    section("Misc Functions");
    check("abs(-7)",     calc::evaluate("abs(-7)"),    7.0);
    check("ceil(1.2)",   calc::evaluate("ceil(1.2)"),  2.0);
    check("floor(1.9)",  calc::evaluate("floor(1.9)"), 1.0);
    check("round(2.5)",  calc::evaluate("round(2.5)"), 3.0);
    check("sign(-5)",    calc::evaluate("sign(-5)"),  -1.0);
    check("max(3,7)",    calc::evaluate("max(3,7)"),   7.0);
    check("min(3,7)",    calc::evaluate("min(3,7)"),   3.0);
    check("hypot(3,4)",  calc::evaluate("hypot(3,4)"), 5.0);
}

void test_constants() {
    section("Constants");
    check("pi",    calc::evaluate("pi"),    calc::constants::PI);
    check("e",     calc::evaluate("e"),     calc::constants::E);
    check("phi",   calc::evaluate("phi"),   calc::constants::PHI);
    check("sqrt2", calc::evaluate("sqrt2"), calc::constants::SQRT2);
}

void test_complex_exprs() {
    section("Complex Expressions");
    check("(1+2)*3",            calc::evaluate("(1+2)*3"),              9.0);
    check("sin(pi/6)+cos(pi/3)",calc::evaluate("sin(pi/6)+cos(pi/3)"), 1.0);
    check("log(e^3)",           calc::evaluate("log(exp(3))"),          3.0);
    check("sqrt(2)*sqrt(2)",    calc::evaluate("sqrt(2)*sqrt(2)"),      2.0, 1e-12);
    check("c(10,3)",            calc::evaluate("c(10,3)"),              120.0);
    check("2^(3+1)",            calc::evaluate("2^(3+1)"),              16.0);
}

void test_errors() {
    section("Error Handling");
    check_throws("division by zero",     "1/0");
    check_throws("sqrt negative",        "sqrt(-1)");
    check_throws("log of zero",          "log(0)");
    check_throws("asin out of domain",   "asin(2)");
    check_throws("unknown identifier",   "xyz");
    check_throws("unmatched paren",      "(1+2");
    check_throws("empty expression",     "");
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main() {
    std::cout << "\n============================================================\n";
    std::cout << "  Scientific Calculator - Unit Tests\n";
    std::cout << "============================================================\n";

    test_arithmetic();
    test_trig();
    test_hyp();
    test_log_exp();
    test_combinatorics();
    test_misc();
    test_constants();
    test_complex_exprs();
    test_errors();

    std::cout << "\n============================================================\n";
    std::cout << "  Results: " << pass_count << " passed, " << fail_count << " failed\n";
    std::cout << "============================================================\n\n";

    return fail_count == 0 ? 0 : 1;
}
