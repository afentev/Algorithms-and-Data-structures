//
// Created by afentev on 10/25/21.
//
#include <iostream>
#include <cmath>
#include <vector>

/*
 * Long arithmetics with support of large integers (BigInteger) and rationals (Rational) numbers
 * Integers are stored in a 10^6 base system
 * Integers are multiplied using the Karatsuba algorithm (if their length is less than 50 in the given system)
 * and using Fast Fourier Transform (with complex numbers) otherwise
 *
 * Rationals are stored as fractions of two BigIntegers
 *
 * Time complexities for basic operations with BigInteger:
 * Summation and Subtraction - O(n)
 * Multiplication - O(n*logn)
 * Division - about O(n^2)
 */

const long double PI = 3.14159265358979323846264338327950288419716939937510L;

class Complex {
 public:
  Complex() : real(0), imaginary(0) {}

  Complex(long double r) : real(r), imaginary(0) {}

  Complex(long double r, long double im) : real(r), imaginary(im) {}

  Complex operator+ (const Complex& other) const {
    return {real + other.real, imaginary + other.imaginary};
  }

  Complex operator- (const Complex& other) const {
    return {real - other.real, imaginary - other.imaginary};
  }

  Complex operator* (const Complex& other) const {
    return {real * other.real - imaginary * other.imaginary,
            real * other.imaginary + imaginary * other.real};
  }

  Complex operator/ (const Complex& other) const {
    Complex ret(*this);
    ret /= other;
    return ret;
  }

  Complex& operator/= (const Complex& other) {
    long double newReal = (real * other.real + imaginary * other.imaginary) /
        (other.real * other.real + other.imaginary * other.imaginary);
    long double newIm = (imaginary * other.real - real * other.imaginary) /
        (other.real * other.real + other.imaginary * other.imaginary);
    real = newReal;
    imaginary = newIm;
    return *this;
  }

  Complex& operator*= (const Complex& other) {
    long double newReal = real * other.real - imaginary * other.imaginary;
    long double newIm = real * other.imaginary + imaginary * other.real;
    real = newReal;
    imaginary = newIm;
    return *this;
  }

  Complex& operator+= (const Complex& other) {
    real += other.real;
    imaginary += other.imaginary;
    return *this;
  }

  bool operator== (const Complex& other) const {
    return std::abs(real - other.real) < 1e-14l && std::abs(imaginary - other.imaginary) < 1e-14l;
  }

 public:
  long double real = 0;
  long double imaginary = 0;
};

class BigInteger {
 public:
  BigInteger() : digits(1, 0), isNegative(false) {}

  BigInteger (const BigInteger& other) {
    digits = other.digits;
    isNegative = other.isNegative;
  };

  BigInteger (int64_t x): isNegative(x < 0) {
    if (isNegative) {
      x = -x;
    }
    uint64_t bound = base;
    uint64_t copy = x;
    size_t power = 1;
    while (bound <= copy) {
      bound *= base;
      ++power;
    }
    digits.resize(power, 0);
    size_t pos = 0;
    while (copy > 0) {
      digits[pos++] = copy % base;  // get n-th bit and increment it after
      copy /= base;
    }
  }

  BigInteger (const std::string& str) {
    if (str[0] == '-') {
      isNegative = true;
    } else {
      isNegative = false;
    }
    int64_t number = 0;
    digits.reserve(str.size() / 6 + 1);
    for (int64_t ind = static_cast<int64_t>(str.size()) - 1; ind >= isNegative; --ind) {
      number += (str[ind] - '0') * binPow(10, (str.size() - ind - 1) % power);
      if ((str.size() - ind) % power == 0) {
        digits.push_back(number);
        number = 0;
      }
    }
    if (number != 0) {
      digits.push_back(number);
    }
    clearLeadingZeros();
  }

  BigInteger operator*= (const BigInteger& other) {
    bool sign = isNegative;
    if (digits.size() == 1) {
      *this = other.fastMultiplication(digits[0]);
    } else if (other.digits.size() == 1) {
      *this = fastMultiplication(other.digits[0]);
    } else if (digits.size() <= 50 && other.digits.size() <= 50) {
      scholarMul(other);
    } else {
      fftMultiplication(digits, other.digits, digits);
      clearLeadingZeros();
    }
    if (digits.size() != 1 || digits[0] != 0) {
      isNegative = sign ^ other.isNegative;
    } else {
      isNegative = false;
    }
    return *this;
  }

