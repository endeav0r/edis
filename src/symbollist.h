#ifndef symbollist_HEADER
#define symbollist_HEADER


#include "elf.h"

class SymbolList {
    private :
        std::list <Symbol *> symbols;
    
    public :
        ~SymbolList();
        void add (Symbol * symbol);
        void sort ();
        
        std::list <Symbol *> g_symbols();
        
        void     reset_iterator ();
        Symbol * g_next         ();
        Symbol * g_lookahead    ();
};


#endif

