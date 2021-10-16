#pragma once
#include <stdio.h>

#define UNUSED_ARG(x) ((void)(x))

#define ARRAY_LEN(arr)                                                         \
    ((sizeof(arr) / sizeof(0 [arr])) /                                         \
     ((size_t)(!(sizeof(arr) % sizeof(0 [arr])))))

#define _TP(t1, t2) t1##t2
#define TOKEN_PASTE(t1, t2) _TP(t1, t2)
#define TOKEN_PASTE_(t1, t2) TOKEN_PASTE(TOKEN_PASTE(t1, _), t2)
#define _STR(x) #x
#define STRINGIFY(x) _STR(x)

#define YD(y, d) TOKEN_PASTE_(TOKEN_PASTE_(year, y), TOKEN_PASTE_(day, d))

#define YDS(y, d, s) TOKEN_PASTE(TOKEN_PASTE(YD(y, d), _), s)

#define FUNCTION_NAME_FOR_YDS(y, d, s) TOKEN_PASTE(YDS(y, d, s), _solution)

#define FUNCTION_DEFN_FOR_YDS(y, d, s)                                         \
    void FUNCTION_NAME_FOR_YDS(y, d, s)(FILE * fp)

#define FUNCTION_DECL_FOR_YDS(y, d, s) extern FUNCTION_DEFN_FOR_YDS(y, d, s)

#define FUNCTION_DECL_FOR_YD(y, d)                                             \
    FUNCTION_DECL_FOR_YDS(y, d, a);                                            \
    FUNCTION_DECL_FOR_YDS(y, d, b)

#define FUNCTION_DECL_FOR_YEAR(y)                                              \
    FUNCTION_DECL_FOR_YD(y, one);                                              \
    FUNCTION_DECL_FOR_YD(y, two);                                              \
    FUNCTION_DECL_FOR_YD(y, three);                                            \
    FUNCTION_DECL_FOR_YD(y, four);                                             \
    FUNCTION_DECL_FOR_YD(y, five);                                             \
    FUNCTION_DECL_FOR_YD(y, six);                                              \
    FUNCTION_DECL_FOR_YD(y, seven);                                            \
    FUNCTION_DECL_FOR_YD(y, eight);                                            \
    FUNCTION_DECL_FOR_YD(y, nine);                                             \
    FUNCTION_DECL_FOR_YD(y, ten);                                              \
    FUNCTION_DECL_FOR_YD(y, eleven);                                           \
    FUNCTION_DECL_FOR_YD(y, twelve);                                           \
    FUNCTION_DECL_FOR_YD(y, thirteen);                                         \
    FUNCTION_DECL_FOR_YD(y, fourteen);                                         \
    FUNCTION_DECL_FOR_YD(y, fifteen);                                          \
    FUNCTION_DECL_FOR_YD(y, sixteen);                                          \
    FUNCTION_DECL_FOR_YD(y, seventeen);                                        \
    FUNCTION_DECL_FOR_YD(y, eighteen);                                         \
    FUNCTION_DECL_FOR_YD(y, nineteen);                                         \
    FUNCTION_DECL_FOR_YD(y, twenty);                                           \
    FUNCTION_DECL_FOR_YD(y, twentyone);                                        \
    FUNCTION_DECL_FOR_YD(y, twentytwo);                                        \
    FUNCTION_DECL_FOR_YD(y, twentythree);                                      \
    FUNCTION_DECL_FOR_YD(y, twentyfour);                                       \
    FUNCTION_DECL_FOR_YD(y, twentyfive)

#define SOLUTION_FOR_YDS(year_name, day_name, side)                            \
    {                                                                          \
        .puzzle_id = STRINGIFY(YDS(year_name, day_name, side)),                \
        .solution = &FUNCTION_NAME_FOR_YDS(year_name, day_name, side),         \
        "./input/" STRINGIFY(year_name) "/day_" STRINGIFY(day_name) ".txt",    \
    }

#define SOLUTIONS_FOR_YD(year_name, day_name)                                  \
    SOLUTION_FOR_YDS(year_name, day_name, a),                                  \
        SOLUTION_FOR_YDS(year_name, day_name, b)

#define SOLUTIONS_FOR_YEAR(year_name)                                          \
    SOLUTIONS_FOR_YD(year_name, one), SOLUTIONS_FOR_YD(year_name, two),        \
        SOLUTIONS_FOR_YD(year_name, three), SOLUTIONS_FOR_YD(year_name, four), \
        SOLUTIONS_FOR_YD(year_name, five), SOLUTIONS_FOR_YD(year_name, six),   \
        SOLUTIONS_FOR_YD(year_name, seven),                                    \
        SOLUTIONS_FOR_YD(year_name, eight), SOLUTIONS_FOR_YD(year_name, nine), \
        SOLUTIONS_FOR_YD(year_name, ten), SOLUTIONS_FOR_YD(year_name, eleven), \
        SOLUTIONS_FOR_YD(year_name, twelve),                                   \
        SOLUTIONS_FOR_YD(year_name, thirteen),                                 \
        SOLUTIONS_FOR_YD(year_name, fourteen),                                 \
        SOLUTIONS_FOR_YD(year_name, fifteen),                                  \
        SOLUTIONS_FOR_YD(year_name, sixteen),                                  \
        SOLUTIONS_FOR_YD(year_name, seventeen),                                \
        SOLUTIONS_FOR_YD(year_name, eighteen),                                 \
        SOLUTIONS_FOR_YD(year_name, nineteen),                                 \
        SOLUTIONS_FOR_YD(year_name, twenty),                                   \
        SOLUTIONS_FOR_YD(year_name, twentyone),                                \
        SOLUTIONS_FOR_YD(year_name, twentytwo),                                \
        SOLUTIONS_FOR_YD(year_name, twentythree),                              \
        SOLUTIONS_FOR_YD(year_name, twentyfour),                               \
        SOLUTIONS_FOR_YD(year_name, twentyfive)