  BigInteger& operator/= (const BigInteger& other) {
    if (isAbsLowerThan(other)) {
      *this = 0;
      isNegative = false;
      return *this;
    }
    BigInteger res;
    res.digits.resize(digits.size() - other.digits.size() + 1, 0);
    BigInteger difference = *this;
    difference.isNegative = false;
    for (int64_t rank = static_cast<int64_t>(res.digits.size()) - 1; rank >= 0 && difference != 0; --rank) {
      int64_t lhs = -1;
      int64_t rhs = base;
      BigInteger correction;
      BigInteger lastLhs;
      while (rhs - lhs > 1) {
        int64_t mid = (rhs + lhs) / 2;
        correction = other.fastMultiplication(mid);
        correction.isNegative = false;
        int64_t prev = static_cast<int64_t>(correction.digits.size());
        correction.digits.resize(prev + rank);
        for (int64_t i = prev + rank - 1; i >= rank; --i) {
          correction.digits[i] = correction.digits[i - rank];
        }
        for (int64_t i = 0; i < rank; ++i) {
          correction.digits[i] = 0;
        }
        correction.clearLeadingZeros();
        if (difference.isAbsLowerThan(correction)) {
          rhs = mid;
        } else {
          lhs = mid;
          lastLhs = correction;
        }
      }
      difference -= lastLhs;
      res.digits[rank] = lhs;
    }
    res.isNegative = isNegative ^ other.isNegative;
    res.clearLeadingZeros();
    *this = res;
    return *this;
  }

  BigInteger operator+= (const BigInteger& other) {
    inplacePlus(other, false);
    return *this;
  }

  BigInteger operator-= (const BigInteger& other) {
    inplacePlus(other, true);
    return *this;
  }

  BigInteger& operator= (const BigInteger& other) {
    digits = other.digits;
    isNegative = other.isNegative;
    return *this;
  };

  friend bool operator< (const BigInteger& left, const BigInteger& right);
  friend bool operator== (const BigInteger& left, const BigInteger& right);
  friend bool operator<= (const BigInteger& left, const BigInteger& right);
  friend bool operator> (const BigInteger& left, const BigInteger& right);
  friend bool operator>= (const BigInteger& left, const BigInteger& right);
  friend bool operator!= (const BigInteger& left, const BigInteger& right);

  BigInteger operator-() const {
    BigInteger result(*this);
    result.isNegative = 1 - result.isNegative;
    return result;
  }

  BigInteger& operator++() {
    *this += 1;
    return *this;
  }

  BigInteger& operator--() {
    *this -= 1;
    return *this;
  }

  BigInteger operator++(int) {
    BigInteger copy(*this);
    *this += 1;
    return copy;
  }

  BigInteger operator--(int) {
    BigInteger copy(*this);
    *this -= 1;
    return copy;
  }

  [[nodiscard]] std::string toString() const {
    std::string res;
    if (isNegative && (digits.size() != 1 || digits[0] != 0)) {
      res = '-';
    }
    res += std::to_string(digits.back());
    for (int64_t i = static_cast<int64_t>(digits.size()) - 2; i >= 0; --i) {
      std::string str = std::to_string(digits[i]);
      res += std::string(power - str.size(), '0') + str;
    }
    return res;
  }

  explicit operator bool() const {
    return digits.size() != 1 || digits[0] != 0;
  }

  friend std::ostream& operator<< (std::ostream& os, const BigInteger& number);
  friend std::istream& operator>> (std::istream& in, BigInteger& number);
  friend class Rational;

