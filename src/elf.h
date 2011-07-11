#ifndef elf_HEADER
#define elf_HEADER

#include <string>

#include <elf.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class SymbolFactory;

class Elf;
class Elf32;
class Shdr;
class Shdr32;
class Symbol;
class Symbol32;


Elf    * elf_loadfile (std::string filename);
int      symbol_value_compare (Symbol * a, Symbol * b);


class SymbolFactory {
    private :
        Elf &elf;
        Shdr &shdr;
    public :
        SymbolFactory (Elf &elf, Shdr &shdr);
        Symbol * g_symbol (int index);
};

class Symbol {
    protected :
        int index;
        int shdr_index;
        Elf &elf;
    
    public :
        Symbol (Elf &elf, Shdr &shdr, int index);
        
        virtual std::string  g_name    () = 0;
        virtual unsigned int g_value   () = 0;
        virtual unsigned int g_type    () = 0;
};



class Symbol32 : public Symbol {
    private :
        Elf32_Sym sym;
    public :
        Symbol32 (Elf32 &elf, Shdr32 &shdr, int index);
        
        std::string  g_name    ();
        unsigned int g_value   ();
        unsigned int g_type    ();
};



class Shdr {
    protected :
        int index;
        Elf &elf;
    
    public :
        Shdr  (Elf &elf, int index);
        
        virtual int             g_index   () = 0;
        virtual unsigned char * g_data    () = 0;
        virtual int             g_link    () = 0;
        virtual std::string     g_name    () = 0;
        virtual int             g_num     () = 0;
        virtual int             g_offset  () = 0;
        virtual int             g_size    () = 0;
        virtual int             g_type    () = 0;
        virtual unsigned int    g_address () = 0;
        
        virtual bool is_executable () = 0;
        
        virtual Elf &           g_elf () = 0;
};


class Shdr32 : public Shdr {
    private :
        Elf32_Shdr * shdr;
    
    public :
        Shdr32 (Elf32 &elf, int index);
        
        int             g_index   ();
        unsigned char * g_data    ();
        int             g_link    ();
        std::string     g_name    ();
        int             g_num     ();
        int             g_offset  ();
        int             g_size    ();
        int             g_type    ();
        unsigned int    g_address ();
        
        bool is_executable ();
        
        Elf &           g_elf ();
};



class Elf {
    protected :
        std::string filename;
        int data_size;
        unsigned char * data;
    
    public :
        Elf  (std::string filename); 
        ~Elf ();
        
        virtual int             g_e_shoff     () = 0;
        virtual int             g_e_shentsize () = 0;
        virtual int             g_e_shstrndx  () = 0;
        virtual int             g_e_shnum     () = 0;
        virtual unsigned char * g_data        () = 0;
        virtual Shdr *          g_shdr        (int index) = 0;
        virtual Shdr *          g_shdr        (std::string name) = 0;
        virtual unsigned char   g_class       () = 0;
};



class Elf32 : public Elf {
    private :
        Elf32_Ehdr * ehdr;
    
    public :
        Elf32 (std::string filename);
        
        int             g_e_shoff     ();
        int             g_e_shentsize ();
        int             g_e_shstrndx  ();
        int             g_e_shnum     ();
        unsigned char * g_data        ();
        Shdr32 *        g_shdr        (int index);
        Shdr32 *        g_shdr        (std::string name);
        unsigned char   g_class       ();
        
        Symbol32      * symbol_at_address (unsigned int address);
};



#endif
