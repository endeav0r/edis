#include "instruction.h"


InstructionX86 :: InstructionX86 (unsigned char * data,
                                  int data_size,
                                  unsigned int address) {
    this->size = x86_disasm(data, data_size, 0, 0, &(this->ins));
    this->address = address;
}


std::string InstructionX86 :: g_description () {
    char buf[256];
    std::string description;
    x86_format_insn(&(this->ins), buf, 64, intel_syntax);
    return std::string(buf) + " " + offset_label;
}

int InstructionX86 :: g_relative_offset () {
    return x86_get_rel_offset(&ins);
}

int InstructionX86 :: g_size () { return this->size; }
unsigned int InstructionX86 :: g_address () { return this->address; }

void InstructionX86 :: s_offset_label (std::string offset_label) {
    this->offset_label = offset_label;
}

bool InstructionX86 :: is_jump () {
    if ((ins.type == insn_jmp) || (ins.type == insn_jcc))
        return true;
    return false;
}

bool InstructionX86 :: is_call () {
    if ((ins.type == insn_call) || (ins.type == insn_callcc))
        return true;
    return false;
}
