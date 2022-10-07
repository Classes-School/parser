/* Definitions the scanner shares with the parser.
   Michael L. Scott, 2008-2022.
*/

#include <tuple>
using std::tuple;
using std::string;

enum token {t_int, t_id, t_gets, t_real, t_trunc, t_lparen, t_rparen, t_float, 
            t_read, t_write, t_if, t_then, t_end, t_while, t_do, t_i_num, t_r_num, 
            t_equal, t_not_equal, t_less, t_greater, t_less_or_equal, t_greater_or_equal,
            t_add, t_sub, t_mul, t_div, t_semicolon, t_eof
            };

const int MAX_TOKEN_LEN = 256;
extern char token_image[MAX_TOKEN_LEN];

class scanner {
    int c = ' ';
public:
    tuple<token, string> scan();
};
