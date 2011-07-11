#ifndef gui_HEADER
#define gui_HEADER

#include <gtkmm.h>

class Gui : public Gtk::Window
{
    protected :
        void on_button_quit ();
        
        class ModelColumns : public Gtk::TreeModel::ColumnRecord {
            public :
                ModelColumns () {
                    add(m_address);
                    add(m_size);
                    add(m_description);
                }
                    
                Gtk::TreeModelColumn <Glib::ustring> m_address;
                Gtk::TreeModelColumn <Glib::ustring> m_size;
                Gtk::TreeModelColumn <Glib::ustring> m_description;
        };
        
        ModelColumns m_Columns;
        
        Gtk::VBox m_VBox;
        
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::TreeView m_TreeView;
        Glib::RefPtr <Gtk::TreeStore> m_refTreeModel;
        
        Gtk::TreeModel::Row current_function;
        Gtk::TreeModel::Row current_section;
        
        Glib::ustring format_08x (unsigned int);
        Glib::ustring format_04x (unsigned int);

    public :
        Gui();
        virtual ~Gui();
        
        void add_instruction (unsigned int address, int size, Glib::ustring description);
        void add_section     (unsigned int address, int size, Glib::ustring label);
        void add_function    (unsigned int address, int size, Glib::ustring label);
        
};

#endif
