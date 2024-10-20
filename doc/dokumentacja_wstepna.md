# Techniki kompilacji 2024Z
## Jan Górski


### Informacje podstawowe

Tematem projektu jest napisanie interpretera własnego języka programowania.

* Język wykorzystany w projekcie: C++
* Wykorzystane biblioteki:
    * Boost.

### Opis zakładanej funkcjonalności

* silne typowanie,
* struktury,
    * wszystko globalne,
    * zawierają:
        * pola,
        * metody,
    * dziedziczenie pojedyncze.
* pętla `while`,
* instrukcje warunkowe `if`, `else`,
* przesłanianie,
* całkowitoliczbowe typy danych, typ logiczny `bool`.

### Przykłady obrazujące dopuszczalne konstrukcje językowe i semantykę
* w pliku `example.txt`.

### Formalna specyfikacja i składnia **EBNF** realizowanego języka

``` EBNF
TODO: I don't know if comments should be included in gramma of the language
       program = { features } ;
      features = { feature } main { feature } ;
       feature = {
                 structure
                 | global_variable
                 | function
                 | one_line_comment
                 | multiline_comment
                 } ;
    expression = something
                 | something_else
                 | something_else
                 | something_else
                 | something_else
                 | something_else
                 | something_else ;
    while_loop = "while" , S ,
                 "(" , S , condition , S , ")" , S ,
                 "{" , S , statements , S , "}";
  if_statement = if_clause , { S , else_if_clause } , [ S , else_clause ]
else_if_clause = "else" , S , if_clause ;
   else_clause = "else" , "{" , S , statements , S , "}" ;
     if_clause = "if" , S ,
                 "(" , S , condition , S , ")" , S ,
                 "{" , S , statements , S , "}" ;
     condition = identifier | "(" , S , condition , S , ")"
                 | identifier , S , relational_op , S , identifier ;

identifier_type = identifier , S , ":" , S , type ;
           type = identifier ;

     identifier = letter , { letter | digit | "_" } ;

  relational_op = "<" | ">" | "==" | "!=" | "<=" | ">=" ;

addition_expression = expression , S , "+" , S , expression ;

one_line_comment = "//" , {digit | letter | ( S - new_line ) } , new_line ;
multiline_comment = "/*" , { digit | letter | S } , "*/" ;

     letter = "A" | "B" | "C" | "D" | "E" | "F" | "G"
              | "H" | "I" | "J" | "K" | "L" | "M" | "N"
              | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
              | "V" | "W" | "X" | "Y" | "Z" | "a" | "b"
              | "c" | "d" | "e" | "f" | "g" | "h" | "i"
              | "j" | "k" | "l" | "m" | "n" | "o" | "p"
              | "q" | "r" | "s" | "t" | "u" | "v" | "w"
              | "x" | "y" | "z" ;
      digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
   new_line = ( "\n" | "\r" | "\r\n" ) ;
          S = { " " | "\n" | "\t" | "\r" } ;








   function = "fn" , S , identifier , S ,
              "(" , [ identifier_type , { S , "," , S , identifier_type } S , identifier_type ] ")" , S ,
              [ "->" , S , type, S ]
              "{" , S, statements, S , "}" ;
TODO: should main have any arguments and should it return anything?
       main = "fn" , S , "main", S ,
              "(" , [ identifier_type , { S , "," , S , identifier_type } S , identifier_type ] ")" , S ,
              [ "->" , S , type, S ]
              "{" , S, statements, S , "}" ;

// TODO: ASSIGNMENT A DEFAULT VALUE
     struct = "struct" , S , type , S , 
              "{" , S ,
              { ( identifier_type , S , "," , S , ) |
              ( function , S , "," , S , ) }
              "}" ;

struct_expression = type , S , 
                    "{" , S ,
                    { "." , S , identifier , S , "=" , S , expression , S , "," , S } , 
                    "}" , S , ";" ;

statement = identifier , S , "=" , S , identifier , S , ";" 
;

return_statement = "return" , S [ expression , S ] ";" , S ;

statements = statement , { S , statement } ; ////TODO!

```
### Obsługa błędów, przykłady komunikatów

### Sposób uruchomienia, wej/wyj.
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

### Analiza wymagań funkcjonalnych i niefunkcjonalnych

### Zwięzły opis modułów, obiektów i interfejsów, lista tokenów, realizacja przetwarzania w poszczególnych komponentach

Lista tokenów:

 * EOF

 * LITERAL
 * IDENTIFIER

 * COMMA
 * DOT
 * LPARENT
 * RPARENT
 * LBRACE
 * RBRACE
 * SEMICOLON
 * COLON
 * ASSIGN
 * AT

 * PLUS
 * MINUS
 * STAR
 * SLASH
 * PERCENT

 * LEQ
 * LESS
 * GEQ
 * GREATER
 * EQ
 * NEQ
 * RARROW

 * PIPE

 * ONELINECOMMENT
 * MULTILINECOMMENT
 * 
 * FN
 * IF
 * ELSE
 * STRUCT
 * GLOBAL
 * MUT
 * RETURN

Lista słów kluczowych:

 * fn
 * if
 * else
 * while
 * struct
 * global
 * mut
 * return
 * as

 * u8
 * u16
 * u32
 * i8
 * i16
 * i32
 * true
 * false

#### Wykorzystane struktury danych

#### Przykład wykorzystania struktury danych

#### Formy pośrednie

#### Opis sposobu testowania
Proces testowania będzie opierał się na stopniowym kolejkowym testowaniu - zawsze wejściem testu będzie strumień znaków. W zależności który etap.

Z wykorzystaniem biblioteki Boost::unit_test_framework.
