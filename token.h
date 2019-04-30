#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_TOKEN_SIZE      12

typedef struct sToken
{
    char token[MAX_TOKEN_SIZE];
}Token_t;


uint32_t lexer(Token_t *tokens, char *string);

#endif /* TOKEN_H_ */
