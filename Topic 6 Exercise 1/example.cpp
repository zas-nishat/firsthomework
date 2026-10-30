// =============================================================================
// DEMO: operator overloading with a Duration class (hours / minutes / seconds)
// =============================================================================
//
// What this demo shows:
//   1. Member operators:      a + b, a - b, a * n, comparisons, ++
//   2. Non-member operators:  n * a (int on the LEFT), and << for printing
//   3. Normalization:         59 min + 1 min must become 1 hour, not "60 min"
//   4. Prefix vs. postfix ++
//   5. Building some operators out of others (!=, >, <=, >= from == and <)
//
// Design choice: everything is stored as ONE number (total seconds). The
// constructor splits hours/minutes/seconds into that number, and the getters
// split it back out. That way normalization happens in exactly one place.

#include <iomanip>
#include <iostream>

class Duration {
public:
    // Any values are accepted: Duration(0, 90, 75) is 1:31:15
    Duration(int hours = 0, int minutes = 0, int seconds = 0)
        : totalSeconds(hours * 3600 + minutes * 60 + seconds) {
        if (totalSeconds < 0) totalSeconds = 0;  // no negative durations
    }

    int hours()   const { return totalSeconds / 3600; }
    int minutes() const { return (totalSeconds % 3600) / 60; }
    int seconds() const { return totalSeconds % 60; }

    // ---- Arithmetic (members: the LEFT operand is a Duration) ---------------

    Duration operator+(const Duration& other) const {
        return Duration(0, 0, totalSeconds + other.totalSeconds);
    }

    // Clamps at zero if other is longer than this
    Duration operator-(const Duration& other) const {
        return Duration(0, 0, totalSeconds - other.totalSeconds);
    }

    // Duration * int
    Duration operator*(int factor) const {
        return Duration(0, 0, totalSeconds * factor);
    }

    // ---- Comparison ---------------------------------------------------------
    // Only == and < contain real logic, the rest are built from them.

    bool operator==(const Duration& other) const {
        return totalSeconds == other.totalSeconds;
    }
    bool operator<(const Duration& other) const {
        return totalSeconds < other.totalSeconds;
    }
    bool operator!=(const Duration& other) const { return !(*this == other); }
    bool operator>(const Duration& other)  const { return other < *this; }
    bool operator<=(const Duration& other) const { return !(other < *this); }
    bool operator>=(const Duration& other) const { return !(*this < other); }

    // ---- Increment (adds one second) ----------------------------------------

    // Prefix: ++d   changes d, returns the NEW value
    Duration& operator++() {
        ++totalSeconds;
        return *this;
    }

    // Postfix: d++  changes d, returns a copy of the OLD value.
    // The dummy int parameter is only there to tell the compiler
    // "this is the postfix version".
    Duration operator++(int) {
        Duration old = *this;
        ++totalSeconds;
        return old;
    }

private:
    int totalSeconds;
};

// ---- Non-member operators ---------------------------------------------------

// int * Duration
// This CANNOT be a member: for "3 * d" the left operand is an int, and ints
// have no member functions. It just reuses the member version with the
// operands swapped (multiplication is commutative).
Duration operator*(int factor, const Duration& duration) {
    return duration * factor;
}

// std::cout << Duration
// Also cannot be a member: the left operand is the stream (std::ostream),
// which we don't own. Uses only the public getters, so no friend needed.
// Returning the stream is what allows chaining:  cout << a << " " << b
std::ostream& operator<<(std::ostream& out, const Duration& duration) {
    out << duration.hours() << ":"
        << std::setfill('0') << std::setw(2) << duration.minutes() << ":"
        << std::setw(2) << duration.seconds()
        << std::setfill(' ');  // restore the default fill character
    return out;
}

// ---- Demonstration ----------------------------------------------------------

int main() {
    Duration a(1, 30, 0);   // 1:30:00
    Duration b(0, 45, 30);  // 0:45:30

    std::cout << "a = " << a << ", b = " << b << "\n";

    // Normalization: values out of range roll over
    std::cout << "Duration(0, 90, 75) = " << Duration(0, 90, 75) << "\n";           // 1:31:15
    std::cout << "59 min + 1 min      = " << Duration(0, 59) + Duration(0, 1) << "\n";  // 1:00:00

    // Arithmetic
    std::cout << "a + b = " << a + b << "\n";                                     // 2:15:30
    std::cout << "a - b = " << a - b << "\n";                                     // 0:44:30
    std::cout << "b - a = " << b - a << "  (clamped at zero)\n";                  // 0:00:00

    // Scalar multiplication, both operand orders
    std::cout << "b * 3 = " << b * 3 << "\n";                                     // 2:16:30
    std::cout << "3 * b = " << 3 * b << "  (needs the non-member version)\n";     // 2:16:30

    // Comparison
    std::cout << std::boolalpha;
    std::cout << "a == b: " << (a == b) << "\n";                                  // false
    std::cout << "a != b: " << (a != b) << "\n";                                  // true
    std::cout << "a >  b: " << (a > b) << "\n";                                   // true
    std::cout << "a <= b: " << (a <= b) << "\n";                                  // false
    std::cout << "Duration(1, 30) == a: " << (Duration(1, 30) == a) << "\n";      // true

    // Prefix vs. postfix
    Duration c(0, 59, 59);
    std::cout << "c = " << c << "\n";                                             // 0:59:59
    std::cout << "c++ gives " << c++ << ", c is now " << c << "\n";               // 0:59:59, 1:00:00
    std::cout << "++c gives " << ++c << ", c is now " << c << "\n";               // 1:00:01, 1:00:01

    return 0;
}