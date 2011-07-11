#include "elf.h"


Elf * elf_loadfile (std::string filename) {
    int bytes_read;
    FILE * fh;
    Elf32_Ehdr ehdr;
    
    fh = fopen(filename.c_str(), "rb");
    if (fh == NULL) {
        fprintf(stderr, "Error reading file %s\n", filename.c_str());
        throw -1;
    }
    
    bytes_read = fread(&ehdr, 1, sizeof(Elf32_Ehdr), fh);
    if (bytes_read != sizeof(Elf32_Ehdr)) {
        fprintf(stderr, "Error reading elf header file %s\n", filename.c_str());
        throw -1;
    }
    
    fclose(fh);
    
    if (ehdr.e_ident[EI_CLASS] == ELFCLASS32)
        return new Elf32(filename);
    else
        throw -2;
}


int symbol_value_compare (Symbol * a, Symbol * b) {
    if (a->g_value() < b->g_value())
        return true;
    return false;
}



/*******************
* SymbolFactory    *
*******************/


SymbolFactory :: SymbolFactory (Elf &elf, Shdr &shdr) : elf(elf), shdr(shdr) {}

Symbol * SymbolFactory :: g_symbol (int index) {
    if (elf.g_class() == ELFCLASS32) {
        return new Symbol32((Elf32 &) elf, (Shdr32 &) shdr, index);
    }
    
    return NULL;
}


/*************
* Symbol    *
*************/


Symbol :: Symbol (Elf &elf, Shdr &shdr, int index) : index(index), elf(elf) {
    this->shdr_index = shdr.g_index();
}



/*************
* Symbol32   *
*************/


Symbol32 :: Symbol32 (Elf32 &elf, Shdr32 &shdr, int index) : Symbol(elf, shdr, index) {
    memcpy(&(this->sym),
           &(elf.g_data()[shdr.g_offset() + (index * sizeof(Elf32_Sym))]),
           sizeof(Elf32_Sym));
}


std::string Symbol32 :: g_name () {
    unsigned char * data;
    Shdr * strtab, * shdr;
    std::string s;
    
    shdr   = this->elf.g_shdr(this->shdr_index);
    strtab = this->elf.g_shdr(shdr->g_link());
    data   = strtab->g_data();
    
    s = (char *) &(data[this->sym.st_name]);
    delete strtab;
    delete shdr;
    
    return s;
}


unsigned int Symbol32 :: g_value () { return this->sym.st_value; }
unsigned int Symbol32 :: g_type  () { return ELF32_ST_TYPE(this->sym.st_info); }



/*************
* Shdr       *
*************/


Shdr :: Shdr (Elf &elf, int index) : index (index), elf (elf) {
}


/**************
* Shdr32      *
**************/


Shdr32 :: Shdr32 (Elf32 &elf, int index) : Shdr (elf, index) {
    unsigned char * data;
    data = this->elf.g_data();
    
    this->shdr = (Elf32_Shdr *) &(data[this->elf.g_e_shoff()
                                       + (this->elf.g_e_shentsize() * index)]);
}


std::string Shdr32 :: g_name () {
    unsigned char * data;
    Shdr * shstrndx;
    
    shstrndx = this->elf.g_shdr(this->elf.g_e_shstrndx());
    data = shstrndx->g_data();
    delete shstrndx;
    
    return (char *) &(data[this->shdr->sh_name]);
}


unsigned char * Shdr32 :: g_data () {
    return &(this->elf.g_data()[this->shdr->sh_offset]);
}

int Shdr32 :: g_link   () { return this->shdr->sh_link; }
int Shdr32 :: g_offset () { return this->shdr->sh_offset; }
int Shdr32 :: g_size   () { return this->shdr->sh_size; }
int Shdr32 :: g_type   () { return this->shdr->sh_type; }
unsigned int Shdr32 :: g_address () { return this->shdr->sh_addr; }
Elf & Shdr32 :: g_elf    () { return this->elf; }
int Shdr32 :: g_index  () { return this->index; }

int Shdr32 :: g_num    () {
    if (this->shdr->sh_entsize == 0)
        return 0;
    return this->shdr->sh_size / this->shdr->sh_entsize;
}

bool Shdr32 :: is_executable () {
    if (this->shdr->sh_flags & SHF_EXECINSTR) return true;
    return false;
}


/*************
* Elf        *
*************/


Elf :: Elf (std::string filename) {
    size_t filesize;
    FILE * fh;
    
    this->data = NULL;
    
    fh = fopen(filename.c_str(), "rb");
    if (fh == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename.c_str());
        throw -1;
    }
    
    fseek(fh, 0, SEEK_END);
    filesize = ftell(fh);
    fseek(fh, 0, SEEK_SET);
    
    this->data = (unsigned char *) malloc(filesize);
    
    if (fread(this->data, 1, filesize, fh) != filesize) {
        fprintf(stderr, "Error reading file %s\n", filename.c_str());
        throw -1;
    }
    
    fclose(fh);
    
    this->data_size = filesize;
    this->filename = filename;
}


Elf :: ~Elf () {
    if (this->data != NULL)
        free(this->data);
}


/*************
* Elf32      *
*************/

Elf32 :: Elf32 (std::string filename) : Elf (filename) {
    this->ehdr = (Elf32_Ehdr *) this->data;
}


int Elf32 :: g_e_shoff     () { return this->ehdr->e_shoff; }
int Elf32 :: g_e_shentsize () { return this->ehdr->e_shentsize; }
int Elf32 :: g_e_shstrndx  () { return this->ehdr->e_shstrndx; }
int Elf32 :: g_e_shnum     () { return this->ehdr->e_shnum; }
unsigned char Elf32 :: g_class       () { return this->ehdr->e_ident[EI_CLASS]; }

unsigned char * Elf32 :: g_data () { return this->data; }

Shdr32 * Elf32 :: g_shdr (int index) {
    return new Shdr32((Elf32 &) *this, index);
}


Shdr32 * Elf32 :: g_shdr (std::string name) {
    int i;
    Shdr32 * shdr;
    
    for (i = 0; i < this->g_e_shnum(); i++) {
        shdr = new Shdr32((Elf32 &) *this, i);
        if (shdr->g_name() == name)
            return shdr;
        delete shdr;
    }
    
    return NULL;
}


Symbol32 * Elf32 :: symbol_at_address (unsigned int address) {
    int shdr_i, symbol_i;
    Shdr32 * shdr;
    Symbol32 * symbol;
    
    for (shdr_i = 0; shdr_i < this->ehdr->e_shnum; shdr_i++) {
        shdr = this->g_shdr(shdr_i);
        if (shdr->g_type() == SHT_SYMTAB) {
            for (symbol_i = 0; symbol_i < shdr->g_num(); symbol_i++) {
                symbol = new Symbol32((Elf32 &) *this, (Shdr32 &) *shdr, symbol_i);
                if (symbol->g_value() == address) {
                    delete shdr;
                    return symbol;
                }
                delete symbol;
            }
        }
        delete shdr;
    }
    
    return NULL;
}
