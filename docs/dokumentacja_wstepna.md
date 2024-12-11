# Techniki kompilacji 2024Z
## Jan Górski

# Informacje podstawowe
Tematem projektu jest napisanie interpretera języka programowania ogólnego
przeznaczenia.

* Język wykorzystany w projekcie: C++
* Wykorzystane biblioteki:
    * GoogleTest.

# Opis zakładanej funkcjonalności
* kod programu napisane w języku ewaluowany linijka po linijce - brak punku
wejściowego, jak np. funkcja `main`.
* wartości są domyślnie stałe: wartość zmiennej zadeklarowanej bez użycia słowa
kluczowego `mut` nie może być zmieniona;
* typowanie dynamiczne - zmienna może w czasie działania programu zmienić swój
typ,
* typowanie silne - zmienna w każdej chwili działania programu ma przypisany
typ,
* zmienne są przekazywanie przez referencję (domyślnie stałą),
* pętla `while`,
* instrukcje warunkowe `if`, `else`,
* funkcje są obiektami pierwszej klasy: mogą być przekazywana jako parametr
innej funkcji, przypisywane do zmiennych itd.,
* funkcje nie mogą być przeciążone,
* typy danych:
   * całkowitoliczbowy (`int`),
   * zmiennopozycyjny (`flt`),
   * logiczny (`bol`)
   * ciąg znaków (`str`),
   * funkcje,
* operatory przyjmujące funkcje jako argument:
   * `|` - operator tworzący kompozycję funkcji.
   * `#(){}` - wyrażenie `lambda`.
   * _opcjonalnie_: `[]()` - operator `debug` - wyświetla informacje dotyczące wywołania funkcji.

# Formalna specyfikacja i składnia **EBNF** realizowanego języka
``` EBNF
                  program = { statement } ;

                statement = function_declaration
                          | variable_declaration
                          | while_loop_statement
                          | if_statement
                          | return_statement
                          | expression_statement
                          | ";" ;

     variable_declaration = "let" [ "mut"] identifier [ "=" expession ] ";" ;

     while_loop_statement = "while"
                            "(" expression ")"
                            "{"  { statement } "}" ;

         return_statement = "return" [ expression ] ";" ;

     expression_statement = [ expression ] ";" ;

               expression = assignement_expression 
                          | or_expression ;

          expression_list = [ expression { "," expression } ] ;

            if_statement = if_clause [ else_clause ] ;

                if_clause = "if"
                            "(" expression ")"
                            "{" { statement } "}" ;

              else_clause = "else"
                            (
                            if_statement
                            | ( "{" { statement } "}" )
                            ) ;

   assignement_expression = "$" identifier "=" or_expression ;

            or_expression = and_expression
                            { "||" and_expression } ;

           and_expression = comparison_expression
                            { "&&" comparison_expression } ;

    comparison_expression = relational_expression
                            { ( "==" | "!=" )
                            relational_expression } ;

    relational_expression = additive_expression
                            { ( "<" | ">" | "<=" | ">=" )
                            additive_expression } ;

      additive_expression = multiplicative_expression
                            { ( "+" | "-" )
                            multiplicative_expression } ;

multiplicative_expression = compositive_expression
                            { ( "*" | "/" | "%" )
                            compositive_expression } ;

   compositive_expression = cast_expression
                            { "|" cast_expression } ;

          cast_expression = prefix_expression { "<-" type_name } ;

        prefix_expression = "!" primary_expression
                          | "-" primary_expression
                          | primary_expression ;

       primary_expression = ( 
                            identifier
                            | ( "#" enclosed_parameter_list function_body )
                            )
                            { "(" expression_list ")" } ;
                          | "(" expression ")" { "(" expression_list ")" }
                          | "[" expression "]" "(" expression_list ")" { "(" expression_list ")" }
                          | literal ;


     function_declaration = "fn" identifier
                            enclosed_parameter_list function_body ;

  enclosed_parameter_list = "(" parameter_list ")" ;

           parameter_list = [
                            [ "mut" ] identifier
                            { "," [ "mut" ] identifier }
                            ] ;

            function_body = "{" 
                            { statement - function_declaration }
                            "}" ;

               identifier = letter { letter | digit | "_" } ;

                  literal = bool_literal
                          | float_literal
                          | integer_literal
                          | string_literal ;

                type_name = "bol" | "flt" | "int" | "str" ;

             bool_literal = "true" | "false" ;

            float_literal = integer_literal "." { digit } ;

          integer_literal = ( non_zero_digit { digit } ) | "0" ;

           string_literal = """ {
                            special_string_character | normal_string_character
                            } """ ;

  normal_string_character = character - ( """ | "\" ) ;

 special_string_character = "\"" | "\n" | "\\" | "\b";

                character = alphanumeric_character
                          | special_character
                          | white_character ;

          white_character = "\n" | "\t" | " " ; 

   alphanumeric_character = letter | digit ;

                   letter = "A" | "B" | "C" | "D" | "E" | "F" | "G"
                          | "H" | "I" | "J" | "K" | "L" | "M" | "N"
                          | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
                          | "V" | "W" | "X" | "Y" | "Z" | "a" | "b"
                          | "c" | "d" | "e" | "f" | "g" | "h" | "i"
                          | "j" | "k" | "l" | "m" | "n" | "o" | "p"
                          | "q" | "r" | "s" | "t" | "u" | "v" | "w"
                          | "x" | "y" | "z" ;

                    digit = "0" | non_zero_digit ;

           non_zero_digit = "1" | "2" | "3" | "4" | "5"
                          | "6" | "7" | "8" | "9" ;

        special_character = "~" | "!" | "@" | "#" | "$" | "%" | "^" | "&" | "*"
                          | "(" | ")" | "-" | "_" | "+" | "=" | "{" | "}" | "["
                          | "]" | "|" | "\" | ":" | ";" | """ | "'" | "<" | ">"
                          | "," | "." | "?" | "/"

                 new_line = ( "\n" | "\r" | "\r\n" ) ;

         one_line_comment = "//" { character - new_line } new_line ;

        multiline_comment = "/*" { ( character character ) - "*/" } "*/" ;


```

