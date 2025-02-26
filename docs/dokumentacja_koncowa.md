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

W pliku `pheonix.ebnf`

# Obsługa błędów, przykłady komunikatów

Na każdym etapie przetwarzania danych znajdowała się będzie struktura, do
której będą zapisywane komunikaty dotyczące wykrytych błędów. Na sam koniec
procesu interpetacji lub gdy wykryty błąd będzie niemożliwy do pominięcia
użytkownik zostanie powiadomiony o tym przez wyjście standardowe, a proces
interpretacji zakończy się błędem i nie zostanie ukończony.


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


