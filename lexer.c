#include "lexer.h"

bool is_special(const char c)
{
    return (c == '*' || c == '-' || c == '+' || c == '/' ||
            c == '#' || c == '[' || c == ']' || c == '.' ||
            c == '=') ? true : false;
}

bool is_digit(const char c)
{
    return ((c > '/') && (c < ':')) ? true : false;
}

bool is_alpha(const char c)
{
    return (((c > '@') && (c < '[')) || ((c > '`') && (c < '{'))) ? true : false;
}


uint32_t getToken(Token_t *token, char *string)
{
    char *s = string;
    uint32_t i = 0;

    //skip any white space
    while(*string == ' ')
    {
        string++;
    }

    //letter + letter + real number
    if(is_alpha(*string))
    {
        token->token[i++] = *string++;
        if((*string == '+') || (*string == '-') || (*string == '.') || (is_digit(*string)))
        {
            token->token[i++] = *string++;
            while(is_digit(*string) || *string == '.')
            {
                token->token[i++] = *string++;
            }
        }
        else if(!is_alpha(*string))
        {
            return 0;
        }
        else
        {
            return 0;
        }
    }

    //Comment
    else if(*string == '(')
    {
        token->token[i++] = *string;
        while(*string != ')')
        {
            if(*string == '\0')
            {
                return 0;
            }
            string++;
        }
        string++;
    }

    //Comment
    else if(*string == ';')
    {
        token->token[i++] = *string++;
        while(*string != '\n')
        {
            if(*string == '\0')
            {
                return 0;
            }
            string++;
        }
    }

    else if(*string == '*')
    {
        //Pronterface checksum
        token->token[i++] = *string++;
        while(is_digit(*string))
        {
            token->token[i++] = *string++;
        }
    }

    else if(*string == '\r' || *string == '\n')
    {
        token->token[i++] = *string++;
    }

    token->token[i] = 0;

    return string - s;
}

//The lexer identifies the individual allowed tokens in a sentence
uint32_t lexer(Token_t *tokens, char *string)
{
    uint32_t size;
    uint32_t i = 0;


    while(*string)
    {
        size = getToken(&tokens[i++], string);
        if(size == 0)
        {
            return 0;
        }

        string += size;
    }

    return i;
}

