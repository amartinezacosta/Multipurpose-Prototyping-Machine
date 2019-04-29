#ifndef DEFAULT_H_
#define DEFAULT_H_

#include "lexer.h"

void G00_Handler(uint32_t count, Token_t *tokens);
void G01_Handler(uint32_t count, Token_t *tokens);
void G02_Handler(uint32_t count, Token_t *tokens);
void G03_Handler(uint32_t count, Token_t *tokens);
void G04_Handler(uint32_t count, Token_t *tokens);
void G20_Handler(uint32_t count, Token_t *tokens);
void G21_Handler(uint32_t count, Token_t *tokens);
void G28_Handler(uint32_t count, Token_t *tokens);
void G92_Handler(uint32_t count, Token_t *tokens);
void M03_Handler(uint32_t count, Token_t *tokens);
void M04_Handler(uint32_t count, Token_t *tokens);
void M104_Handler(uint32_t count, Token_t *tokens);
void M105_Handler(uint32_t count, Token_t *tokens);
void M109_Handler(uint32_t count, Token_t *tokens);
void M106_Handler(uint32_t count, Token_t *tokens);
void M114_Handler(uint32_t count, Token_t *tokens);



#endif /* DEFAULT_H_ */