# Obsługa błędów, przykłady komunikatów

Na każdym etapie przetwarzania danych znajdowała się będzie struktura, do
której będą zapisywane komunikaty dotyczące wykrytych błędów. Na sam koniec
procesu interpetacji lub gdy wykryty błąd będzie niemożliwy do pominięcia
użytkownik zostanie powiadomiony o tym przez wyjście standardowe, a proces
interpretacji zakończy się błędem i nie zostanie ukończony.

# Sposób uruchomienia, wej/wyj.
``` shell
$ ./my-language --help
Usage: ./my-language [OPTIONS] INPUT
Options:
  -h [ --help ]                         produce help message
  -l [ --lexer ]                        create a file with lexer output (with
                                        .l extention)
  -p [ --parser ]                       create a file with result of syntax
                                        analysis (with .p extention)
  -s [ --semantic ]                     create a file with result of semantic
                                        analysis (with .s extention)
  -i [ --intermediate_representation ]  create a file with result of
                                        intermediate representation(with .ir
                                        extention)
```
# Lista tokenów:

 * ERROR, ERROR_NUMBER, ERROR_STRING, UNFINISHED_COMMENT,
 * END_OF_FILE,
 * ONE_LINE_COMMENT, MULTILINE_COMMENT,
 * PLUS, MINUS, STAR, SLASH, ASSIGN,
 * EQUALS, LESS, LEQ, GREATER, GEQ, OR, AND, NOT, AT, HASHTAG
 * LPARENT, RPARENT, LBRACE, RBRACE, LBRACKET, RBRACKET,
 * DOUBLE_QUOTE, SEMICOLON, COMMA, DOT, RARROW, LARROW,
 * IDENTIFIER, STRING, INTEGER, FLOAT,
 * FN, LET, MUT, RETURN,
 * IF, ELSE, WHILE,
 * INT, STR, FLT, BOL,
 * TRUE, FALSE,
 * NOT_A_KEYWORD,

