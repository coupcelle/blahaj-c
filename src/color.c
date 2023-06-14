#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"

#define ANSI_COLOR_RESET   "\x1b[0m"

// Functions to make or print colored strings go here
// File handling needs to go in a different file.

// Double percentages to get the correct output
void replacePercentage(char* str) {
    char* found = NULL;
    const char* percentage = "%";
    const char* doublePercentage = "%%";
    int percentageLen = strlen(percentage);
    int doublePercentageLen = strlen(doublePercentage);

    while ((found = strstr(str, percentage)) != NULL) {
        if (found > str && *(found - 1) == '%') {
            str = found + percentageLen;
            continue;
        }

        memmove(found + doublePercentageLen, found + percentageLen, strlen(found + percentageLen) + 1);
        memcpy(found, doublePercentage, doublePercentageLen);

        str = found + doublePercentageLen;
    }
}

// Blahaj helps you take a string and make it gay. It does not print.
char* stringhaj(int offset,color_scheme* cscheme, char* buffer) {
    color_scheme scheme = *cscheme;
    char *returnstr;
    char *line;
    char* save;
    // get line from string
    int totlen = 0;
    int i = 0;
    while (1==1) {
        int* colors = scheme.colors[(offset % scheme.color_count)];
        //char ansi_code[20];
          
        if (i == 0 ){
            line = strtok_r(buffer,"\n",&save);
            if (line == NULL) {
                break;
            }
            int colline_len = strlen(line)+17+2;
            char colline[colline_len];
            sprintf(colline, "\033[38;2;%d;%d;%dm %s", colors[0], colors[1], colors[2], line); 
            replacePercentage(colline);

            totlen += (strlen(colline) + 2);
            returnstr = (char*)calloc(totlen, sizeof(char));
            sprintf(returnstr,"%s%s\n", returnstr,colline);
        } else {
            line = strtok_r(NULL,"\n",&save);
            if (line == NULL)
                break;
            int colline_len = strlen(line)+17+2;
            char colline[colline_len];
            sprintf(colline, "\033[38;2;%d;%d;%dm %s", colors[0], colors[1], colors[2], line); 
            replacePercentage(colline);
            totlen += (strlen(colline) + 2);
            returnstr = (char*)realloc(returnstr,totlen * sizeof(char));
            if (returnstr == NULL) {
                free(returnstr);
                return NULL;    
            } else {
                sprintf(returnstr,"%s%s\n", returnstr,colline);
            }
        }
        
        i++;
        offset++;
    }
    return returnstr;
}

// Blahaj will take a string, make it gay, and print it to the terminal
void printhaj(int offset,color_scheme* cscheme, char* buffer, int linecount) {
    for (int i = 0; i <= linecount; i++)
    {
        printf("\n");
    }
    printf("\033[%dA", linecount);
    char *out = stringhaj(offset,cscheme,buffer);
    int newcount = get_linecount(out);
    
    printf(out);
    
    printf("\033[%dA", linecount);
    
    
    
    printf("%s",ANSI_COLOR_RESET);
    free(out);

}