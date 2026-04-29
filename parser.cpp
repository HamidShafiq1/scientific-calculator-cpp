#include "calculator.h"
#include <string>
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <algorithm>

namespace calc {

// ── Tokenizer / Parser State ──────────────────────────────────────────────────

struct Parser {
    const std::string& expr;
    size_t pos;

    explicit Parser(const std::string& e) : expr(e), pos(0) {}

    char peek() const {
        while (pos < expr.size() && std::isspace((unsigned char)expr[pos]))
            const_cast<Parser*>(this)->pos++;
        return pos < expr.size() ? expr[pos] : '\0';
    }

    char consume() {
        peek();
        return pos < expr.size() ? expr[pos++] : '\0';
    }

    bool match(char c) {
        if (peek() == c) { consume(); return true; }
        return false;
    }

    void expect(char c) {
        if (!match(c))
            throw std::runtime_error(std::string("Expected '") + c + "' at position " + std::to_string(pos));
    }

    // Forward declarations
    double parseExpr();
    double parseTerm();
    double parseFactor();
    double parsePower();
    double parseUnary();
    double parsePrimary();
    double parseNumber();
    double parseFunction(const std::string& name);
    std::string parseIdentifier();
};

// ── Grammar (precedence, low to high):
//   expr     := term (('+' | '-') term)*
//   term     := power (('*' | '/' | '%') power)*
//   power    := unary ('^' power)?          right-associative
//   unary    := '-' unary | primary
//   primary  := number | constant | func '(' args ')' | '(' expr ')'

double Parser::parseExpr() {
    double left = parseTerm();
    while (true) {
        if (match('+'))      left = add(left, parseTerm());
        else if (match('-')) left = subtract(left, parseTerm());
        else break;
    }
    return left;
}

double Parser::parseTerm() {
    double left = parsePower();
    while (true) {
        if (match('*'))      left = multiply(left, parsePower());
        else if (match('/')) left = divide(left, parsePower());
        else if (match('%')) left = modulo(left, parsePower());
        else break;
    }
    return left;
}

double Parser::parsePower() {
    double base = parseUnary();
    if (match('^'))
        return power(base, parsePower()); // right-associative
    return base;
}

double Parser::parseUnary() {
    if (match('-')) return -parseUnary();
    if (match('+')) return parseUnary();
    return parsePrimary();
}

double Parser::parsePrimary() {
    char c = peek();

    // Parenthesised sub-expression
    if (c == '(') {
        consume();
        double val = parseExpr();
        expect(')');
        return val;
    }

    // Number literal
    if (std::isdigit((unsigned char)c) || c == '.') {
        return parseNumber();
    }

    // Identifier: constant or function
    if (std::isalpha((unsigned char)c) || c == '_') {
        std::string name = parseIdentifier();

        // Named constants
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower == "pi")    return constants::PI;
        if (lower == "e")     return constants::E;
        if (lower == "phi")   return constants::PHI;
        if (lower == "sqrt2") return constants::SQRT2;
        if (lower == "ln2")   return constants::LN2;
        if (lower == "ln10")  return constants::LN10;
        if (lower == "inf" || lower == "infinity") return std::numeric_limits<double>::infinity();

        // Must be a function call
        if (peek() == '(')
            return parseFunction(lower);

        throw std::runtime_error("Unknown identifier: " + name);
    }

    throw std::runtime_error(std::string("Unexpected character '") + c + "' at position " + std::to_string(pos));
}

double Parser::parseNumber() {
    size_t start = pos;
    peek(); // skip whitespace already accounted for in peek()
    while (pos < expr.size() && (std::isdigit((unsigned char)expr[pos]) || expr[pos] == '.'))
        pos++;
    // Scientific notation: 1e10, 2.5E-3
    if (pos < expr.size() && (expr[pos] == 'e' || expr[pos] == 'E')) {
        pos++;
        if (pos < expr.size() && (expr[pos] == '+' || expr[pos] == '-'))
            pos++;
        while (pos < expr.size() && std::isdigit((unsigned char)expr[pos]))
            pos++;
    }
    std::string token = expr.substr(start, pos - start);
    if (token.empty() || token == ".")
        throw std::runtime_error("Invalid number at position " + std::to_string(start));
    return std::stod(token);
}

