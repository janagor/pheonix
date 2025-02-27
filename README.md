# Basic Information

The project involves writing an interpreter for a general-purpose programming
language.

# Requirements

* cmake
* gcc / clang
* git

# Instalation

```bash
git clone https://gitlab-stud.elka.pw.edu.pl/TKOM_24Z_WW/jgorski/pheonix.git
cd pheonix
mkdir build && cd build
cmake .. && make
```

# Running Unit Tests

```bash
ctest
```

Or individually:

``` bash 
./test_lexer
./test_parser
./test_evaluator
```

# Running Examples from Documentation
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

# Running the Main Program

```bash
> ./pheonix -h
Usage: ./pheonix [OPTIONS]

                 Read from stdin
    -h           Display this message
    -p INPUT     Generate parser output
    -l INPUT     Generate lexer output
    -i INPUT     Generate interpreter output
```


# Functionality Overview:
* The program code is evaluated line by line—there is no entry point (e.g., a `main` function).
* Values are immutable by default: variables declared without the `mut` keyword cannot be modified.
* **Dynamic typing**: Variables can change their type during runtime.
* **Strong typing**: Variables always have a defined type at any point during execution.
* Variables are passed **by reference** (constant by default).
* Control structures:
  - `while` loops.
  - `if` and `else` conditional statements.
* Functions are **first-class objects**:
  - Can be passed as parameters, assigned to variables, etc.
  - Functions cannot be overloaded.
* Data types:
  - `int` (integer),
  - `flt` (floating-point),
  - `bol` (boolean),
  - `str` (string),
  - Functions.
* Function-related operators:
  - `|` – Function composition operator.
  - `#(){}` – Lambda expression syntax (e.g., `#(x){ x + 1 }`).
  - `[]()` – Debug operator (e.g., `[myFunc]()` prints debug info for `myFunc`).
* **REPL** (Read-Eval-Print Loop) support.


# Formal Specification (EBNF)
Defined in `pheonix.ebnf`.

# Error Handling
* Errors are logged in a dedicated structure during processing.
* Unrecoverable errors trigger:
  - A message printed to standard output.
  - Immediate termination of interpretation.

# Project Structure
* **Lexer**:
  - *Input*: Raw code stream.
  - *Output*: Token stream.
  - Key method: `Lexer::nextToken()` fetches the next token.
* **Parser**:
  - *Input*: Token stream (aggregates a `Lexer`).
  - *Output*: Abstract Syntax Tree (AST).
  - Key method: `Parser::generateParsingTree()` constructs the AST.
* **Evaluator**:
  - *Input*: AST.
  - *Output*: Program results (stdout).
  - Uses the **Visitor pattern** to traverse/execute the AST.
  - State managed via a `Context` structure (variables, scopes).

# Testing
* **Lexer Tests**:
  - Compare generated tokens/errors against expected outputs.
* **Parser Tests**:
  - Validate the AST’s string representation matches expectations.
* **Evaluator Tests**:
  - Verify final program outputs match expected results.
