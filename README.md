# Scientific Calculator (C++)

A command-line scientific calculator built in C++17 with a recursive-descent expression parser.

## Project Structure

```
scientific_calculator/
├── include/
│   └── calculator.h       # Public API declarations
├── src/
│   ├── operations.cpp     # All math function implementations
│   ├── parser.cpp         # Recursive-descent infix expression parser
│   ├── main.cpp           # Interactive REPL
│   └── tests.cpp          # Unit test suite
└── Makefile
```

## Build

```bash
make            # builds both calculator and test runner
make run        # launch interactive REPL
make test       # run unit tests
make clean      # remove build artifacts
```

Requires: g++ with C++17 support, make, libm (standard on Linux/macOS).

## Features

### Operators
| Operator | Description |
|----------|-------------|
| `+` `-` `*` `/` | Basic arithmetic |
| `%` | Modulo |
| `^` | Power (right-associative) |

### Functions
| Function | Description |
|----------|-------------|
| `sin(x)` `cos(x)` `tan(x)` | Trig (radians) |
| `asin(x)` `acos(x)` `atan(y,x)` | Inverse trig |
| `sinh(x)` `cosh(x)` `tanh(x)` | Hyperbolic |
| `log(x)` | Natural logarithm |
| `log10(x)` `log2(x)` `logb(x,b)` | Logarithm bases |
| `exp(x)` | e^x |
| `sqrt(x)` `cbrt(x)` `nrt(x,n)` | Roots |
| `pow(a,b)` | a^b |
| `abs(x)` `ceil(x)` `floor(x)` `round(x)` | Rounding / absolute value |
| `sign(x)` | Sign: -1, 0, or 1 |
| `fact(n)` | n! |
| `p(n,r)` | Permutation nPr |
| `c(n,r)` | Combination nCr |
| `gcd(a,b)` `lcm(a,b)` | GCD / LCM |
| `max(a,b)` `min(a,b)` `hypot(a,b)` | Misc |
| `deg(d)` | Convert degrees to radians |

### Constants
`pi`, `e`, `phi`, `sqrt2`, `ln2`, `ln10`

### REPL Commands
| Command | Action |
|---------|--------|
| `ans` | Substitutes the last result |
| `history` | Show evaluation history |
| `clear` | Clear history |
| `help` | Show help screen |
| `quit` / `exit` | Exit |

## Example Session

```
  > sin(pi/6)
  = 0.5

  > 2^10
  = 1024

  > c(10,3)
  = 120

  > ans + fact(5)
  = 240

  > log(e^5)
  = 5

  > sqrt(2)*cos(pi/4)
  = 1
```

## Architecture

The parser implements standard recursive-descent with the following grammar:

```
expr    := term (('+' | '-') term)*
term    := power (('*' | '/' | '%') power)*
power   := unary ('^' power)?          // right-associative
unary   := ('-' | '+') unary | primary
primary := number | constant | func '(' args ')' | '(' expr ')'
```

All math functions are wrapped in `namespace calc` with domain validation and `std::domain_error` / `std::overflow_error` throws where appropriate.
