#include <cstdio>
#include <cstring>
#include <iostream>

#include "exprtk.hpp"

using std::cin;
using std::string;
using std::getline;
typedef double num;
typedef exprtk::symbol_table<num>         symbol_table_t;
typedef exprtk::expression<num>           expression_t;
typedef exprtk::parser<num>               parser_t;
typedef exprtk::function_compositor<num>  compositor_t;
typedef compositor_t::function            function_t;


static const num global_lower_bound_x = -100.0;
static const num global_lower_bound_y = -100.0;
static const num global_lower_bound_z = -100.0;
static const num global_upper_bound_x = +100.0;
static const num global_upper_bound_y = +100.0;
static const num global_upper_bound_z = +100.0;
static const num global_delta         = 7.00;
static const num global_eps           = 1e-4;


void preprocess(string& str)
{
    size_t i, j = 0;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ' || str[i] == '\t') continue;
        else if (str[i] == '*' && str[i+1] == '*') str[j++] = '^', i++;
        else if (str[i] == '+' || str[i] == '-')
        {
            bool flag = str[i] == '-';
            while (++i, str[i] == '+' || str[i] == '-')
              if (str[i] == '-') flag = !flag;
            str[j++] = flag ? '-' : '+';
            i--;
        }
        else str[j++] = str[i];
    }
    str = str.substr(0, j);
}


void add_function(compositor_t& compositor)
{
    string func_str, fname, param1, param2, param3, expr;
    getline(cin, func_str);
    preprocess(func_str);
    int equal_pos = -1;
    for (size_t i = 0; i < func_str.length(); i++)
    {
        if (func_str[i] == '(' || func_str[i] == ')' || func_str[i] == ',') continue;
        if (func_str[i] == '=') { equal_pos = i; break; }
        if (fname.empty()) fname = func_str[i];
        else if (param1.empty()) param1 = func_str[i];
        else if (param2.empty()) param2 = func_str[i];
        else if (param3.empty()) param3 = func_str[i];
    }
    expr = func_str.substr(equal_pos + 1);

    if (param2.empty())
        compositor.add(function_t(
            fname,
            expr,
            param1
        ));
    else if (param3.empty())
        compositor.add(function_t(
            fname,
            expr,
            param1,
            param2
        ));
    else
        compositor.add(function_t(
            fname,
            expr,
            param1,
            param2,
            param3
        ));
}


int main()
{
    num x, y, z;
    exprtk::timer timer;

    symbol_table_t symbol_table;
    symbol_table.add_constants();
    symbol_table.add_variable("x", x);
    symbol_table.add_variable("y", y);
    symbol_table.add_variable("z", z);

    compositor_t compositor(symbol_table);

#ifdef DEBUG
    timer.start();
#endif

    int func_cnt;
    cin >> func_cnt;
    string useless;
    getline(cin, useless);
    while (func_cnt--) add_function(compositor);

#ifdef DEBUG
    timer.stop();
    fprintf(stderr, "[function] Time: %12.8f\n", timer.time());
    timer.start();
#endif

    string expression_str1, expression_str2;
    getline(cin, expression_str1);
    preprocess(expression_str1);
    getline(cin, expression_str2);
    preprocess(expression_str2);

#ifdef DEBUG
    timer.stop();
    fprintf(stderr, "[read] Time: %12.8f\n", timer.time());
    timer.start();
#endif

    expression_t  expression1, expression2;
    expression1.register_symbol_table(symbol_table);
    expression2.register_symbol_table(symbol_table);

    parser_t parser;
    parser.compile(expression_str1, expression1);
    parser.compile(expression_str2, expression2);

#ifdef DEBUG
    timer.stop();
    fprintf(stderr, "[parse] Time: %12.8f\n", timer.time());
    timer.start();
#endif


    for (x = global_lower_bound_x; x <= global_upper_bound_x; x += global_delta)
      for (y = global_lower_bound_y; y <= global_upper_bound_y; y += global_delta)
        for (z = global_lower_bound_z; z <= global_upper_bound_z; z += global_delta)
        {
            num result1 = expression1.value();
            num result2 = expression2.value();

            if (abs(result1 - result2) / std::max(result1, 1.0) < global_eps) continue;
            printf("Wrong Answer at (%.1lf, %.1lf, %.1lf) with %.8lf != %.8lf\n", x, y, z, result1, result2);
            exit(-1);
        }
    printf("Accepted!\n");

#ifdef DEBUG
    timer.stop();
    fprintf(stderr, "[judge] Time: %12.8f\n", timer.time());
#endif

    return 0;
}