 private:
  void inplacePlus(const BigInteger& other, bool isMinus) {
    bool negateRes = false;
    bool negateLeft = false;
    bool negateRight = false;
    if (isNegative && (other.isNegative ^ isMinus)) {
      negateRes = true;
    } else if (isNegative && !(other.isNegative ^ isMinus)) {
      if (!isAbsLowerThan(other)) {
        negateRes = true;
        negateRight = true;
      } else {
        negateLeft = true;
      }
    } else if (!isNegative && (other.isNegative ^ isMinus)) {
      if (isAbsLowerThan(other)) {
        negateRes = true;
        negateLeft = true;
      } else {
        negateRight = true;
      }
    }
    int64_t carry = 0;
    size_t index = 0;
    for (; index < std::min(digits.size(), other.digits.size()); ++index) {
      int64_t sum = digits[index] * (negateLeft? -1LL: 1LL) +
          other.digits[index] * (negateRight? -1LL: 1LL) +
          carry;
      digits[index] = modulo(sum, base);
      carry = (sum >= base) - (sum < 0);
    }
    for (; index < digits.size(); ++index) {
      int64_t sum = digits[index] * 1LL + carry;
      digits[index] = modulo(sum, base);
      carry = (sum >= base) - (sum < 0);
      if (carry == 0) {
        break;
        // all next digits won't change
      }
    }
    for (; index < other.digits.size(); ++index) {
      int64_t sum = other.digits[index] * 1LL + carry;
      digits.push_back(modulo(sum, base));
      carry = (sum >= base) - (sum < 0);
    }
    if (carry) {
      digits.push_back(carry);
    }
    isNegative = negateRes;
    clearLeadingZeros();
  }

  void clearLeadingZeros() {
    for (int64_t rightPtr = static_cast<int64_t>(digits.size()) - 1; rightPtr > 0; --rightPtr) {
      if (digits[rightPtr] == 0) {
        digits.erase(digits.begin() + rightPtr);
      } else {
        break;
      }
    }
    if (digits.size() == 1 && digits[0] == 0) {
      isNegative = false;
    }
  }

  [[nodiscard]] bool isAbsLowerThan(const BigInteger& other) const {
    if (digits.size() > other.digits.size()) {
      return false;
    }
    if (digits.size() < other.digits.size()) {
      return true;
    }
    for (int64_t i = static_cast<int64_t>(digits.size()) - 1; i >= 0; --i) {
      if (digits[i] > other.digits[i]) {
        return false;
      }
      if (digits[i] < other.digits[i]) {
        return true;
      }
    }
    return false;
  }

  static int64_t modulo(int64_t a, int64_t b) {
    int64_t res = a % b;
    if (res < 0) {
      return base + res;
    }
    return res;
  }

  [[nodiscard]] BigInteger fastMultiplication(int64_t b) const {
    BigInteger result(*this);
    int64_t carry = 0;
    for (int32_t& digit : result.digits) {
      int64_t cur = carry + digit * b;
      digit = cur % base;
      carry = cur / base;
    }
    while (carry) {
      result.digits.push_back(carry % base);
      carry /= base;
    }
    result.clearLeadingZeros();
    return result;
  }

  void scholarMul(const BigInteger& other) {
    BigInteger result;
    result.digits.resize(digits.size() + other.digits.size());
    for (size_t leftInd = 0; leftInd < digits.size(); ++leftInd) {
      int64_t carry = 0;
      for (size_t rightInd = 0; rightInd < other.digits.size() || carry != 0; ++rightInd) {
        int64_t cur;
        if (rightInd >= other.digits.size()) {
          cur = result.digits[leftInd + rightInd] * 1LL + carry;
        } else {
          cur = result.digits[leftInd + rightInd] * 1LL + (digits[leftInd] * 1LL) * other.digits[rightInd] + carry;
        }
        result.digits[leftInd + rightInd] = cur % base;
        carry = cur / base;
      }
    }
    result.clearLeadingZeros();
    *this = result;
  }

  static size_t rev (size_t num, size_t log) {
    size_t mask = 0;
    for (size_t level = 0; level < log; ++level) {
      if (num & (1ull << level)) {
        mask |= 1ull << (log - level - 1);
      }
    }
    return mask;
  }

  static void fft(Complex* A, size_t n, bool isDirect = true) {
    size_t log = 63 - __builtin_clzll(n);

    for (size_t i = 0; i < n; ++i) {
      if (i < rev(i, log)) {
        std::swap(A[i], A[rev(i, log)]);
      }
    }

    for (size_t size = 2; size <= n; size <<= 1) {
      long double ang = PI / size * 2;
      if (!isDirect) {
        ang = -ang;
      }
      std::vector<Complex> precalcedCoeff(size / 2);
      for (size_t i = 0; i < size / 2; ++i) {
        precalcedCoeff[i] = Complex(cosl(ang * i), sinl(ang * i));
      }
      for (size_t leftInd = 0; leftInd < n; leftInd += size) {
        for (size_t rightInd = 0; rightInd < size / 2; ++rightInd) {
          Complex v1 = A[leftInd + rightInd];
          Complex v2 = A[leftInd + rightInd + size / 2] * precalcedCoeff[rightInd];
          A[leftInd + rightInd] = v1 + v2;
          A[leftInd + rightInd + size / 2] = v1 - v2;
        }
      }
    }
    if (!isDirect) {
      for (size_t i = 0; i < n; ++i) {
        A[i] /= n;
      }
    }
  }

