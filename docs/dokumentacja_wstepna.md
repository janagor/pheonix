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
* typy danych:
   * całkowitoliczbowy (`int`),
   * zmiennopozycyjny (`dbl`),
   * logiczny (`bol`)
   * ciąg znaków (`str`),
   * funkcje,
* operatory przyjmujące funkcje jako argument:
   * `@` - zwraca liczbę argumentów podanych do funkcji,
   * `#[]` - wywołuje funkcję wielokrotnie razy.
   Zwraca wynik ostatniego wywołania.

# Formalna specyfikacja i składnia **EBNF** realizowanego języka
``` EBNF
                  program = { statement } ;

                statement = function_declaration
                          | variable_declaration
                          | while_loop_statement
                          | if_statement
                          | return_statement
                          | expression_statement ;

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
                            "(" relational_expression ")"
                            "{" { statement - function_declaration } "}" ;

              else_clause = "else"
                            (
                            if_statement
                            | ( "{" { statement - function_declaration } "}" )
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

multiplicative_expression = execute_expression
                            { ( "*" | "/" )
                            execute_expression } ;

       execute_expression = cast_expression # expression "[" expression_list "]" ;

          cast_expression = prefix_expression "<-" type_name ;

        prefix_expression = "@" expression
                          | "!" primary_expression ;

       primary_expression = identifier { "(" expression_list ")" { "(" expression_list ")" } } ;
                          | literal
                          | "(" expression ")" { "(" expression_list ")" } ;

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

                type_name = "bol" | "dbl" | "int" | "str" ;

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
 * SINGLE_QUOTE, DOUBLE_QUOTE, DOUBLE_QUOTE, SEMICOLON, COMMA, DOT, RARROW,
 LARROW,
 * IDENTIFIER, STRING, INTEGER, DOUBLE,
 * FN, LET, MUT, RETURN,
 * IF, ELSE, WHILE,
 * INT, STR, DBL, BOL,
 * TRUE, FALSE,
 * NOT_A_KEYWORD,

Lista słów kluczowych:

 * fn, let, return, mut,
 * if, else, while,
 * int, str, bol, dbl,
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
let mut a = 1 <- str; // "1"
$a = 1.12 <- str; // "1.12"
$a = true <- str; // "true"
$a = 1 <- dbl; // 1.0
$a = 1.21 <- int; // 1
$a = "1.12" <- dbl; // 1.12
$a = "1.12" <- int; // 1
$a = "true" <- bol; // true
// $a = "1" <- bol; // błąd, typ bol nie jest reprezentowany przez liczbę.
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
```

6. Funkcja **print()**

```
fn example(arg1, arg2){}

print(1); // 1
print("Hello world"); // Hello world
print(1.12); // 1.12
print(true); // true
print(example); // fn: example(_, _)
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
    // ...
}
```

11. Deklaracja funkcji z parametrami mutowalnymi

```
fn example(arg1, arg2) {
    $arg1 = 12; // ok
    // ...
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

12. Argumenty funkcji są przekazywane przez referencję

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

13. Funkcje jako zmienne
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

14. Operator `@`

```
fn has_two_args(first, second) {
    // ...
    if (@has_two_args != 2) {
        print("Should have given 2 args");
    } else {
        print("given args are: " + first<-str + " and " + second<-str);
    }
}
has_two_args(12); // "Should have given 2 args"
has_two_args(12, 2); // "given args are 12 and 2"
```

15. Operator `#` z funkcją bez argumentów mutowalnych bez zwracanej wartości

```
fn show(a) {
    print(a);
}
3 # show("Hello"); // HelloHelloHello
/* równoznaczne z: show("Hello");show("Hello");show("Hello");
```

16. Operator `#` z funkcją bez argumentów mutowalnych ze zwracaną wartością

```
fn add_one(a) {
    return a + 1;

}
let a = 3#add_one[1]; // 1
// równoznacze z: add_one(1);add_one(1); let a = add_one(1);
```

17. Operator `#` z funkcją z argumentem mutowalnym

```
fn inc_and_ret(mut a) {
    return $a = a + 1;

}
let mut a = 0;
let b = 3#inc_and_return[a];
// równoznacze z: inc_and_ret(a);inc_and_ret(a);let b = inc_and_ret(a);
print(b); // 3
print(3); // 3
```


18. Rekurencja

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

19. Przykład wykorzystania operatora `# [ ]`

```
let one_time_payment=100;
let mut current_sum=0;

fn notify_user(payment) {
    print("A payment of " +
          payment<-str +
          " has been made. The balance in your account is" + 
          current_sum<-str);
}
fn make_deposit(value) {
    $current_sum = current_sum + value;
    notify_user(payment);
    return current_sum;
}
// making many small deposits with notification
let balance = 5 # make_deposit[one_time_payment]; // balance==500
// A payment of 100 has been made. The balance in your account is 100.
// A payment of 100 has been made. The balance in your account is 200.
// A payment of 100 has been made. The balance in your account is 300.
// A payment of 100 has been made. The balance in your account is 400.
// A payment of 100 has been made. The balance in your account is 500.

```
19. Przykład wykorzystania operatora `@`

```
func handle_event(type, x, y) {
    if (@handle_event == 1) {
        print("Got event: " + typ<-str)
    }
    if (@handle_event == 2) {
        print("Got event: " + typ<-str + " at x=" + x<-str)
    }
    if (@handle_event == 3) {
        print("Got event: " + typ<-str + " at x=" + x<-str + ", y=" + y<-str)
    }
}

// Przykłady użycia
handle_event("click");             // "Got event: click"
handle_event("move", 100); //  "Got event: move at x=100"
handle_event("touch", 120, 250); // "Got event: touch at x=120, y=250"
```