Lista słów kluczowych:

 * fn, let, return, mut,
 * if, else, while,
 * int, str, bol, flt,
 * true, false.

Lista węzłów drzewa AST:

 * planowanym jest, aby lista węzłów drzewa AST w znaczącym stopniu odwzorywała
 elementy występujące w składni **EBNF** języka.


# Wykorzystane struktury danych

* tablica dynamiczna,
* graf acykliczny (drzewo),
* tablica symboli,

# Przykład wykorzystania struktury danych

* tablica dynamiczna: przechowywanie lexemów, przechowywanie informacji o
błędach,
* graf acykliczny (drzewo): struktura w analizatorze składniowym.
* tablica symboli: dla każdego `zasięgu` (bloku kodu) oddzielna tablica mająca
postać hash-mapy. Same zaś zasięgi będą ustrukturyzowane poprzez użycie stosu
zasięgów.

#### Formy pośrednie

1. Kod źródłowy -> analizator leksykalny
   * analizator leksykalny pobiera wejściowe dane ze strumienia
   (`std::istream`)
1. analizator leksykalny -> analizator składniowy
   * analizator składniowy pobiera obiekty typu `Leksem` (`tokeny`) z
   analizatora leksykalnego
1. analizator składniowy -> analizator semantyczny
   * analizator składniowy generuje drzewo `AST`.

# Opis sposobu testowania

Proces testowania będzie opierał się na stopniowym kolejkowym testowaniu -
zawsze wejściem testu będzie strumień znaków.

* Analizator leksykalny:
   * wejście - ciąg znaków do przeprowadzenia analizy leksykalnej,
   * wyjście - tablica leksemów + tablica błędów,
   * test - porównywanie poszczególnych wartości leksemów i błędów wynikowych
   z oczekiwanymi,
* Analizator składniowy:
   * wejście - ciąg znaków do przeprowadzenia analizy leksykalnej,
   * wyjście - postać drzewa wyjścia parsera sprowadzona do postaci
   wyświetlalnej (jako `string`),
   * test - porównanie wyjścia z oczekiwanym wynikiem.


# Przykłady obrazujące dopuszczalne konstrukcje językowe i semantykę

1. Komentarze

 ```
// These are one line comments.
/* these
are
multiline
comments */
 ```

2. Podstawowe typy danych

 ```
let a = 12; // int - const
let mut b = 12.12; // flt - mutowalny
let c = "duck"; // str - const
let mut d = true; // bol - mutowalny
 ```

3. Zmiana typu danych

```
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
```

4. Dodawanie zmiennych

```
let mut a = 1;
let mut b = 2;
a + b; // 3
a <- flt + b <- flt; / 3.0
$a = "Hello"; $b = " World";
a + b; // "Hello World"
```

5. Podstawowe operacje matematyczne

```
let mut a = 1;
let mut b = 2;

a+b; // 3 ok.
a-b; // -1 ok.
a*b; // 2 ok.
a/b; // 0 ok.
a%b; // 0 ok.
```

6. Funkcja **print()**

Funkcja print jest funkcją wbudowaną. Przyjmuje ona 1 argument jednego z typów:
`str`, `int`, `bol`, `flt` lub funkcję i wyświetla ją na wyjście standardowe.

```
fn example(arg1, arg2){}

print(1); // 1
print("Hello world"); // Hello world
print(1.12); // 1.12
print(true); // true
print(example); // function(_,_)
```

7. Instrukcje warunkowe : if

```
let mut result = 0;
if (true) {
    $result = 1;
}
print(result); // 1
```

8. Instrukcje warunkowa: if else
```

let mut result = 0;
if (false) {
    $result = 1;
} else {
    $result = 2;
}
print(result); // 2
```

9. Pętla `while`

```
let mut i = 1;

while (i < 10) {
    $i = i + 1;
}
print(i); // 10
```

10. Deklaracja funkcji z parametrami niemutowalnymi

```
fn example(arg1, arg2) {
    // $arg1 = 12; // błąd
    let mut a = arg1;
    $a = 12; // ok, `a` to inna zmienna
}
```

