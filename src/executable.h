#ifndef executable_HEADER
#define executable_HEADER

#include <cstdio>

#include "elf.h"
#include "section.h"

class Executable {

    private :
        Elf & elf;
        std::list <Section *>  exec_sections; // executable sections
        std::list <Symbol *>   symbols;
        
        void add_symbols         ();
        void add_symbols_factory (Shdr & symbol_shdr);
        void add_exec_sections   ();
        void add_functions       ();
        void add_instructions    ();
        void set_instruction_offset_labels ();
        
        std::string get_offset_label (unsigned int address);
        
        void sort_symbols ();
    
    public :
        Executable (Elf & elf);
        
        void load ();
        std::list <Section *> g_sections();
};

#endif
