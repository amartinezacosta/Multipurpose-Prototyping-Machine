#ifndef LEXER_H_
#define LEXER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_TOKEN_SIZE      12



struct sToken
{
    char token[MAX_TOKEN_SIZE];
};


uint32_t lexer(struct sToken *tokens, char *string);

#endif /* LEXER_H_ */
