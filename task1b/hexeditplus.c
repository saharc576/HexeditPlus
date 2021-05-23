#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hexeditplus.h"

static char* hex_formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
static char* dec_formats[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
int main(int argc, char **argv)
{
    struct fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDebugMode},
        {"Set File Name", setFileName},
        {"Set Unit Size", setUnitSize},
        {"Load Into Memory", loadIntoMemory},
        {"Memory Display", memoryDisplay},
        {"Save Into File", saveIntoFile},
        {"Memory Modify", memoryModify},
        {"Quit", quit}
        };

    int upperBound = (sizeof(menu) / sizeof(struct fun_desc));
    int i;
    char input[BUF_SZ];
    int choise;
    char c;
    state *s = (state*)calloc(sizeof(state), 1);

    while (1) 
    {
        printf("Choose actoin:\n");

        for (i = 0; i < upperBound ; i++) 
        {
            printf("%d- %s\n", i, menu[i].name);
        }

        printf("option: ");
        
        
        fgets(input,BUF_SZ,stdin);
        sscanf(input, "%d%c", &choise, &c);

        if ( c != '\n' || choise >= upperBound || choise < 0) 
        {
            printf("Not within bounds\n");
            // free(carray);
            exit(EXIT_FAILURE);
        }
        else 
        {
            printf("Within bounds\n");
        }
        
        menu[choise].fun(s);

        printf("DONE.\n");
    }

    return EXIT_SUCCESS;
}


void toggleDebugMode (state *s)
{
    if (s->debug_mode == 0)
    {
        s->debug_mode = 1;
        printf("Debug flag now on\n");
    }
    else if (s->debug_mode == 1)
    {
        s->debug_mode = 0;
        printf("Debug flag now off\n");
    }

}

void setFileName (state *s)
{
    char input[NAME_LEN];
    char file[NAME_LEN];
    printf("Enter file name: ");
    fflush(stdout);
     
    fgets(input,BUF_SZ,stdin);
    sscanf(input, "%s", file);

    strcpy(s->file_name, file);

    if (s->debug_mode == 1)
    {
        fprintf(stderr, "Debug: file name set to %s\n", s->file_name);
        fflush(stderr);
    }

}
void setUnitSize (state *s)
{
    char input[BUF_SZ];
    int choise;
    char c;
    printf("Enter number: ");
    fflush(stdout);  

    fgets(input,BUF_SZ,stdin);
    sscanf(input, "%d%c", &choise, &c);

    if ( c != '\n' || (choise != 1 && choise != 2 && choise != 4)) 
    {
        printf("Not a valid size\n");
        fflush(stdout);
        return;
    }

    s->unit_size = choise;

    if (s->debug_mode == 1)
    {
        fprintf(stderr, "Debug: set size to %d\n", s->unit_size);
        fflush(stderr);
    }
}
void loadIntroMemory (state *s) 
{
    fprintf(stdout, "not implemented yet\n");
    fflush(stdout);

}

void loadIntoMemory (state *s)
{
    FILE *file;
    char input[BUF_SZ];
    int location;
    int length;
    if (strcmp(s->file_name, "") == 0)
    {
        printf("Please set file name first\n");
        fflush(stdout);
        return;
    }
    if ((file = fopen(s->file_name, "r")) == NULL)
    {
        printf("Please provide reading premission\n");
        fflush(stdout);
        return;
    }

    printf("Please enter <location> <length>\n");
    fflush(stdout);  

    fgets(input,BUF_SZ,stdin);
    sscanf(input, "%X%d", &location,&length);

    
    if (s->debug_mode == 1)
    {
        fprintf(stderr, "Debug: file name is %s\n", s->file_name);
        fflush(stderr);
        fprintf(stderr, "Debug: location is %X\n", location);
        fflush(stderr);
        fprintf(stderr, "Debug: length is %d\n", length);
        fflush(stderr);
    }

    fseek(file, location, SEEK_SET);
    fread(s->mem_buf, s->unit_size, length, file);

    fclose(file);
}
void memoryDisplay (state *s)
{
    FILE *file;
    char input[BUF_SZ];
    int address;
    int length;
    char *ptr;
    char *end;
    printf("Enter address and length\n");
    fflush(stdout);

    fgets(input,BUF_SZ,stdin);
    sscanf(input, "%X%d", &address,&length);

    printf("Hexadecimal\n===========\n");
    if (address == 0)   /* read from mem_buf */
    {
        print_units(s->mem_buf, s->unit_size, length, hex_formats[s->unit_size -1]);
    }

    else                /* read from memory */
    {
        ptr = (char *)address; 
        end = ptr + (s->unit_size)*length;
        while (ptr < end) 
        {
            
            printf(hex_formats[s->unit_size -1], *(ptr) - '0');
            ptr = ptr + s->unit_size;
        }           
    }

    printf("Decimal\n===========\n");
    if (address == 0)   /* read from mem_buf */
    {
        print_units(s->mem_buf, s->unit_size, length, dec_formats[s->unit_size -1]);      
    }
    else                /* read from memory */
    {
        ptr = (char *)address; 
        end = ptr + (s->unit_size)*length;
        while (ptr < end) 
        {
            printf(dec_formats[s->unit_size -1], *(ptr) - '0');
            ptr = ptr + s->unit_size;
        }       
    }
}


void saveIntoFile (state *s)
{
    fprintf(stdout, "not implemented yet\n");
    fflush(stdout);

}
void memoryModify (state *s)
{


    fprintf(stdout, "not implemented yet\n");
    fflush(stdout);
}

void quit (state *s)
{
    if (s->debug_mode == 1)
    {
        fprintf(stderr, "Debug: quitting\n");
        fflush(stderr);
    }
    exit(EXIT_SUCCESS);
}

void print_units(char* address, int unit_size, int length, char *format) 
{
    char* end;
    char* curr;
    int var;
    end = address + (unit_size)*length;
    curr = address;
    while (curr < end) 
    {
        var = *((int*)(curr));
        fprintf(stdout, format, var);
        curr = curr + unit_size;
    }
}
