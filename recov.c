#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "toml.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BLAHAJ_LINECOUNT 17

#define NUMBER_REGEX        "/\b([01]?[0-9][0-9]?|2[0-4][0-9]|25[0-5])"


typedef struct
{
    int** colors;
    int color_count;
} color_scheme;


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

void free_scheme(color_scheme scheme) {
    for (int i = 0; i < scheme.color_count; i++)
    {
        free(scheme.colors[i]);
    }
    
    free(scheme.colors);
}

void color_by_line(char* fname, char* cscheme){
    FILE *fd;
    color_scheme scheme = get_colors(cscheme);
    fd = fopen(fname,"r");
    if (!fd) {
        perror("fopen");
    }
    char output[90]; // haj is 1376 chars long
    int i = 0;
    while (fgets(output,90,fd) != NULL)
    {
        /* code */
        if (i%3 == 0)
        {
            printf("%s %s",ANSI_COLOR_BLUE,output);
        } else if (i%3 ==1)
        {
            printf("%s %s",ANSI_COLOR_MAGENTA,output);
        } else {
            printf("%s %s",ANSI_COLOR_WHITE,output);
        }
        
        
        i++;
    }
    fclose(fd);
    free_scheme(scheme);
    printf(ANSI_COLOR_RESET);

}

int main (int argc, char** argv){
    
    char *flag = NULL;
    int c;

    while ((c = getopt(argc, argv, "c:")) != -1)
    switch (c)
    {
      case 'c':
        flag = optarg;
        break;
    }
    if (flag == NULL)
    {
        flag = "trans";
    }
    
    
    
    color_by_line("res/ascii.txt",flag);
    
    printf("\n");
    
    return 0;
}