11. Deklaracja funkcji z parametrami mutowalnymi

```
fn example(arg1, arg2) {
    $arg1 = 12; // ok
}
```

12. Funkcje zwracające wartość

```
fn add_one(num) {
    let a = num + 1;
    return a;
}
let a = 1;
let b = add_one(a); // 2
```

13. Argumenty funkcji są przekazywane przez referencję

```
fn increment(mut a) {
    a = a + 1;
    return a;
}
let a = 1;
let b = increment(a);
print(a); // 2
print(b); // 2
```

14. Funkcje jako zmienne
```
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
```

15. Rekurencja

```
fn fibonacci(num) {
    if (num <= 1) {
        return num;
    } else {
        return fibonacci(num - 1) + fibonacci(num - 2);
    }
}
let a = fibonacci(5); // 5
```

17. Operator `#(){}`

Operator `#(){}` zwraca funkcję anonimową, która przyjmuje argumenty, których
identyfikatory, używane wewnątrz ciała funkcji są wskazane wewnątrz nawiasów
`()`. Ciało funkcji jest określane wewnątrz nawiasów klamrowych `{}`. Zwracaną
funkcję można przypisać do zmiennej. Możliwe jest wywołanie rekursywne wewnątrz
ciała funkcji. 

```
fn add_one(a) {
    return a + 1;
}
let a = add_one(1); // 2
let b = #(x){ return x+1; }(1); // 2
let c = #(x){ return x+1; };
let d = c(1); // 2
```

18. Przykład wykorzystania operatora `#(){}`

```
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
```

19. Operator `[]()`

Operator `[]()` służy do wyświetlenia wartości `instrukcji-wyrażeń`, tj.
wyświetlenia wyrażeń zakończonych znakiem `;`, które interpreter napotka w
ciele funkcji. Operator nie działa rekurencyjnie - wywołanie funkcji `fx();` wewnątrz
ciała funkcji `f2` podczas operacji `[f2]();` nie bedzię wyświetlać wartości
`instrukcji-wyrażeń` znajdujących się wewnątrz `fx`. Wyświetli to wyłącznie
wartość zwracaną przez wywołanie `fx()`. Operator wyświetla
argumenty podane do funkcji i wartość przez nią zwracaną. Jeżeli `instrukcja-wyrażenie`
nie zwraca wartości, to jest to widoczne wewnątrz nawiasów kwadratowych `[]`.

```
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
```

20. Przykład rekurencji w operatorze `[]()`

```
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
```

21. Operator '|`

Operator `|` służy do tworzenia kompozycji funkcji. Załóżmy, że `f1` i `f2` są
funcjami, gdzie, `f1` przyjmują listy argumentów odpowiednio `A1` i `A2` oraz
zwracają odpowiednio `B1` i `B2`. Wyrażenie `f1|f2` jest równoznaczne z 
wyrażeniem lambda: `let f3 = #(**A1**){return f2(f1(A1));};`.


# Przykładowe typy komunikatów o błędzię

* dodanie dwóch zmiennych różnych typów (wiersz 10, kolumna 12)
    * `[10:12] error: conficting types using "+" operator.`
* wywołanie funkcji, która przyjmuje 2 argumenty, z 3 argumentami (wiersz 10, kolumna 12)
    * `[10:12] error: expected '2' arguments, got '3'.`
* próba zmiany wartości zmiennej niemutowalnej (wiersz 10, kolumna 12)
    * `[10:12] error: variable is read-only.`
* próba redeklaracji zmiennej (wiersz 10, kolumna 12)
    * `[10:12] error: variable cannot be redeclared.`
* zbyt długa nazwa zmiennej (wiersz 10, kolumna 12)
    * `[10:12] error: identifier to long.`
* overflow (wiersz 10, kolumna 12)
    * `[10:12] error: overflow encountered.`
* nieudana próba wykonania operacji kastowania ze `stringa` do `boola` (wiersz 10, kolumna 12)
    * `[10:12] error: conversion from 'str` to `bol`.`