  static void fftMultiplication(const std::vector<int32_t>& a, const std::vector<int32_t>& b,
                                std::vector<int32_t>& res) {
    std::vector<Complex> A(a.begin(), a.end());
    std::vector<Complex> B(b.begin(), b.end());
    size_t n = 1ull << (33 - __builtin_clz(static_cast<uint32_t>(a.size() + b.size())));
    A.resize(n);
    B.resize(n);
    fft(A.data(), n, true);
    fft(B.data(), n, true);
    for (size_t i = 0; i < A.size(); ++i) {
      A[i] *= B[i];
    }

    fft(A.data(), n, false);
    res.assign(n, 0);
    for (size_t i = 0; i < n; ++i) {
      int64_t cur = roundl(A[i].real);
      size_t iteration = 0;
      while (cur != 0) {
        res[i + iteration] += cur % base;
        cur /= base;
        cur += res[i + iteration] / base;
        res[i + iteration] %= base;
        iteration++;
      }
    }
  }

  static int64_t binPow(int64_t base_, int64_t exp) {
    int64_t result = 1;
    while (exp) {
      if (exp % 2)
        result *= base_;
      exp /= 2;
      base_ *= base_;
    }
    return result;
  }

  static const uint8_t power = 6;
  static const int64_t base = 1000000;  // binPow(10, power);

 public:
  std::vector<int32_t> digits;
  bool isNegative;
};

bool operator< (const BigInteger& left, const BigInteger& right) {
  if (left.isNegative && !right.isNegative) {
    return true;
  }
  if (!left.isNegative && right.isNegative) {
    return false;
  }
  bool absInequality = left.isAbsLowerThan(right);
  if (left.digits == right.digits) {
    return false;
  }
  return absInequality != left.isNegative;
}

bool operator== (const BigInteger& left, const BigInteger& right) {
  if (left.isNegative != right.isNegative) {
    return false;
  }
  if (left.digits.size() != right.digits.size()) {
    return false;
  }
  for (size_t i = 0; i < left.digits.size(); ++i) {
    if (left.digits[i] != right.digits[i]) {
      return false;
    }
  }
  return true;
}

bool operator<= (const BigInteger& left, const BigInteger& right) {
  return left < right || left == right;
}

bool operator> (const BigInteger& left, const BigInteger& right) {
  return right < left;
}

bool operator>= (const BigInteger& left, const BigInteger& right) {
  return left > right || right == left;
}

bool operator!= (const BigInteger& left, const BigInteger& right) {
  return !(left == right);
}

BigInteger operator* (const BigInteger& left, const BigInteger& right) {
  BigInteger result(left);
  result *= right;
  return result;
}

BigInteger operator/ (const BigInteger& left, const BigInteger& right) {
  BigInteger result(left);
  result /= right;
  return result;
}

BigInteger operator+ (const BigInteger& left, const BigInteger& right) {
  BigInteger result(left);
  result += right;
  return result;
}

BigInteger operator- (const BigInteger& left, const BigInteger& right) {
  BigInteger result(left);
  result -= right;
  return result;
}

BigInteger& operator%= (BigInteger& left, const BigInteger& right) {
  BigInteger copy(left / right);
  left -= copy * right;
  return left;
}

BigInteger operator% (const BigInteger& left, const BigInteger& right) {
  BigInteger result(left);
  result %= right;
  return result;
}

std::ostream& operator<< (std::ostream& os, const BigInteger& number) {
  os << number.toString();
  return os;
}

std::istream& operator>> (std::istream& in, BigInteger& number) {
  std::string str;
  in >> str;
  number = BigInteger(str);
  return in;
}

BigInteger operator "" _bi (const char* x) {
  return BigInteger(x);
}

class Rational {
 public:
  Rational() {
    numerator = 0;
    denominator = 1;
  }

  Rational (const BigInteger& num, const BigInteger& den) {
    bool isNegative = num.isNegative ^ den.isNegative;
    numerator = num;
    denominator = den;
    denominator.isNegative = false;
    numerator.isNegative = isNegative;
    reduction();
  }