std::string Parser::parseIdentifier() {
    peek();
    size_t start = pos;
    while (pos < expr.size() && (std::isalnum((unsigned char)expr[pos]) || expr[pos] == '_'))
        pos++;
    return expr.substr(start, pos - start);
}

double Parser::parseFunction(const std::string& name) {
    expect('(');

    // Helper: parse one argument
    auto arg1 = [&]() -> double {
        double v = parseExpr();
        expect(')');
        return v;
    };

    // Helper: parse two comma-separated arguments
    auto arg2 = [&]() -> std::pair<double,double> {
        double a = parseExpr();
        expect(',');
        double b = parseExpr();
        expect(')');
        return {a, b};
    };

    if (name == "sin")   return sine(arg1());
    if (name == "cos")   return cosine(arg1());
    if (name == "tan")   return tangent(arg1());
    if (name == "asin")  return arcsine(arg1());
    if (name == "acos")  return arccosine(arg1());
    if (name == "atan")  { auto [a,b] = arg2(); return arctangent2(a,b); }
    if (name == "atan1") return arctangent(arg1());
    if (name == "sinh")  return sinh_val(arg1());
    if (name == "cosh")  return cosh_val(arg1());
    if (name == "tanh")  return tanh_val(arg1());
    if (name == "ln" || name == "log") return natural_log(arg1());
    if (name == "log10") return log_base10(arg1());
    if (name == "log2")  return log_base(arg1(), 2.0);
    if (name == "logb")  { auto [x,b] = arg2(); return log_base(x, b); }
    if (name == "exp")   return exp_val(arg1());
    if (name == "sqrt")  return sqrt_val(arg1());
    if (name == "cbrt")  return cbrt_val(arg1());
    if (name == "nrt")   { auto [x,n] = arg2(); return nth_root(x, n); }
    if (name == "pow")   { auto [a,b] = arg2(); return power(a, b); }
    if (name == "abs")   return absolute(arg1());
    if (name == "ceil")  return ceil_val(arg1());
    if (name == "floor") return floor_val(arg1());
    if (name == "round") return round_val(arg1());
    if (name == "sign")  return sign_val(arg1());
    if (name == "fact")  return factorial(arg1());
    if (name == "p")     { auto [n,r] = arg2(); return permutation(n, r); }
    if (name == "c")     { auto [n,r] = arg2(); return combination(n, r); }
    if (name == "gcd")   { auto [a,b] = arg2(); return gcd_val(a, b); }
    if (name == "lcm")   { auto [a,b] = arg2(); return lcm_val(a, b); }
    if (name == "deg")   return deg_to_rad(arg1());
    if (name == "rad")   return rad_to_deg(arg1());
    if (name == "mod")   { auto [a,b] = arg2(); return modulo(a, b); }
    if (name == "max")   { auto [a,b] = arg2(); return std::max(a, b); }
    if (name == "min")   { auto [a,b] = arg2(); return std::min(a, b); }
    if (name == "hypot") { auto [a,b] = arg2(); return std::hypot(a, b); }

    throw std::runtime_error("Unknown function: " + name);
}

// ── Public entry point ────────────────────────────────────────────────────────

double evaluate(const std::string& expression) {
    if (expression.empty())
        throw std::invalid_argument("Empty expression");
    Parser p(expression);
    double result = p.parseExpr();
    p.peek();
    if (p.pos != expression.size())
        throw std::runtime_error("Unexpected token at position " + std::to_string(p.pos));
    return result;
}

} // namespace calc
