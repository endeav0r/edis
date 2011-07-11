#include <list>
#include <string>

#include <libdis.h>
#include <cstdio>

#include "elf.h"
#include "executable.h"
#include "function.h"
#include "gui.h"
#include "instruction.h"
#include "section.h"

#define STR_BUF_SIZE 64



int main (int argc, char * argv[]) {
    int offset;
    Elf * elf;
    
    std::list <Section *> sections;
    std::list <Section *> :: iterator sit;
    
    std::list <Function *> functions;
    std::list <Function *> :: iterator fit;
    
    std::list <Instruction *> instructions;
    std::list <Instruction *> :: iterator iit;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <ELF>\n", argv[0]);
        exit(-1);
    }
    
    x86_init(opt_none, NULL, NULL);
    Gtk::Main kit(argc, argv);
    
    
    elf = elf_loadfile(argv[1]);
    Gui gui; 
    Executable executable(*elf);
    executable.load();
    
    sections = executable.g_sections();
    for (sit = sections.begin(); sit != sections.end(); sit++) {
        gui.add_section((*sit)->g_address(),
                        (int) (*sit)->g_size(),
                        (*sit)->g_name());
        functions = (*sit)->g_functions();
        for (fit = functions.begin(); fit != functions.end(); fit++) {
            gui.add_function((*fit)->g_addr_begin(),
                             (int) (*fit)->g_size(),
                             (*fit)->g_name());
            instructions = (*fit)->g_instructions();
            offset = 0;
            for (iit = instructions.begin(); iit != instructions.end(); iit++) {
                gui.add_instruction((*iit)->g_address(),
                                    offset,
                                    (*iit)->g_description());
                offset += (int) (*iit)->g_size();
            }
        }
    }
    
    Gtk::Main::run(gui);
    x86_cleanup();
    
    delete elf;
    
    return 0;
}
