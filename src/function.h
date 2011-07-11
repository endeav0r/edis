#ifndef function_HEADER
#define function_HEADER

#include <list>
#include <string>

#include "elf.h"
#include "instruction.h"
#include "section.h"

class Function;
class Section;

bool functions_sort (Function * a, Function * b);

class Function {
    private :
        Section & section;
        Symbol & symbol;
        unsigned int size;
        std::list <Instruction *> instructions;
    
    public :
        Function  (Section & section, Symbol & symbol, unsigned int size);
        Function  (Section & section, Symbol & symbol);
        ~Function ();
        
        void                      add_instructions ();
        void                      add_instruction (Instruction * instruction);
        std::list <Instruction *> g_instructions  ();
        
        unsigned int              g_addr_begin ();
        unsigned int              g_addr_end   ();
        unsigned int              g_size       ();
        std::string               g_name       ();
        
        void s_size (unsigned int size);
};

#endif
