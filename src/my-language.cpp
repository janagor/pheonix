#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <cstdlib>
using namespace std;

int main(int ac, char* av[])
{
    try {
        string file_name;

        po::positional_options_description p;
        p.add("input-file", 1);

        po::options_description desc("Options");
        desc.add_options()
            ("help,h", "produce help message")
            ("lexer,l", "create a file with lexer output (with .l extention)")
            ("parser,p", "create a file with result of syntax analysis (with .p extention)")
            ("semantic,s", "create a file with result of semantic analysis (with .s extention)")
            (
                "intermediate_representation,i", 
                "create a file with result of intermediate representation(with .ir extention)"
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
            cout << "Interpreter loop is running." << endl;
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
