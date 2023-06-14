#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "toml.h"
#include "color.h"

#define NUMBER_REGEX        "/\b([01]?[0-9][0-9]?|2[0-4][0-9]|25[0-5])"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Get a color scheme from conf.toml
color_scheme get_colors(char* scheme){
    FILE *fd;

    int cap = 8; //Color 'cap'

    color_scheme cscheme;
    cscheme.colors = malloc(sizeof(int)* 3 * cap);
    char errbuf[200];
    
    fd = fopen("res/conf.toml","r");
    if (!fd) {
        perror("fopen");
    }

    toml_table_t* conf = toml_parse_file(fd,errbuf,sizeof(errbuf));
    fclose(fd);
    if (!conf) {
        perror("cannot parse - ");
    }

    // Grab a section (IG it's called a table?)
    toml_table_t* server = toml_table_in(conf, scheme);
    if (!server) {
        
        toml_free(conf);
        free(cscheme.colors);
        perror("missing color scheme");
        exit(1);
    }

    toml_array_t* colarray = toml_array_in(server, "colors");
    if (!colarray) {
        perror("cannot read scheme.colors");
    }
    for (int i = 0; ; i++) {
        toml_array_t* color = toml_array_at(colarray, i);
        if (!color) {
            cscheme.color_count = i;
            break;
        }
        
        toml_datum_t rtmp = toml_int_at(color,0);
        if (!rtmp.ok)
        {
            printf("done fucked up\n");
            break;
        }
        int r = (int)rtmp.u.i;
        int g = (int)toml_int_at(color,1).u.i;
        int b = (int)toml_int_at(color,2).u.i;
        int* coArray = malloc(sizeof(int) * 3);
        //printf("hewwo r: %d g: %d b: %d\n", r,g,b);
        coArray[0] = r;
        coArray[1] = g;
        coArray[2] = b;
        //printf("hewwo %ls\n", coArray);
        cscheme.colors[i] = coArray;
    }
    toml_free(conf);
    return cscheme;
}

// Free memory from a color_scheme
void free_scheme(color_scheme scheme) {
    for (int i = 0; i < scheme.color_count; i++)
    {
        free(scheme.colors[i]);
    }
    
    free(scheme.colors);
}

int get_filesize(FILE **fpointer) {
    FILE *fd = *fpointer;
    fseek(fd, 0L, SEEK_END);
    int size = ftell(fd);
    rewind(fd);
    return size;
}

int get_linecount(const char* str) {
    int count = 1;

    while (*str != '\0') {
        if (*str == '\n') {
            count++;
        }
        if (*str == '\r')
        {
            count++;
        }
        
        str++;
    }

    return count;
}

void file_print_once(char* fname, char* cscheme){
    FILE *fd;
    color_scheme scheme = get_colors(cscheme);
    fd = fopen(fname,"r");
    if (!fd) {
        perror("fopen");
    }
    int size = get_filesize(&fd);
    char* buffer = malloc((size+1) * sizeof(char));// haj is 1377 chars long
    if (!buffer) {
        fclose(fd);
        free_scheme(scheme);
        return;
    }
    fread(buffer,1,size,fd);
    buffer[size] = '\0'; 
    int lc = get_linecount(buffer);   
    printf("Lines in file : %d\n",lc); 
    char* buf = strdup(buffer);
    printhaj(1,&scheme,buf,lc);
    int ld = get_linecount(buf);
    printf("\033[%dB", lc);
    free(buf);
    
    
    fclose(fd);
    free(buffer);
    free_scheme(scheme);
    printf("%s",ANSI_COLOR_RESET);

}

void color_row_change(char* buffer, char* cscheme){
    color_scheme scheme = get_colors(cscheme);
    int lc = get_linecount(buffer);
    int tim_number = (lc/scheme.color_count)*scheme.color_count+1;
    int spedmul = 4;
    int timemul = 1;
    const struct timespec waittime = {0,(500000000/spedmul)};
    for (int mul = 0; mul < timemul; mul++)
    {    /* code */ 
        for (int i = 0; i < tim_number; i++)
        {
            char* buf = strdup(buffer);
            printhaj(i,&scheme,buf,lc);
            free(buf);
            nanosleep(&waittime,NULL);
        }
        nanosleep(&waittime,NULL);
        if (mul == (timemul-1)) {
            printf("\033[%dB", lc);
        }
    }
    free_scheme(scheme);
}

void file_row_change(char* fname, char* cscheme){
    FILE *fd;
    fd = fopen(fname,"r");
    if (!fd) {
        perror("fopen");
    }
    int size = get_filesize(&fd);
    char* buffer = malloc((size+1) * sizeof(char));// haj is 1377 chars long
    buffer[size] = '\0'; 
    
    fread(buffer,1,size,fd);
    color_row_change(buffer,cscheme);
    
    free(buffer);
    fclose(fd);
    

}

int main (int argc, char** argv){
    
    char *flag = NULL;
    char *path = NULL;
    int mode;
    int c;

    mode = 0;
    while ((c = getopt(argc, argv, "dc:f:")) != -1)
    switch (c)
    {
        case 'c':
            flag = optarg;
            break;
        case 'd':
            mode = 1;
            break;
        case 'f':
            path = optarg;
            break;
    }
    if (flag == NULL)
    {
        flag = "trans";
    }
    if (path == NULL)
    {
        path = "res/ascii.txt";
    }
    if (mode == 1) {
        file_row_change(path,flag);
    } else {
        file_print_once(path,flag);
    }
    
    
    
    return 0;
}
