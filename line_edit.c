#include "line_edit.h"

void kill_char(char *string, int *offset)
{
    memmove(string + *offset, string + *offset + 1, 4096 - strlen(string) - 1);
    string[--(*offset)] = '\0';
    printf("\b \b");
}

void kill_word(char *string, int *offset)
{
    char *ch = &string[*offset];
    while(ch > string && *(ch-1) != ' ') {
        *ch-- = '\0';
        printf("\b \b");
        --(*offset);
    }
    *ch-- = '\0';
}

void kill_line(char *string, int *offset)
{
    while(&string[*offset] > string) {
        string[--(*offset)] = '\0';
        printf("\b \b");
    }
}

void insert_char(char *string, char ch, int *curs)
{
    memmove(string + *curs + 1, string + *curs + 0, 4096 - strlen(string) - 1);
    string[*curs] = ch;
    ++(*curs);
    putc(ch, stdout);
    printf("\033[s%s\033[u", &string[*curs]);
}

char* get_word_under_cursor(char *string, int *offset)
{
    //if(!string[*offset]) return NULL;
    char *tmp = malloc(4096);
    memset(tmp, '\0', 4096);
    char *out = tmp;
    char *in = &string[*offset];
    while(in > (string-1) && *in != ' ') {
        --in;
    };
    ++in;
    while(*in && *in != ' ') {
        *out++ = *in++;
    }
    return tmp;
}

void insert_str(char *string, char *ins, int *curs)
{
    for(char *in = ins; *in; ++in) {
        insert_char(string, *in, curs);
    }
}

/*
 * Read one line from the user, with tab completion.
 */
char* get_line_compl(char *prompt, char *buffer)
{
    fflush(stdin);
    fflush(NULL);
    printf("%s", prompt);
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    struct termios newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    memset(buffer, '\0', 4096);


    int ch;
    int cursor = 0;
    char *word;
    char *comp;
    char prev;
    static char *prev_command = NULL;
    static char *tmp_buf = NULL;
    int line = 0;
    if(!prev_command) prev_command = malloc(4096);
    if(!tmp_buf) tmp_buf = malloc(4096);
    do {
        ch = fgetc(stdin);
        switch(ch) {
            case 4: /* ^D EOF */
#ifndef CONTROL_D_QUIT
                ch = '\n';                                  /* Control-D behaves the same as enter here. */
#else
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);    /* Reset the terminal to normal */
                printf("\n");                               /* Add newline to not mess up bash prompt */
                strcpy(buffer, "exit");                     /* Return command "exit", which the user should */
                return buffer;                              /* have implemented. */
#endif
                break;
            case 127: /* DEL */
            case '\b': /* backspace */
                if(cursor > 0) {
                    kill_char(buffer, &cursor);
                }
                break;

            case 23: /* ^W kill word */
                if(cursor > 0) {
                    kill_word(buffer, &cursor);
                }
                break;

            case 21: /* ^U */
            case 24: /* ^X Kill line */
                if(cursor > 0) {
                    kill_line(buffer, &cursor);
                }
                break;


            case 12: /* ^L, clear screen */
                printf("\033[2J\033[0;0H");         /* erase display, move cursor to 0,0 */
                printf("%s%s", prompt, buffer);
                break;

            case '\033':                            /* ESC, first part of escape sequence. */
                fgetc(stdin);
                ch = fgetc(stdin);                  /* Read character "[" from standard input */
                switch(ch) {
                    case 'C':                       /* Right arrow pressed */
                        if(cursor < 4096) {
                            printf("\033[C");
                            ++cursor;
                        }
                        break;

                    case 'D':                       /* Left arrow pressed */
                        if(cursor > 0) {
                            printf("\033[D");
                            --cursor;
                        }
                        break;

                    case 'A':                       /* Up arrow */
                        if(!line) {                                         /* Only do this if we are not in default buffer */
                            memcpy(tmp_buf, buffer, 4096);                  /* Copy current buffer to holding buffer */
                            memcpy(buffer, prev_command, 4096);             /* Copy previous buffer to current buffer */
                            cursor = strlen(buffer);                        /* Set cursor to end of current line */
                            printf("\033[200D\033[K%s%s", prompt, buffer);  /* Move terminal cursor to start, kill line, display */
                            line = 1;
                        }
                        break;

                    case 'B':                       /* Down arrow */
                        if(line) {                                          /* Only do this if we are in the default buffer */
                            memcpy(prev_command, buffer, 4096);             /* Copy current buffer to temporary buffer */
                            memcpy(buffer, tmp_buf, 4096);                  /* Copy original main buffer back to current buffer */
                            cursor = strlen(buffer);                        /* Set cursor to end of current line */
                            printf("\033[200D\033[K%s%s", prompt, buffer);  /* Move cursor left, kill line, print prompt and buffer */
                            line = 0;
                        }
                        break;

                    default:
                        break;
                }
                break;

            case '\t':                                                      /* Tab completion */
                if(global_comps) {
                    word = get_word_under_cursor(buffer, &cursor);          /* Get *copy* of word behind cursor */
                    if(word) {
                        comp = complete_text(word, global_comps);           /* Get characters to complete the word */
                        if(comp) {
                            insert_str(buffer, comp, &cursor);              /* Insert completion into buffer */
                        } else if(prev == '\t') {                           /* If tab pressed twice in a row */
                            putc('\n', stdout);                             /* Print all commands */
                            print_completions(global_comps);
                            printf("%s%s", prompt, buffer);
                        }
                    }
                    free(word);                                             /* free word allocated with strdup */
                }
                break;

            default:
                if(ch > 31 && ch < 127) {                                   /* Is printable character, so add to buffer */
                    insert_char(buffer, ch, &cursor);
                }
                break;
        }
        prev = ch;
    } while(ch != '\n');

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);                                /* Reset the terminal before returning */
    putc('\n', stdout);
    memcpy(prev_command, buffer, 4096);                                     /* Copy current command into previous buffer */
    return buffer;                                                          /* So it is available for the next run */
}

/*
 * Read a line from the user with getline, then delete the newline.
 * No tab completion.
 */
char* get_line(char *prompt, char *buffer)
{
    printf("%s", prompt);
    size_t len;
    memset(buffer, '\0', 4096);
    getline(&buffer, &len, stdin);
    for(char *in = buffer; *in; ++in) { /* Delete newline */
        if(*in == '\n') {
            *in = '\0';
            break;
        }
    }
    return buffer;
}

char* get_multiple_lines(char *prompt, char *stop);
