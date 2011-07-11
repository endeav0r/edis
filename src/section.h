#ifndef section_HEADER
#define section_HEADER

#include <list>
#include <string>

#include "function.h"
#include "elf.h"

class Function;

class Section {
    private :
        Shdr & shdr;
        std::list <Function *> functions;
        
    public :
        Section (Shdr & shdr);
        ~Section ();
        
        unsigned int    g_address ();
        unsigned int    g_size();
        unsigned char * g_data();
        std::string     g_name   ();
        
        void add_function (Function * function);
        
        void sort_functions ();
        
        void process_instructions ();
        
        std::list <Function *> g_functions();
};
        
#endif
