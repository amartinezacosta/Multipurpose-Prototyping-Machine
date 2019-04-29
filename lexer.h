#ifndef LEXER_H_
#define LEXER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_TOKEN_SIZE      12

typedef struct sToken
{
    char token[MAX_TOKEN_SIZE];
}Token_t;


uint32_t lexer(Token_t *tokens, char *string);

#endif /* LEXER_H_ */