  Rational (int64_t integer) {
    numerator = integer;
    denominator = 1;
  }

  Rational (const BigInteger& integer) {
    numerator = integer;
    denominator = 1;
  }

  Rational (const Rational& other) {
    numerator = other.numerator;
    denominator = other.denominator;
  }

  Rational& operator+= (const Rational& other) {
    numerator = numerator * other.denominator + other.numerator * denominator;
    denominator *= other.denominator;
    reduction();
    return *this;
  }

  Rational& operator-= (const Rational& other) {
    numerator = numerator * other.denominator - other.numerator * denominator;
    denominator *= other.denominator;
    reduction();
    return *this;
  }

  Rational& operator*= (const Rational& other) {
    numerator *= other.numerator;
    denominator *= other.denominator;
    reduction();
    return *this;
  }

  Rational& operator/= (const Rational& other) {
    if (this == &other) {
      *this = 1;
      return *this;
    }
    bool sign = numerator.isNegative ^ other.numerator.isNegative;
    numerator *= other.denominator;
    denominator *= other.numerator;
    numerator.isNegative = sign;
    denominator.isNegative = false;
    reduction();
    return *this;
  }

  Rational& operator= (const Rational& other) {
    numerator = other.numerator;
    denominator = other.denominator;
    return *this;
  }

  Rational operator-() const {
    Rational result(*this);
    result.numerator.isNegative = 1 - result.numerator.isNegative;
    return result;
  }

  [[nodiscard]] std::string toString() const {
    std::string res = numerator.toString();
    if (denominator != 1) {
      res += "/" + denominator.toString();
    }
    return res;
  }

  [[nodiscard]] std::string asDecimal(size_t precision = 0) const {
    BigInteger exp = binPow(BigInteger(10), BigInteger(precision));
    BigInteger shifted = numerator * exp;
    shifted /= denominator;
    shifted.isNegative = false;
    std::string res = shifted.toString();
    size_t integer = (numerator / denominator).toString().size() - numerator.isNegative;
    res = std::string(precision + integer - res.size() + (integer == 0), '0') + res;
    if (precision != 0) {
      res.insert(res.size() - precision, ".");
    }
    if (numerator.isNegative) {
      res = '-' + res;
    }
    return res;
  }

  friend bool operator< (const Rational& left, const Rational& right);
  friend bool operator== (const Rational& left, const Rational& right);

  explicit operator double() const {
    return std::stod(asDecimal(20));
  }

 private:
  void reduction() {
    BigInteger greatestCommonDivisor = gcd(numerator, denominator);
    if (greatestCommonDivisor == 1) {
      return;
    }
    numerator /= greatestCommonDivisor;
    denominator /= greatestCommonDivisor;
  }

  static BigInteger gcd (BigInteger first, BigInteger second) {
    first.isNegative = second.isNegative = false;
    while (second) {
      first %= second;
      std::swap(first, second);
    }
    return first;
  }

  static BigInteger binPow (BigInteger base_, BigInteger exp) {
    BigInteger result = 1;
    while (exp) {
      if (exp % 2)
        result *= base_;
      exp /= 2;
      base_ *= base_;
    }
    return result;
  }

 private:
  BigInteger numerator, denominator;
};

Rational operator+ (const Rational& left, const Rational& right) {
  Rational result(left);
  result += right;
  return result;
}

Rational operator- (const Rational& left, const Rational& right) {
  Rational result(left);
  result -= right;
  return result;
}

Rational operator* (const Rational& left, const Rational& right) {
  Rational result(left);
  result *= right;
  return result;
}

Rational operator/ (const Rational& left, const Rational& right) {
  Rational result(left);
  result /= right;
  return result;
}

bool operator< (const Rational& left, const Rational& right) {
  return (left - right).numerator < 0;
}

bool operator== (const Rational& left, const Rational& right) {
  return left.numerator == right.numerator && left.denominator == right.denominator;
}

bool operator<= (const Rational& left, const Rational& right) {
  return left < right || left == right;
}

bool operator> (const Rational& left, const Rational& right) {
  return right < left;
}

bool operator>= (const Rational& left, const Rational& right) {
  return left > right || right == left;
}

bool operator!= (const Rational& left, const Rational& right) {
  return !(left == right);
}
