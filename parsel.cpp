/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2022.
*/

#include <iostream>
#include <tuple>
#include <vector>
#include <bits/stdc++.h>

using std::cerr;
using std::count;
using std::cout;
using std::endl;
using std::get;
using std::hex;
using std::string;
using std::tie;

#include "scan.hpp"

// const char* names[] = {"read", "write", "id", "literal", "gets", "add",
//                        "sub", "mul", "div", "lparen", "rparen", "eof"};

const char *names[] = {"int", "id", "gets", "real", "trunc", "lparen", "rparen", "float",
                       "read", "write", "if", "then", "end", "while", "do", "i_num", "r_num",
                       "equal", "noequal", "less", "greater", "less_or_equal", "greater_or_equal",
                       "add", "sub", "mul", "div", "semi_colon", "eof"};

const std::vector<token> FIRST_P = {t_int, t_real, t_id, t_read, t_write, t_if, t_while, t_trunc, t_float};
const std::vector<token> FIRST_S = {t_int, t_real, t_id, t_read, t_write, t_if, t_while, t_trunc, t_float};
const std::vector<token> FIRST_SL = {t_int, t_real, t_id, t_read, t_write, t_if, t_while, t_trunc, t_float};
const std::vector<token> FIRST_TP = {t_int, t_real};
const std::vector<token> FIRST_F = {t_lparen, t_id, t_i_num, t_r_num};
const std::vector<token> FIRST_T = {t_lparen, t_id, t_i_num, t_r_num};
const std::vector<token> FIRST_E = {t_lparen, t_id, t_i_num, t_r_num};
const std::vector<token> FIRST_C = {t_lparen, t_id, t_i_num, t_r_num};
const std::vector<token> FIRST_AO = {t_add, t_sub};
const std::vector<token> FIRST_MO = {t_mul, t_div};
const std::vector<token> FIRST_TT = {t_add, t_sub};
const std::vector<token> FIRST_FT = {t_mul, t_div};
const std::vector<token> FIRST_RO = {t_equal, t_not_equal, t_less, t_greater, t_less_or_equal, t_greater_or_equal, t_less_or_equal, t_less_or_equal};


const std::vector<token> FOLLOW_P = {t_eof};
const std::vector<token> FOLLOW_SL = {t_eof, t_end};
const std::vector<token> FOLLOW_S = {t_semicolon};
const std::vector<token> FOLLOW_TP = {t_id};
const std::vector<token> FOLLOW_C = {t_then, t_do};
const std::vector<token> FOLLOW_E = {t_rparen, t_equal, t_not_equal, t_less, t_greater, t_less_or_equal, t_greater_or_equal, t_semicolon, t_then, t_do};
const std::vector<token> FOLLOW_TT = {t_rparen, t_equal, t_not_equal, t_less, t_greater, t_less_or_equal, t_greater_or_equal, t_semicolon, t_then, t_do};
const std::vector<token> FOLLOW_T = {t_add, t_sub, t_rparen, t_equal, t_not_equal, t_less, t_greater, t_less_or_equal,  t_then, t_do, t_greater_or_equal, t_semicolon};
const std::vector<token> FOLLOW_FT = {t_add, t_sub, t_mul, t_div, t_rparen, t_not_equal, t_less, t_greater, t_equal, t_less_or_equal, t_then, t_do, t_greater_or_equal, t_semicolon, };
const std::vector<token> FOLLOW_F = {t_mul, t_div, t_add, t_sub, t_rparen,t_less, t_greater, t_less_or_equal, t_greater_or_equal, t_semicolon, t_equal, t_not_equal, t_then, t_do};
const std::vector<token> FOLLOW_RO = {t_lparen, t_id, t_i_num, t_r_num, t_trunc, t_float };
const std::vector<token> FOLLOW_AO = {t_lparen, t_id, t_i_num, t_r_num, t_trunc, t_float};
const std::vector<token> FOLLOW_MO = {t_lparen, t_id, t_i_num, t_r_num, t_trunc, t_float};

bool contains(std::vector<token> tokens, token k)
{
    return std::count(tokens.begin(), tokens.end(), k);
}

class parser
{
    token next_token;
    string token_image;
    scanner s;

    void errors()
    {
        cerr << "syntax error ffffff" << endl;
        exit(1);
    }

