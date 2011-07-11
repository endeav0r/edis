#include "function.h"

bool functions_sort (Function * a, Function * b) {
    if (a->g_addr_begin() < b->g_addr_begin())
        return true;
    return false;
}

Function :: Function (Section & section, Symbol & symbol, unsigned int size)
                   :  section (section), symbol (symbol), size (size) {}


Function :: Function (Section & section, Symbol & symbol)
                   :  section (section), symbol (symbol) {}


Function :: ~Function () {
    std::list <Instruction *> :: iterator it;
    for (it = instructions.begin(); it != instructions.end(); it++) {
        delete *it;
    }
}


void Function :: add_instructions () {
    int offset_begin;
    int offset_end;
    int offset_ins;
    Instruction * ins;
    
    offset_begin = g_addr_begin() - section.g_address();
    offset_end   = offset_begin + size;
    offset_ins   = offset_begin;
    
    while (offset_ins < offset_end) {
        ins = new InstructionX86(&(section.g_data()[offset_ins]),
                                 offset_end - offset_ins,
                                 section.g_address() + offset_ins);
        if (ins->g_size() == 0) {
            delete ins;
            break;
        }
        this->instructions.push_back(ins);
        offset_ins += ins->g_size();
    }
} 


void Function :: add_instruction (Instruction * instruction) {
    instructions.push_back(instruction);
}


std::list <Instruction *> Function :: g_instructions () { return instructions; }

unsigned int Function :: g_addr_begin () { return this->symbol.g_value(); }
unsigned int Function :: g_addr_end   () {
    return this->symbol.g_value() + this->size;
}
unsigned int Function :: g_size       () { return size; }
std::string Function :: g_name () { return this->symbol.g_name(); }

void Function :: s_size (unsigned int size) { this->size = size; }
