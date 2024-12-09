#include "lexer.hpp"
#include <iostream>
#include <sstream>
#include <vector>

const std::vector<std::string> EXAMPLES = {
    // Ex1
    R"(// These are one line comments.
/* these
are
multiline
comments */)",
    // Ex2
    R"(
let a = 12; // int - const
let mut b = 12.12; // flt - mutowalny
let c = "duck"; // str - const
let mut d = true; // bol - mutowalny
)",
    // Ex3
    R"(
let mut a = 0;

// zmiana na stringa
$a = 1 <- str; // "1"
$a = 1.12 <- str; // "1.12"
$a = true <- str; // "true"

// zmiana na float
$a = 12 <- flt; // 12.0
$a = true <- flt; // 1.0

// zmiana na inta
$a = 1.2 <- int; // 1
$a = true <- int; // 1

// zmiana na boola
$a = 1 <- bol; // true
)",
    // Ex4
    R"(
let mut a = 1;
let mut b = 2;
a + b; // 3
<<<<<<< HEAD
a <- flt + b <- flt; / 3.0
=======
a <- flt + b <- flt; // 3.0
>>>>>>> temp_lexer
$a = "Hello"; $b = " World";
a + b; // "Hello World"
)",
    // Ex5
    R"(
let mut a = 1;
let mut b = 2;

a+b; // 3 ok.
a-b; // -1 ok.
a*b; // 2 ok.
a/b; // 0 ok.
a%b; // 0 ok.
)",
    // Ex6
    R"(
fn example(arg1, arg2){}

print(1); // 1
print("Hello world"); // Hello world
print(1.12); // 1.12
print(true); // true
print(example); // function(_,_)
)",
    R"(
let mut result = 0;
if (true) {
    $result = 1;
}
print(result); // 1
)",
    R"(
let mut result = 0;
if (false) {
    $result = 1;
} else {
    $result = 2;
}
print(result); // 2
)",
    R"(
let mut i = 1;

while (i < 10) {
    $i = i + 1;
}
print(i); // 10
)",
    R"(
fn example(arg1, arg2) {
    // $arg1 = 12; // błąd
    let mut a = arg1;
    $a = 12; // ok, `a` to inna zmienna
}
)",
    R"(
fn example(arg1, arg2) {
    $arg1 = 12; // ok
}
)",
    R"(
fn add_one(num) {
    let a = num + 1;
    return a;
}
let a = 1;
let b = add_one(a); // 2
)",
    R"(
fn increment(mut a) {
    a = a + 1;
    return a;
}
let a = 1;
let b = increment(a);
print(a); // 2
print(b); // 2
)",
    R"(
let mut a = 0;

fn increment(mut a) {
    $a = a + 1;
    return a;
}
fn add_one(a) {
    return a + 1;
}

if (a != 0) {
    $a = increment;
} else {
    $a = add_one;
}
let mut b = 12;
let c = a(b);
print(b); // 12
print(c); // 13
)",
    R"(
fn fibonacci(num) {
    if (num <= 1) {
        return num;
    } else {
        return fibonacci(num - 1) + fibonacci(num - 2);
    }
}
let a = fibonacci(5); // 5
)",
    R"(
fn add_one(a) {
    return a + 1;
}
let a = add_one(1); // 2
let b = #(x){ return x+1; }(1); // 2
let c = #(x){ return x+1; };
let d = c(1); // 2
)",
    R"(
fn is_prime(num) {
    let is_prime_rec = #(n, devisor) {
        if (n <= 1) { return false; }
        else {
            if (divisor == 1) { return true; }
            else {
                if (n % divisor == 0) { return false; }
                else { return is_prime_rec(n, divisor - 1); }
            }
        }
    }
    return is_prime_rec(num, num-1);
}
print(is_prime(7)); // true
print(is_prime(10));  // false
)",
    R"(
fn double(num) {
    print(num);
    let x = 2*num;
    x;
    return x;
}
[double](3);

/*
// Wynik na wyjściu standardowym:
[input: 3]
3
[]
[6]
[return: 6]
*/
)",
    R"(
fn is_prime(num) {
    let is_prime_rec = #(n, devisor) {
        n;
        devisor;
        if (n <= 1) { return false; }
        else {
            if (divisor == 1) { return true; }
            else {
                if (n % divisor == 0) { return false; }
                else { return [is_prime_rec](n, divisor - 1); }
            }
        }
    }
    let res = is_prime_rec(num, num-1);
    return res;
}
print(is_prime(10));

/*
// Wynik na wyjściu standardowym:
[input: 10, 8]
[10]
[8]
[input: 10, 7]
[10]
[7]
[input: 10, 6]
[10]
[6]
[input: 10, 5]
[10]
[5]
[return: false]
[return: false]
[return: false]
[return: false]
[return: false]
false
*/
)",
};

// helper
std::vector<pheonix::lexer::Lexem> lexerize(pheonix::lexer::Lexer &lexer) {
  std::vector<pheonix::lexer::Lexem> result;
  while (true) {
    pheonix::lexer::Lexem l = lexer.nextLexem();
    result.emplace_back(l);
    if (l.token.getTokenType() == pheonix::types::TokenType::END_OF_FILE) {
      return result;
    }
  }
}

int main(int argc, char **argv) {
  if (argc == 2) {
    size_t test_case = 1;
    test_case = std::atoi(argv[1]) - 1;
    if (test_case >= EXAMPLES.size()) {
      std::cout << "wrong test case use index from [0, " << EXAMPLES.size()
                << '.' << std::endl;
      return 1;
    }
    std::istringstream in(EXAMPLES[test_case]);
    pheonix::lexer::Lexer l(in);
    std::vector<pheonix::lexer::Lexem> result = lexerize(l);
    std::cout << "Example nr. " << ++test_case << std::endl;
    for (const auto &lexem : result) {
      std::cout << lexem << std::endl;
    }
    std::cout << std::endl;
    return 0;
  }

  int i = 0;
  for (const auto &ex : EXAMPLES) {
    std::istringstream in(ex);
    pheonix::lexer::Lexer l(in);
    std::vector<pheonix::lexer::Lexem> result = lexerize(l);
    std::cout << "Example nr. " << ++i << std::endl;
    for (const auto &lexem : result) {
      std::cout << lexem << std::endl;
    }
    std::cout << std::endl;
  }
}
