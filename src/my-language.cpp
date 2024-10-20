#include <boost/program_options.hpp>
#include "../inc/repl.hpp"
#include <iostream>
#include <iterator>
#include <cstdlib>
namespace po = boost::program_options;
using namespace std;

constexpr const char* PHEONIX =
R"(                          j    |
  .                         u _ (
' c                        .   t t
f Y            u  v            t r
 n  u           ccczc      t  t:.  t
  uu uv        vzz zzc    tr jj  ft
 v  vvv`cxcc   zzn   X  `nnnx xrr'
X  vccczczzzzzXXXj    vcvvvuuuunnn. X
 `YX    XzzXXXYYYYzU  Xzzzzzc-  'U
         XfYYYYUUUUUU.UUYYYYX  {
          Y    UUJJJJCCC'  I `
               JJCCCCC0Q
              J'ClLCLL  x0
                 LLLQ
            . LL QQQ
          x L^ rQQ1.
         u    Q0.0
        cc   0  O
       `X   Ov  ff c
           . zz  Z   zz.
            'UU Y mmm.YXX
             CC C'mmm   U
             0.0Q.  tm
               OO
______ _        }             _
| ___ \ |                    (_)
| |_/ / |__   ___  ___  _ __  ___  __
|  __/| '_ \ / _ \/ _ \| '_ \| \ \/ /
| |   | | | |  __/ (_) | | | | |>  < 
\_|   |_| |_|\___|\___/|_| |_|_/_/\_\
)";



int main(int ac, char* av[])
{
    try {
        string file_name;

        po::positional_options_description p;
        p.add("input-file", 1);

        po::options_description desc("Options");
        desc.add_options()
            ("help,h", "produce help message")
            ("lexer,l", "create a file with lexer output \
(with .l extention)")
            ("parser,p", "create a file with result of syntax analysis \
(with .p extention)")
            ("semantic,s", "create a file with result of semantic analysis \
(with .s extention)")
            (
                "intermediate_representation,i", 
                "create a file with result of intermediate representation \
(with .ir extention)"
            )
        ;

        po::options_description hidden("Hidden");
        hidden.add_options()
            (
                "input-file",
                po::value<string>(&file_name)
            )
        ;

        po::options_description all_options;
        all_options.add(desc).add(hidden);

        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).
                  options(all_options).positional(p).run(), vm);
        po::notify(vm);

        if (vm.empty()) {
            cout << PHEONIX;
            cout << "Interpreter loop is running." << endl;
            repl::start(cin, cout);
            return EXIT_SUCCESS;
        } 
        if (vm.count("help")) {
            cout << "Usage: ./my-language [OPTIONS] INPUT" << endl;
            cout << desc << "\n";
            return EXIT_SUCCESS;
        }

        if (vm.count("input-file")) {
        cout << "working on a file: " << file_name << endl;

        }
        if (vm.count("lexer")) {
        cout << "lexer output created" << endl;
        }
        if (vm.count("parser")) {
        cout << "parser output created" << endl;
        }
        if (vm.count("semantic")) {
        cout << "semantic output created" << endl;
        }
        if (vm.count("intermediate_representation")) {
        cout << "intermediate_representation output created" << endl;
        }

        cout << "Interpreter loop with data from a given file." << endl;
        return EXIT_SUCCESS;

    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return EXIT_SUCCESS;
}
