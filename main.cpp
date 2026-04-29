#include "calculator.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>

// ── History ───────────────────────────────────────────────────────────────────

struct HistoryEntry {
    std::string expr;
    double result;
};

static std::vector<HistoryEntry> history;
static double last_ans = 0.0;

// ── Helpers ───────────────────────────────────────────────────────────────────

static void print_separator(char c = '-', int width = 60) {
    std::cout << std::string(width, c) << "\n";
}

static std::string format_result(double val) {
    if (std::isinf(val)) return val > 0 ? "Infinity" : "-Infinity";
    if (std::isnan(val)) return "NaN";

    std::ostringstream oss;
    // Integer check
    if (val == std::floor(val) && std::abs(val) < 1e15) {
        oss << std::fixed << std::setprecision(0) << val;
    } else {
        oss << std::setprecision(15) << val;
        // Trim trailing zeros after decimal
        std::string s = oss.str();
        if (s.find('.') != std::string::npos) {
            s.erase(s.find_last_not_of('0') + 1);
            if (s.back() == '.') s.pop_back();
        }
        return s;
    }
    return oss.str();
}

// ── Help Text ─────────────────────────────────────────────────────────────────

static void print_help() {
    print_separator('=');
    std::cout << "  SCIENTIFIC CALCULATOR  |  C++ Expression Evaluator\n";
    print_separator('=');
    std::cout <<
R"(
OPERATORS
  +  -  *  /  %  ^     (^ is right-associative: 2^3^2 = 2^9)

FUNCTIONS
  sin(x)   cos(x)   tan(x)         Trig (radians)
  asin(x)  acos(x)  atan(y,x)      Inverse trig
  sinh(x)  cosh(x)  tanh(x)        Hyperbolic
  log(x)   log10(x) log2(x)        Logarithms (log = natural)
  logb(x,b)                         Log base b
  exp(x)   sqrt(x)  cbrt(x)        Exp & roots
  nrt(x,n)                          nth root of x
  pow(a,b) abs(x)   sign(x)        Power, absolute, sign
  ceil(x)  floor(x) round(x)       Rounding
  fact(n)  p(n,r)   c(n,r)         Factorial, nPr, nCr
  gcd(a,b) lcm(a,b)                GCD / LCM
  max(a,b) min(a,b) hypot(a,b)     Misc
  deg(d)             rad->deg       Degree to radian
  mod(a,b)                          Modulo

CONSTANTS
  pi    e    phi    sqrt2    ln2    ln10

SPECIAL COMMANDS
  ans         last result
  history     show history
  clear       clear history
  help        this screen
  quit / exit exit the program

EXAMPLES
  sin(pi/6)
  log(e^3)
  fact(10)
  c(10,3)
  2^10
  sqrt(2) * cos(pi/4)
  atan(1,1) * 4        (should give pi)
)";
    print_separator('=');
}

// ── History ───────────────────────────────────────────────────────────────────

static void print_history() {
    if (history.empty()) {
        std::cout << "  (empty)\n";
        return;
    }
    print_separator();
    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << "  [" << (i + 1) << "]  "
                  << history[i].expr
                  << "  =  " << format_result(history[i].result) << "\n";
    }
    print_separator();
}

// ── Preprocess: replace 'ans' token with last result ─────────────────────────

static std::string substitute_ans(const std::string& input) {
    std::string out;
    size_t i = 0;
    while (i < input.size()) {
        if (input.substr(i, 3) == "ans") {
            // Make sure it's not part of a longer identifier
            bool after_ok  = (i + 3 >= input.size()) || !std::isalnum((unsigned char)input[i+3]);
            bool before_ok = (i == 0) || !std::isalnum((unsigned char)input[i-1]);
            if (before_ok && after_ok) {
                std::ostringstream oss;
                oss << std::setprecision(15) << last_ans;
                out += "(" + oss.str() + ")";
                i += 3;
                continue;
            }
        }
        out += input[i++];
    }
    return out;
}

// ── Main REPL ─────────────────────────────────────────────────────────────────

int main() {
    print_help();
    std::cout << "\n";

    std::string line;
    while (true) {
        std::cout << "  > ";
        if (!std::getline(std::cin, line)) break;

        // Trim
        size_t s = line.find_first_not_of(" \t\r\n");
        if (s == std::string::npos) continue;
        size_t e = line.find_last_not_of(" \t\r\n");
        line = line.substr(s, e - s + 1);

        if (line.empty()) continue;

        // To lowercase for command matching
        std::string cmd = line;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (cmd == "quit" || cmd == "exit") {
            std::cout << "\n  Goodbye.\n\n";
            break;
        }
        if (cmd == "help") {
            print_help();
            continue;
        }
        if (cmd == "history") {
            print_history();
            continue;
        }
        if (cmd == "clear") {
            history.clear();
            std::cout << "  History cleared.\n";
            continue;
        }

        // Substitute 'ans'
        std::string processed = substitute_ans(line);

        try {
            double result = calc::evaluate(processed);
            last_ans = result;
            history.push_back({line, result});

            std::cout << "  = " << format_result(result) << "\n\n";
        } catch (const std::exception& ex) {
            std::cerr << "  Error: " << ex.what() << "\n\n";
        }
    }

    return 0;
}
