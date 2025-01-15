# Techniki kompilacji 2024Z
## Dokumentacja końcowa
## Jan Górski

# Informacje podstawowe

Tematem projektu jest napisanie interpretera języka programowania ogólnego
przeznaczenia.

# Wymagania

* cmake
* gcc / clang
* git

# Instalacja

```bash
git clone https://gitlab-stud.elka.pw.edu.pl/TKOM_24Z_WW/jgorski/pheonix.git
cd pheonix
mkdir build && cd build
cmake .. && make
```

# Uruchomienie testów jednostkowych:

```bash
ctest
```

Lub każdy osobno:

``` bash 
./test_lexer
./test_parser
./test_evaluator
```

# Uruchomienie przykładów z dokumentacji:

```bash
> ./example
Usage: ./example [OPTIONS]

    -h            Display this message
    -p            Generate parser output for all examples
    -l            Generate lexer output for all examples
    -i            Generate interpreter output for all examples
    -p NUMBER     Generate parser output for example nr NUMBER
    -l NUMBER     Generate lexer output for example nr NUMBER
    -i NUMBER     Generate interpreter output for example nr NUMBER
```

# Uruchomienie właściwego programu:

```bash
> ./pheonix -h
Usage: ./pheonix [OPTIONS]

                 Read from stdin
    -h           Display this message
    -p INPUT     Generate parser output
    -l INPUT     Generate lexer output
    -i INPUT     Generate interpreter output
```


# Opis funkcjonalności:
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
   * `[]()` - operator `debug` - wyświetla informacje dotyczące wywołania funkcji.
* REPL

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

   assignement_expression = identifier "=" or_expression ;

            or_expression = and_expression
                            { "||" and_expression } ;

           and_expression = comparison_expression
                            { "&&" comparison_expression } ;

    comparison_expression = relational_expression
                            [ ( "==" | "!=" )
                            relational_expression ] ;

    relational_expression = additive_expression
                            [ ( "<" | ">" | "<=" | ">=" )
                            additive_expression ] ;

      additive_expression = multiplicative_expression
                            { ( "+" | "-" )
                            multiplicative_expression } ;

multiplicative_expression = compositive_expression
                            { ( "*" | "/" | "%" )
                            compositive_expression } ;

   compositive_expression = cast_expression
                            { "|" cast_expression } ;

          cast_expression = prefix_expression { "<-" type_name } ;

        prefix_expression = "!" other_expression
                          | "-" other_expression
                          | other_expression ;

         other_expression = maybe_call_expression
                          | literal ;

    maybe_call_expression = maybe_identifier_call
                          | maybe_lambda_call
                          | maybe_parent_call
                          | maybe_debug_call ;

    maybe_identifier_call = identifier { "(" expression_list ")" } ;

        maybe_lambda_call = lambda_expression { "(" expression_list ")" } ;

        maybe_parent_call = parent_expression { "(" expression_list ")" } ;

         maybe_debug_call = debug_expression { "(" expression_list ")" } ;

        lambda_expression = "#" enclosed_parameter_list function_body ;

        parent_expression = "(" expression ")" ;

         debug_expression = "[" expression "]" "(" expression_list ")" ;

                  literal = bool_literal
                          | float_literal
                          | integer_literal
                          | string_literal ;

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

# Lista tokenów:

* END_OF_FILE = 0,
* ONE_LINE_COMMENT,
* MULTILINE_COMMENT,
* ASSIGN,
* BANG,
* PLUS,
* MINUS,
* STAR,
* SLASH,
* PERCENT,
* HASH,
* PIPE,
* DOLAR,
* SEMICOLON,
* COMMA,
* LPARENT,
* RPARENT,
* LBRACE,
* RBRACE,
* LBRACKET,
* RBRACKET,
* AND,
* OR,
* EQUALS,
* NEQ,
* LEQ,
* GEQ,
* GREATER,
  LESS,
* LARROW,
* IDENTIFIER,
* STRING,
* INTEGER,
* FLOAT,
* FN,
* LET,
* MUT,
* RETURN,
* IF,
* ELSE,
* WHILE,
* INT,
* STR,
* FLT,
* BOL,
* TRUE,
* FALSE,
* NOT_A_TOKEN,

Lista słów kluczowych:

* fn
* let
* return
* mut
* if
* else
* while
* int
* str
* bol
* flt
* true
* false

# Struktura projektu

Projekt składa się z nastepujących modułów:

* Lexer
    * wejście: strumień kodu programu
    * wyjście: strumień tokenów
    * metod **Lexer::nextToken()** zwraca następny token
* Parser
    * wejście: strumień kodu programu
    * wyjście: drzewo AST
    * parser agreguje obiekt **Lexer**. Wczytuje strumień wejściowy. Metoda
    **Lexer::p.generateParsingTree()** generuje drzewo składniowe
    wywołując w pętli **Lexer::nextToken()** i dodaje w ten sposob kolejne
    węzły drzewa.
* Evaluator
    * wejście: drzewo AST
    * wyjściu: standardowe wyjście
    * Evaluator implementuje wzorzec wizytatora. Evaluator _wizytuje_ drzewo
    AST wykonując poszczególne instrukcje. Pamięć ewaluatora jest
    reprezentowana za pomocą struktury **Context**: przechowuje ona informacje
    o aktualnym stanie programu, tj. nazw, wartości i zakresów zmiennych.
    Struktura ta jest 

# Opis sposobu testowania

Proces testowania polega na wczytywaniu strumienia wejściowego i sprawdzania,
jak poszczególne moduły reagują na zadane wejście i co generują.

* Analizator leksykalny:
   *  porównywanie poszczególnych wartości leksemów i błędów wynikowych
   z oczekiwanymi,
* Analizator składniowy:
   * porównanie reprezentacji napisowej drzewa AST z oczekiwaną wartością
* Ewaluacja:
   * porównywane są ostatnie wartości generowane przez ewaluator dla danego
   programu.


