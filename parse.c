#include "parse.h"

int parse_move(char *string, int *rank, suits *suit)
{
    char *in = string;
    char num[24];
    memset(num, '\0', 24);
    char *out = num;
    //for(; *in && *in == ' '; ++in); //Skip any leading spaces
    while(*in) {
        switch(*in) {
            case 'A':
            case 'a':
                *rank = 12;
                break;
            case 'K':
            case 'k':
                *rank = 11;
                break;
            case 'Q':
            case 'q':
                *rank = 10;
                break;
            case 'J':
            case 'j':
                *rank = 9;
                break;
            case 'H':
            case 'h':
                *suit = HEARTS;
                break;
            case 'D':
            case 'd':
                *suit = DIAMONDS;
                break;
            case 'S':
            case 's':
                *suit = SPADES;
                break;
            case 'C':
            case 'c':
                *suit = CLUBS;
                break;
            default:
                if(isdigit(*in)) {
                    *out++ = *in;
                }
        }
        ++in;
    }
    if(num[0])
        *rank = atoi(num) - 2;
    if(*rank > 0 && *rank < 13) return 0;
    else return -1;
}
