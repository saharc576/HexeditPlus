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

       if (s->debug_mode == 1)
        {
            fprintf(stderr, "Debug:\nunit size: %d\nfile name: %s\nmem_count: %d\n",s->unit_size, s->file_name, s->mem_count);
            fflush(stderr);
        }
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
            free(s);
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
    s->mem_count = fread(s->mem_buf, s->unit_size, length, file);
    s->mem_count = (s->mem_count) * (s->unit_size);

    fclose(file);
}
void memoryDisplay (state *s)
{
    char input[BUF_SZ];
    int address;
    int length;
    unsigned char *ptr;
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
        ptr = (unsigned char *)address; 
        print_units(ptr, s->unit_size, length, hex_formats[s->unit_size -1]);
    }

    printf("Decimal\n===========\n");
    if (address == 0)    /* read from mem_buf */
    {
        print_units(s->mem_buf, s->unit_size, length, dec_formats[s->unit_size -1]);      
    }
    else                 /* read from memory */
    {
        ptr = (unsigned char *)address ;
        print_units(ptr, s->unit_size, length, dec_formats[s->unit_size -1]);      
    }
}


void saveIntoFile (state *s)
{
    FILE *output;
    int source_address;
    int target_location;
    int length;
    int file_size;
    unsigned char *ptr;
    char input[BUF_SZ];

    fprintf(stdout, "Please enter <source-address> <target-location> <length>\n");
    fflush(stdout);

    fgets(input,BUF_SZ,stdin);
    sscanf(input, "%X%X%d", &source_address,&target_location, &length);
    
    if ((output = fopen(s->file_name, "rb+")) == NULL)
    {
        if (s->debug_mode == 1)
        {
            printf("Please provide writing premission\n");
            fflush(stderr);
        }
        return;
    }

    fseek(output, 0L, SEEK_END);
    file_size = ftell(output);
    fseek(output, 0L, SEEK_SET);

    if (file_size < target_location)
    {
        if (s->debug_mode == 1)
        {
            printf("Error: target_location = %d > file_size = %d\n", target_location, file_size);
            fflush(stderr);
        }
        return;
    }

    fseek(output, target_location, SEEK_SET);

    if (source_address == 0)
    {
        fwrite(s->mem_buf, s->unit_size, length, output);
    }
    else 
    {
        ptr = (unsigned char *)source_address;
        fwrite(ptr, s->unit_size, length, output);
    }
    fclose(output);


}

void memoryModify (state *s)
{
    char input[BUF_SZ];
    int location;
    int val;
    char *location_ptr;
    char *val_ptr;
    unsigned char *end;

    fprintf(stdout, "Please enter <location> <val>\n");
    fflush(stdout);
    
    fgets(input,BUF_SZ,stdin);
    sscanf(input, "%X%X", &location,&val);

    if (s->debug_mode == 1)
    {
        printf("Debug:\nlocation = %X\nval = %X\n", location, val);
        fflush(stderr);
    }

    location_ptr = (char*)location;
    end = s->mem_buf + (s->mem_count - 1)*(s->unit_size);
    if (location_ptr > end)
    {
        if (s->debug_mode == 1)
        {
            printf("Debug: no such location: %d\n", location);
            fflush(stderr);
        }
        return;
    }
    location_ptr = (s->mem_buf)+location;
    memcpy(location_ptr, &val, s->unit_size);
}


void quit (state *s)
{
    if (s->debug_mode == 1)
    {
        fprintf(stderr, "Debug: quitting\n");
        fflush(stderr);
    }
    free(s);
    exit(EXIT_SUCCESS);
}

void print_units(unsigned char* address, int unit_size, int length, char *format) 
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
