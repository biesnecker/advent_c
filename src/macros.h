#pragma once
#include <stdio.h>

#define UNUSED_ARG(x) ((void)(x))

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
