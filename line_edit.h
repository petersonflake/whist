#ifndef LINE_EDIT_H
#define LINE_EDIT_H
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "completions.h"
#define CONTROL_D_QUIT

/*
 * Returns pointer to allocated buffer with line, exluding newline.
 * Must turn off ICANON and ECHO for this to work properly.
 */
char* get_line_compl(char *prompt, char *buffer);

/*
 * Uses getline, so editing capability is determined by your shell/terminal.
 * Returns pointer to allocated buffer with typed line, excluding newline.
 */
char* get_line(char *prompt, char *buffer);

/*
 * Returns dynamically allocated buffer with however many lines the user inputs before the stop string,
 * including the newlines.  Reads input until encountering a line containing only the stop string.
 */
char* get_multiple_lines(char *prompt, char *stop);

/*
 * Turn off ICANON and ECHO, needed for get_line_compl.
 */
void icanon_off();

/*
 * Turn terminal back to normal.
 */
void reset_term();

#endif
