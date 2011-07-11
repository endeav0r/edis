#include <iostream>
#include "gui.h"

Gui :: Gui () {
    int cols_count;
    Gtk::CellRendererText * pRenderer;
    Gtk::TreeViewColumn * pColumn;
    
    set_title("edis");
    set_border_width(5);
    set_default_size(780, 460);
    
    add(m_VBox);
    
    m_ScrolledWindow.add(m_TreeView);
    
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    
    m_VBox.pack_start(m_ScrolledWindow);
    
    this->m_refTreeModel = Gtk::TreeStore::create(m_Columns);
    m_TreeView.set_model(m_refTreeModel);
    
    
    pRenderer  = Gtk::manage(new Gtk::CellRendererText());
    cols_count = m_TreeView.append_column("Address", *pRenderer);
    pColumn    = m_TreeView.get_column(cols_count-1);
    if (pColumn)
    {
        pColumn->add_attribute(pRenderer->property_text(), m_Columns.m_address);
        pRenderer->property_family() = "Monospace";
        pColumn->set_resizable(true);
    }
    
    
    pRenderer  = Gtk::manage(new Gtk::CellRendererText());
    cols_count = m_TreeView.append_column("Size", *pRenderer);
    pColumn    = m_TreeView.get_column(cols_count-1);
    if (pColumn)
    {
        pColumn->add_attribute(pRenderer->property_text(), m_Columns.m_size);
        pRenderer->property_family() = "Monospace";
        pColumn->set_resizable(true);
    }
    
    
    pRenderer  = Gtk::manage(new Gtk::CellRendererText());
    cols_count = m_TreeView.append_column("Description", *pRenderer);
    pColumn    = m_TreeView.get_column(cols_count-1);
    if (pColumn)
    {
        pColumn->add_attribute(pRenderer->property_text(), m_Columns.m_description);
        pRenderer->property_family() = "Monospace";
        pColumn->set_resizable(true);
    }
    
    show_all_children();
}



     Gui :: ~Gui           () {}
void Gui :: on_button_quit () { hide(); }

Glib::ustring Gui :: format_08x (unsigned int number) {
    char buf[16];
    snprintf(buf, 16, "%08x", number);
    return buf;
}

Glib::ustring Gui :: format_04x (unsigned int number) {
    char buf[16];
    snprintf(buf, 16, "%04x", number);
    return buf;
}

void Gui :: add_instruction (unsigned int address, int size, Glib::ustring description) {
    Gtk::TreeModel::Row row = *(m_refTreeModel->append(current_function.children()));
    row[m_Columns.m_address]     = format_08x(address);
    row[m_Columns.m_size]        = format_04x(size);
    row[m_Columns.m_description] = description;
}

void Gui :: add_function (unsigned int address, int size, Glib::ustring label) {
    current_function = *(m_refTreeModel->append(current_section.children()));
    current_function[m_Columns.m_address]     = format_08x(address);
    current_function[m_Columns.m_size]        = format_04x(size);
    current_function[m_Columns.m_description] = label;
}

void Gui :: add_section (unsigned int address, int size, Glib::ustring description) {
    current_section = *(m_refTreeModel->append());
    current_section[m_Columns.m_address]     = format_08x(address);
    current_section[m_Columns.m_size]        = format_04x(size);
    current_section[m_Columns.m_description] = description;
}
