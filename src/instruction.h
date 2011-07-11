#ifndef instruction_HEADER
#define instruction_HEADER

#include <string>
#include <cstring>

#include <libdis.h>


class Instruction {
    public :
        virtual std::string  g_description () = 0;
        virtual int          g_size        () = 0;
        virtual unsigned int g_address     () = 0;
        virtual int          g_relative_offset() = 0;
        
        virtual void s_offset_label (std::string offset_label) = 0;
        
        virtual bool is_jump () = 0;
        virtual bool is_call () = 0;
};


class InstructionX86 : public Instruction {
    private :
        std::string offset_label;
        unsigned int address;
        int size;
        x86_insn_t ins;
    public :
        InstructionX86 (unsigned char * data, int data_size, unsigned int address);
        
        std::string  g_description     ();
        int          g_size            ();
        unsigned int g_address         ();
        int          g_relative_offset ();
        
        void s_offset_label (std::string offset_label);
        
        bool is_jump ();
        bool is_call ();
};


#endif
