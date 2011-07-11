#include "section.h"


Section :: Section (Shdr & shdr) : shdr(shdr) {}


Section :: ~Section () {
    std::list <Function *> :: iterator fit;
    
    for (fit = functions.begin(); fit != functions.end(); fit++) {
        delete *fit;
    }
}


unsigned int    Section :: g_address () { return this->shdr.g_address(); }
unsigned int    Section :: g_size    () { return this->shdr.g_size(); }
unsigned char * Section :: g_data    () { return this->shdr.g_data(); }
std::string     Section :: g_name    () { return this->shdr.g_name(); }

void Section :: add_function (Function * function) {
    this->functions.push_back(function);
}


void Section :: sort_functions () {
    functions.sort(functions_sort);
}


void Section :: process_instructions () {
    std::list <Function *> :: iterator fit;
    
    for (fit = functions.begin(); fit != functions.end(); fit++) {
        (*fit)->add_instructions();
    }
}

std::list <Function *> Section :: g_functions() { return functions; }