    void error()
    {
        cerr << "syntax error" << endl;
    }

    void match(token expected)
    {
        if (next_token == expected)
        {
            cout << "matched " << names[next_token] << endl;
            tie(next_token, token_image) = s.scan();
        }
        else
        {
            cerr << "syntax error: got " << names[next_token] << " expected " << names[expected] << endl;
        }
    }

public:
    parser()
    {
        tie(next_token, token_image) = s.scan();
    }

    void program()
    {
        switch (next_token)
        {
        case t_int:
        case t_real:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof:
            cout << "predict program --> stmt_list eof" << endl;
            stmt_list();
            match(t_eof);
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_P, next_token))
                {
                    program();
                    break;
                }
                else if (contains(FOLLOW_P, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

private:
    void stmt_list()
    {
        switch (next_token)
        {
        case t_int:
        case t_real:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
            cout << "predict stmt_list --> stmt ; stmt_list" << endl;
            stmt();
            match(t_semicolon);
            stmt_list();
            break;
        case t_end:
        case t_eof:
            cout << "predict stmt_list --> epsilon" << endl;
            break; // epsilon production
        default:
            error();
            while (true)
            {
                if (contains(FIRST_SL, next_token))
                {
                    stmt_list();
                    break;
                }
                else if (contains(FOLLOW_SL, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void stmt()
    {
        switch (next_token)
        {
        case t_int:
            cout << "predict stmt --> int id gets expr" << endl;
            match(t_int);
            match(t_id);
            match(t_gets);
            expr();
            break;
        case t_real:
            cout << "predict stmt --> real id gets expr " << endl;
            match(t_real);
            match(t_id);
            match(t_gets);
            expr();
            break;
        case t_id:
            cout << "predict stmt --> id gets expr" << endl;
            match(t_id);
            match(t_gets);
            expr();
            break;
        case t_read:
            cout << "predict stmt --> read type id" << endl;
            match(t_read);
            type();
            match(t_id);
            break;
        case t_write:
            cout << "predict stmt --> write expr" << endl;
            match(t_write);
            expr();
            break;
        case t_if:
            cout << "predict stmt --> if condition then stmt_list end " << endl;
            match(t_if);
            condition();
            match(t_then);
            stmt_list();
            match(t_end);
            break;
        case t_while:
            cout << "predict stmt --> while condition do stmt_list end" << endl;
            match(t_while);
            condition();
            match(t_do);
            stmt_list();
            match(t_end);
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_S, next_token))
                {
                    stmt();
                    break;
                }
                else if (contains(FOLLOW_S, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void type()
    {
        switch (next_token)
        {
        case t_int:
            cout << "predict type --> int" << endl;
            match(t_int);
            break;
        case t_real:
            cout << "predict type --> real" << endl;
            match(t_real);
            break;
        case t_id:
            cout << "predict type --> epsilon" << endl;
            break; // epsilon production
        default:
            error();
            while (true)
            {
                if (contains(FIRST_TP, next_token))
                {
                    type();
                    break;
                }
                else if (contains(FOLLOW_TP, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
            break;
        }
    }

    void condition()
    {
        switch (next_token)
        {
        case t_lparen:
        case t_id:
        case t_i_num:
        case t_r_num:
        case t_float:
        case t_trunc:
            cout << "predict condition --> expr ro expr" << endl;
            expr();
            ro();
            expr();
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_C, next_token))
                {
                    condition();
                    break;
                }
                else if (contains(FOLLOW_C, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void expr()
    {
        switch (next_token)
        {
        case t_lparen:
        case t_id:
        case t_i_num:
        case t_r_num:
        case t_float:
        case t_trunc:
            cout << "predict expr --> term term_tail" << endl;
            term();
            term_tail();
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_E, next_token))
                {
                    expr();
                    break;
                }
                else if (contains(FOLLOW_E, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void term_tail()
    {
        switch (next_token)
        {
        case t_add:
        case t_sub:
            cout << "predict term_tail --> add_op term term_tail" << endl;
            add_op();
            term();
            term_tail();
            break;
        case t_rparen:
        case t_equal:
        case t_not_equal:
        case t_greater:
        case t_less:
        case t_greater_or_equal:
        case t_less_or_equal:
        case t_do:
        case t_then:
        case t_semicolon:
            cout << "predict term_tail --> epsilon" << endl;
            break; // epsilon production
        default:
            error();
            while (true)
            {
                if (contains(FIRST_TT, next_token))
                {
                    term_tail();
                    break;
                }
                else if (contains(FOLLOW_TT, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void term()
    {
        switch (next_token)
        {
        case t_lparen:
        case t_id:
        case t_i_num:
        case t_r_num:
        case t_float:
        case t_trunc:
            cout << "predict term --> factor factor_tail" << endl;
            factor();
            factor_tail();
            break;
        default:
            while (true)
            {
                if (contains(FIRST_T, next_token))
                {
                    term();
                    break;
                }
                else if (contains(FOLLOW_SL, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void factor_tail()
    {
        switch (next_token)
        {
        case t_mul:
        case t_div:
            cout << "predict factor_tail --> mul_op factor factor_tail"
                 << endl;
            mul_op();
            factor();
            factor_tail();
            break;
        case t_add:
        case t_sub:
        case t_semicolon:
        case t_rparen:
        case t_equal:
        case t_not_equal:
        case t_greater:
        case t_less:
        case t_greater_or_equal:
        case t_less_or_equal:
        case t_do:
        case t_then:
            cout << "predict factor_tail --> epsilon" << endl;
            break; // epsilon production
        default:
            error();
            while (true)
            {
                if (contains(FIRST_FT, next_token))
                {
                    factor_tail();
                    break;
                }
                else if (contains(FOLLOW_FT, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void factor()
    {
        switch (next_token)
        {
        case t_i_num:
            cout << "predict factor --> t_i_num" << endl;
            match(t_i_num);
            break;
        case t_r_num:
            cout << "predict factor --> t_r_num" << endl;
            match(t_r_num);
            break;
        case t_id:
            cout << "predict factor --> id" << endl;
            match(t_id);
            break;
        case t_lparen:
            cout << "predict factor --> lparen expr rparen" << endl;
            match(t_lparen);
            expr();
            match(t_rparen);
            break;
        case t_trunc:
            cout << "predict factor --> t_trunc lparen expr rparen" << endl;
            match(t_trunc);
            match(t_lparen);
            expr();
            match(t_rparen);
            break;
        case t_float:
            cout << "predict factor --> t_float lparen expr rparen" << endl;
            match(t_float);
            match(t_lparen);
            expr();
            match(t_rparen);
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_F, next_token))
                {
                    factor();
                    break;
                }
                else if (contains(FOLLOW_F, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void ro()
    {
        switch (next_token)
        {
        case t_equal:
            cout << "predict ro --> equal" << endl;
            match(t_equal);
            break;
        case t_not_equal:
            cout << "predict ro --> not_equal" << endl;
            match(t_not_equal);
            break;
        case t_less:
            cout << "predict ro --> less" << endl;
            match(t_less);
            break;
        case t_greater:
            cout << "predict ro --> greater" << endl;
            match(t_greater);
            break;
        case t_less_or_equal:
            cout << "predict ro --> less_or_equal" << endl;
            match(t_less_or_equal);
            break;
        case t_greater_or_equal:
            cout << "predict ro --> greater_or_equal" << endl;
            match(t_greater_or_equal);
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_RO, next_token))
                {
                    ro();
                    break;
                }
                else if (contains(FOLLOW_RO, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void add_op()
    {
        switch (next_token)
        {
        case t_add:
            cout << "predict add_op --> add" << endl;
            match(t_add);
            break;
        case t_sub:
            cout << "predict add_op --> sub" << endl;
            match(t_sub);
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_AO, next_token))
                {
                    add_op();
                    break;
                }
                else if (contains(FOLLOW_AO, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }

    void mul_op()
    {
        switch (next_token)
        {
        case t_mul:
            cout << "predict mul_op --> mul" << endl;
            match(t_mul);
            break;
        case t_div:
            cout << "predict mul_op --> div" << endl;
            match(t_div);
            break;
        default:
            error();
            while (true)
            {
                if (contains(FIRST_MO, next_token))
                {
                    mul_op();
                    break;
                }
                else if (contains(FOLLOW_MO, next_token) || next_token == t_eof)
                {
                    return;
                }
                else
                    tie(next_token, token_image) = s.scan();
            }
        }
    }
};

int main()
{
    parser p;
    p.program();
    return 0;
}
