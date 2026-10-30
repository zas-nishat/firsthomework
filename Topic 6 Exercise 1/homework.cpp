// =============================================================================
// EXERCISE: operator overloading
// =============================================================================
//
// Goal: write a class `Polynomial` (integer coefficients) that can be used
// like a mathematical object:
//
//     Polynomial p = Polynomial({3, 2, 5});    // 3x^2 + 2x + 5
//     Polynomial q = Polynomial({1, 1});       // x + 1
//
// -----------------------------------------------------------------------------
// RULES
// -----------------------------------------------------------------------------
// 1. Write your class and all operators in the marked section below.
//
// 2. Storage format is your choice. You may store the coefficients highest-power-first
//    or lowest-power-first, in a std::vector, or anything else you like.
//
// 3. The constructor takes a list of integers ordered HIGHEST power first:
//        Polynomial{3, 2, 5}   means   3x^2 + 2x + 5
//        Polynomial{1, 0, -4}  means   x^2 - 4
//    Leading zeros are allowed in the input and must not change the meaning:
//        Polynomial{0, 0, 5, 2}  is the same polynomial as  Polynomial{5, 2}
//    An empty list or {0} gives the zero polynomial.
//
// 5. Two polynomials with the same mathematical value must always compare
//    equal, no matter how they were produced. (For example, subtracting
//    x^2 + 1 from x^2 + 3 must give exactly the same polynomial as {2}.)
//
// -----------------------------------------------------------------------------
// REQUIRED INTERFACE (your class must provide all of these)
// -----------------------------------------------------------------------------
//   Polynomial(vector<int> coeffs);   // highest power first
//   Polynomial + Polynomial
//   Polynomial - Polynomial
//   Polynomial + int
//   int + Polynomial
//   Polynomial - int
//   int - Polynomial
//   int operator()(int x) const    // evaluate the polynomial at x
//   std::ostream << Polynomial     // output format should look like e.g.: 2x^3 + 6x^2 + 4x + 10
//      use evaluate.py to check if what you made matches what is expected, adjust if needed.
//   Polynomial == Polynomial
//   Polynomial != Polynomial

#include <iostream>
#include <string>
#include <vector>

// ======================= YOUR CODE STARTS HERE ===============================

#include <cmath>
#include <algorithm>
#include <initializer_list>

class Polynomial {
private:
    std::vector<int> coeffs; // lowest-power-first: coeffs[i] is coefficient of x^i

    void normalize() {
        while (!coeffs.empty() && coeffs.back() == 0) {
            coeffs.pop_back();
        }
    }

public:
    Polynomial() = default;

    Polynomial(const std::vector<int>& inputCoeffs) {
        coeffs.assign(inputCoeffs.rbegin(), inputCoeffs.rend());
        normalize();
    }

    Polynomial(std::initializer_list<int> list) {
        coeffs.assign(std::rbegin(list), std::rend(list));
        normalize();
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial res;
        size_t n = std::max(coeffs.size(), other.coeffs.size());
        res.coeffs.resize(n, 0);
        for (size_t i = 0; i < n; ++i) {
            int a = (i < coeffs.size()) ? coeffs[i] : 0;
            int b = (i < other.coeffs.size()) ? other.coeffs[i] : 0;
            res.coeffs[i] = a + b;
        }
        res.normalize();
        return res;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial res;
        size_t n = std::max(coeffs.size(), other.coeffs.size());
        res.coeffs.resize(n, 0);
        for (size_t i = 0; i < n; ++i) {
            int a = (i < coeffs.size()) ? coeffs[i] : 0;
            int b = (i < other.coeffs.size()) ? other.coeffs[i] : 0;
            res.coeffs[i] = a - b;
        }
        res.normalize();
        return res;
    }

    Polynomial operator+(int n) const {
        Polynomial res = *this;
        if (res.coeffs.empty()) {
            if (n != 0) {
                res.coeffs.push_back(n);
            }
        } else {
            res.coeffs[0] += n;
            res.normalize();
        }
        return res;
    }

    friend Polynomial operator+(int n, const Polynomial& p) {
        return p + n;
    }

    Polynomial operator-(int n) const {
        Polynomial res = *this;
        if (res.coeffs.empty()) {
            if (n != 0) {
                res.coeffs.push_back(-n);
            }
        } else {
            res.coeffs[0] -= n;
            res.normalize();
        }
        return res;
    }

    friend Polynomial operator-(int n, const Polynomial& p) {
        if (p.coeffs.empty()) {
            Polynomial res;
            if (n != 0) {
                res.coeffs.push_back(n);
            }
            return res;
        }
        Polynomial res;
        res.coeffs.resize(p.coeffs.size());
        for (size_t i = 0; i < p.coeffs.size(); ++i) {
            res.coeffs[i] = -p.coeffs[i];
        }
        res.coeffs[0] += n;
        res.normalize();
        return res;
    }

    int operator()(int x) const {
        if (coeffs.empty()) {
            return 0;
        }
        long long val = 0;
        for (int i = static_cast<int>(coeffs.size()) - 1; i >= 0; --i) {
            val = val * x + coeffs[i];
        }
        return static_cast<int>(val);
    }

    bool operator==(const Polynomial& other) const {
        return coeffs == other.coeffs;
    }

    bool operator!=(const Polynomial& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        if (p.coeffs.empty()) {
            os << "0";
            return os;
        }

        bool first = true;
        for (int i = static_cast<int>(p.coeffs.size()) - 1; i >= 0; --i) {
            int c = p.coeffs[i];
            if (c == 0) {
                continue;
            }

            if (first) {
                if (c < 0) {
                    os << "-";
                }
                first = false;
            } else {
                if (c < 0) {
                    os << " - ";
                } else {
                    os << " + ";
                }
            }

            int abs_c = std::abs(c);
            if (i == 0) {
                os << abs_c;
            } else {
                if (abs_c != 1) {
                    os << abs_c;
                }
                if (i == 1) {
                    os << "x";
                } else {
                    os << "x^" << i;
                }
            }
        }
        return os;
    }
};

// ======================== YOUR CODE ENDS HERE ================================

int main(int argc, char* argv[]) {

    const char* defaultArgs[] = {"N/A", "3", "2", "5", "|", "1", "1"};
    if (argc == 1) {
        argc = 7;
        argv = const_cast<char**>(defaultArgs);
    }
    // by default, output should be:
    // "3x^2 + 2x + 5 ; 3x^2 + 3x + 6 ; 3x^2 + x + 4 ; 3x^2 + 2x + 8 ; 3x^2 + 2x + 8 ; 3x^2 + 2x + 2 ; -3x^2 - 2x - 2 ; 21 ; false ; true"

    std::vector<int> firstCoeffs, secondCoeffs;
    bool readingSecond = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "|")             readingSecond = true;
        else if (readingSecond)     secondCoeffs.push_back(std::stoi(arg));
        else                        firstCoeffs.push_back(std::stoi(arg));
    }

    Polynomial polynomial1(firstCoeffs);
    Polynomial polynomial2(secondCoeffs);
    const int number = 3;
    const int evalPoint = 2;

    std::cout << std::boolalpha
              << polynomial1                    << " ; "
              << polynomial1 + polynomial2      << " ; "
              << polynomial1 - polynomial2      << " ; "
              << polynomial1 + number           << " ; "
              << number      + polynomial1      << " ; "
              << polynomial1 - number           << " ; "
              << number      - polynomial1      << " ; "
              << polynomial1(evalPoint)         << " ; "
              << (polynomial1 == polynomial2)   << " ; "
              << (polynomial1 != polynomial2);
    return 0;
}
