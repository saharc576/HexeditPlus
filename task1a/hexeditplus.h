#define  NAME_LEN  128
#define  BUF_SZ    10000

typedef struct {
  char debug_mode;
  char file_name[NAME_LEN];
  int unit_size;
  unsigned char mem_buf[BUF_SZ];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;  
  
  struct fun_desc {
        char *name;
        void (*fun)(state*);
    };



/* When the debug mode is on, the following values in the state are printed:
   unit_size, file_name, and mem_count,
   every time just before the menu is printed*/     
void toggleDebugMode (state*);

void setFileName (state *s);
void setUnitSize (state *s);
void loadIntroMemory (state *s);
void loadIntoMemory (state *s);
void memoryDisplay (state *s);
void saveIntoFile (state *s);
void memoryModify (state *s);
void quit (state *s);
void print_units(char* address, int unit_size, char *formats[]); 
