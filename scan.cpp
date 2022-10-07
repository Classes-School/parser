/* Simple ad-hoc scanner for the calculator language.
   Dies on lexical errors.
   Michael L. Scott, 2008-2022.
*/

#include <iostream>
#include <cctype>   // isalpha, isspace, isdigit
#include <tuple>
using std::cerr;
using std::cin;
using std::hex;
using std::endl;
using std::string;
using std::make_tuple;

#include "scan.hpp"

tuple<token, string> scanner::scan() {
    string token_image;
    //// keywords and variable (id)
    // { "id", 
    // "real", "trunc", "int", "float", "read", "write", "if", "then", "end", "while", "do",

    // "i_num", "r_num", 
    // "lparen", "rparen", 
    // "gets",
    // "equal", "noequal", "less", "greater", "less_or_equal", "greater_or_equal",
    // "add", "sub", "mul", "div", "comma", "eof"
    // };

    // for each bad character, 
    // skip white space
    while (isspace(c)) {
        c = cin.get();
    }
    if (c == EOF)
        return make_tuple(t_eof, "");
    if (isalpha(c)) {
        do {
            token_image += c;
            c = cin.get();
        } while (isalpha(c) || isdigit(c) || c == '_');
        if (token_image == "real") return make_tuple(t_real, "");
        else if (token_image == "trunc") return make_tuple(t_trunc, "");
        else if (token_image == "int") return make_tuple(t_int, "");
        else if (token_image == "float") return make_tuple(t_float, "");
        else if (token_image == "read") return make_tuple(t_read, "");
        else if (token_image == "write") return make_tuple(t_write, "");
        else if (token_image == "if") return make_tuple(t_if, "");
        else if (token_image == "then") return make_tuple(t_then, "");
        else if (token_image == "end") return make_tuple(t_end, "");
        else if (token_image == "while") return make_tuple(t_while, "");
        else if (token_image == "do") return make_tuple(t_do, "");
        else return make_tuple(t_id, token_image);
    }
    else if (isdigit(c)) {
        // [d+ . d* | d* . d+ ] ( e [ + | - | ε ] d+ | ε )
        // [0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
        do {
            token_image += c;
            c = cin.get();
        } while (isdigit(c));
        if (c == '.'){
            token_image += c;
            c = cin.get();
            if (isdigit(c)){
                do {
                    token_image += c;
                    c = cin.get();
                } while (isdigit(c));
                if (c == 'e'){
                    token_image += c;
                    c = cin.get();
                    if (c == '+' || c == '-'){
                        token_image += c;
                        c = cin.get();
                        if(!isdigit(c)){
                            // case C:
                            cerr << "lexical error: invalid real number. got '"
                            <<  std::string(1, c) << "' (0x" << hex << c << ") after " << token_image << "\n";
                            return scan();
                        } else{
                            do{
                                token_image += c;
                                c = cin.get();
                            }while (isdigit(c));
                            return make_tuple(t_r_num, token_image);
                        }
                    } else {
                        if (isdigit(c)){
                            do{
                                token_image += c;
                                c = cin.get();
                            }while (isdigit(c));
                            return make_tuple(t_r_num, token_image);
                        } else{
                            // case C:
                            cerr << "lexical error: invalid real number. got '"
                            <<std::string(1, c) << "' (0x" << hex << c << ") after " << token_image << "\n";
                            return scan();
                        }
                    }
                } else{
                    return make_tuple(t_r_num, token_image);
                }
            } else {
                // case C:
                cerr << "lexical error: invalid real number. got '"
                     << std::string(1, c) << "' (0x" << hex << c << ") after " << token_image << "\n";
                return scan();
            }
        }else{
            return make_tuple(t_i_num, token_image);
        }
    } else switch (c) {
        case ':':
            c = cin.get();
            if (c != '=') {
                // case C:
                cerr << "lexical error: expected '=' after ':', got '"
                     <<  std::string(1, c) << "' (0x" << hex << c << ")\n";
                return scan();
            } else {
                c = cin.get();
                return make_tuple(t_gets, "");
            }
            break;
        case '=':
            c = cin.get();
            if (c != '=') {
                // case C:
                cerr << "lexical error: expected '=' after ':', got '"
                     << std::string(1, c) << "' (0x" << hex << c << ")\n";
                return scan();
            } else {
                c = cin.get();
                return make_tuple(t_equal, "");
            }
            break;
        case '<':
            c = cin.get();
            if(c == '>'){
                c = cin.get();
                return make_tuple(t_not_equal, "");
            } 
            else if(c == '='){
                c = cin.get();
                return make_tuple(t_less_or_equal, "");
            }
            else {return make_tuple(t_less, "");}
            break;
        case '>':
            c = cin.get();
            if (c == '=') {
                c = cin.get();
                return make_tuple(t_greater_or_equal, "");
            } else {
                return make_tuple(t_greater, "");
            }
            break;
        case '+': c = cin.get(); return make_tuple(t_add, "");
        case '-': c = cin.get(); return make_tuple(t_sub, "");
        case '*': c = cin.get(); return make_tuple(t_mul, "");
        case '/': c = cin.get(); return make_tuple(t_div, "");
        case '(': c = cin.get(); return make_tuple(t_lparen, "");
        case ')': c = cin.get(); return make_tuple(t_rparen, "");
        case ';': c = cin.get(); return make_tuple(t_semicolon, "");
        default:   
            // case A:
            cerr << "lexical error: began with unexpected character '"
                 << std::string(1, c) << "' (0x" << hex << c << ")\n";
            c = cin.get();
            return scan();
    }
}

class tree{

};