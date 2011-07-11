#include "executable.h"


Executable :: Executable (Elf & elf) : elf(elf) {}


void Executable :: add_symbols_factory (Shdr & symbol_shdr) {
    int symbol_i;
    Symbol * symbol;
    SymbolFactory symbolFactory(elf, symbol_shdr);
    
    for (symbol_i = 0; symbol_i < symbol_shdr.g_num(); symbol_i++) {
        symbol = symbolFactory.g_symbol(symbol_i);
        this->symbols.push_back(symbol);
    }
}


void Executable :: add_symbols () {
    Shdr * shdr;
    
    // load all the symbols
    shdr = elf.g_shdr(".symtab");
    if (shdr != NULL) {
        add_symbols_factory(*shdr);
        delete shdr;
    }
    
    shdr = elf.g_shdr(".dynsym");
    if (shdr != NULL) {
        add_symbols_factory(*shdr);
        delete shdr;
    }
}


void Executable :: sort_symbols () {
    symbols.sort(symbol_value_compare);
}


void Executable :: add_exec_sections () {
    int shdr_i;
    Shdr * shdr;
    
    for (shdr_i = 0; shdr_i < elf.g_e_shnum(); shdr_i++) {
        shdr = elf.g_shdr(shdr_i);
        if (shdr->is_executable())
            this->exec_sections.push_back(new Section((Shdr &) *shdr));
        else
            delete shdr;
    }
}


void Executable :: add_functions () {
    unsigned int laaddress; // la = lookahead
    Section * section;
    Symbol * symbol, * lasymbol;
    
    std::list <Section *> :: iterator secit;
    std::list <Symbol *>  :: iterator symit, lasymit;
    
    // for each executable section
    for (secit = exec_sections.begin(); secit != exec_sections.end(); secit++) {
        section = *secit;
        // find function symbols that fit within this section
        for (symit = symbols.begin(); symit != symbols.end(); symit++) {
            // is this symbol executable and in this section
            symbol = *symit;
            if (    (symbol->g_type() == STT_FUNC)
                 && (symbol->g_value() >= section->g_address())
                 && (symbol->g_value() <  section->g_address() + section->g_size())) {

                // find next func symbol for lookahead address
                laaddress = section->g_address() + section->g_size();
                lasymit = symit;
                for (lasymit++; lasymit != symbols.end(); lasymit++) {
                    lasymbol = *lasymit;
                    if (lasymbol->g_type() == STT_FUNC) {
                        if (    (lasymbol->g_value() >= section->g_address())
                             && (lasymbol->g_value() <  section->g_address() + section->g_size()))
                            laaddress = lasymbol->g_value();
                        break;
                    }
                }
                // add function to section with size based of laaddress from
                // next executable symbol
                section->add_function(new Function((Section &) *section,
                                                   *symbol,
                                                   laaddress - symbol->g_value()));
            }
        }
    }
}


void Executable :: add_instructions () {
    Section  * section;
    Function * function;
    std::list <Function *> functions;
    std::list <Section *>  :: iterator  sit;
    std::list <Function *> :: iterator fit;
    
    for (sit = exec_sections.begin(); sit != exec_sections.end(); sit++) {
        section = *sit;
        functions = section->g_functions();
        for (fit = functions.begin();
             fit != functions.end(); fit++) {
            function = *fit;
            function->add_instructions();
        }
    }
}


std::string Executable :: get_offset_label (unsigned int address) {
    char offset_label[256];
    std::list <Function *> functions;
    std::list <Section *> :: iterator sit;
    std::list <Function *> :: iterator fit;
    
    for (sit = exec_sections.begin(); sit != exec_sections.end(); sit++) {
        if (    (address >= (*sit)->g_address())
             && (address < (*sit)->g_address() + (*sit)->g_size())) {
            functions = (*sit)->g_functions();
            for (fit = functions.begin();
                 fit != functions.end();
                 fit++) {
                if (    (address >= (*fit)->g_addr_begin())
                     && (address < (*fit)->g_addr_end())) {
                    snprintf(offset_label, 256, "<%s+%x>",
                             (*fit)->g_name().c_str(),
                             address - (*fit)->g_addr_begin());
                    return offset_label;
                }
            }
        }
    }
    return offset_label;
}


void Executable :: set_instruction_offset_labels () {
    unsigned int address;
    Section * section;
    Function * function;
    Instruction * instruction;
    
    std::list <Function *> functions;
    std::list <Instruction *> instructions;
    std::list <Section *>  :: iterator sit;
    std::list <Function *> :: iterator fit;
    std::list <Instruction *> :: iterator iit;
    
    for (sit = exec_sections.begin(); sit != exec_sections.end(); sit++) {
        section = *sit;
        functions = section->g_functions();
        for (fit = functions.begin();
            fit != functions.end(); fit++) {
            function = *fit;
            instructions = function->g_instructions();
            for (iit = instructions.begin(); iit != instructions.end(); iit++) {
                if (((*iit)->is_jump()) || ((*iit)->is_call())) {
                    instruction = *iit;
                    address = instruction->g_address();
                    address += (unsigned int) instruction->g_relative_offset();
                    address += instruction->g_size();
                    instruction->s_offset_label(get_offset_label(address));
                }
            }
        }
    }
}


void Executable :: load () {
    printf("adding symbols\n");
    add_symbols      ();
    sort_symbols     ();
    printf("adding executable sections\n");
    add_exec_sections();
    printf("adding functions\n");
    add_functions    ();
    printf("adding instructions\n");
    add_instructions ();
    printf("setting instruction offset labels\n");
    set_instruction_offset_labels();
    printf("done\n");
}


std::list <Section *> Executable :: g_sections() { return exec_sections; }
