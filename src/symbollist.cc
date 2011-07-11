SymbolList :: ~SymbolList () {
    std::list <Symbol *> :: iterator :: it;
    
    for (it = symbols.begin(); it != symbols.end(); it++) {
        delete *it;
    }
}
