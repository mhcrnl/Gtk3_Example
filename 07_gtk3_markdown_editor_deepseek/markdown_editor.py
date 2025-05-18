import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gio, GLib

class MarkdownEditor(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="Editor Markdown")
        self.set_default_size(800, 600)
        
        # Variabile pentru stocarea fișierului curent
        self.current_file = None
        
        # Crearea interfeței
        self.create_ui()
        
    def create_ui(self):
        # Layout principal
        self.box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        self.add(self.box)
        
        # Bară de instrumente
        self.create_toolbar()
        
        # Panou de editare (editor și previzualizare)
        self.paned = Gtk.Paned(orientation=Gtk.Orientation.HORIZONTAL)
        self.box.pack_start(self.paned, True, True, 0)
        
        # Editor Markdown
        scrolled_editor = Gtk.ScrolledWindow()
        self.text_buffer = Gtk.TextBuffer()
        self.text_view = Gtk.TextView(buffer=self.text_buffer)
        self.text_view.set_wrap_mode(Gtk.WrapMode.WORD)
        scrolled_editor.add(self.text_view)
        self.paned.add1(scrolled_editor)
        
        # Previzualizare Markdown
        scrolled_preview = Gtk.ScrolledWindow()
        self.preview_label = Gtk.Label()
        self.preview_label.set_line_wrap(True)
        self.preview_label.set_selectable(True)
        scrolled_preview.add(self.preview_label)
        self.paned.add2(scrolled_preview)
        
        # Conectare la modificări pentru actualizare previzualizare
        self.text_buffer.connect("changed", self.update_preview)
        
        # Setare dimensiuni panouri
        self.paned.set_position(400)
        
    def create_toolbar(self):
        toolbar = Gtk.Toolbar()
        self.box.pack_start(toolbar, False, False, 0)
        
        # Buton Deschidere
        open_btn = Gtk.ToolButton.new_from_stock(Gtk.STOCK_OPEN)
        open_btn.connect("clicked", self.on_open_clicked)
        toolbar.insert(open_btn, 0)
        
        # Buton Salvare
        self.save_btn = Gtk.ToolButton.new_from_stock(Gtk.STOCK_SAVE)
        self.save_btn.connect("clicked", self.on_save_clicked)
        toolbar.insert(self.save_btn, 1)
        
        # Buton Salvare ca
        save_as_btn = Gtk.ToolButton.new_from_stock(Gtk.STOCK_SAVE_AS)
        save_as_btn.connect("clicked", self.on_save_as_clicked)
        toolbar.insert(save_as_btn, 2)
        
        # Separator
        toolbar.insert(Gtk.SeparatorToolItem(), 3)
        
        # Buton Ieșire
        exit_btn = Gtk.ToolButton.new_from_stock(Gtk.STOCK_QUIT)
        exit_btn.connect("clicked", self.on_quit_clicked)
        toolbar.insert(exit_btn, 4)
    
    def update_preview(self, buffer):
        start_iter = buffer.get_start_iter()
        end_iter = buffer.get_end_iter()
        text = buffer.get_text(start_iter, end_iter, False)
        
        # Aici ai putea adăuga un renderer Markdown mai avansat
        # Momentan afișăm textul simplu
        self.preview_label.set_text(text)
    
    def on_open_clicked(self, widget):
        dialog = Gtk.FileChooserDialog(
            title="Deschide fișier Markdown",
            parent=self,
            action=Gtk.FileChooserAction.OPEN,
            buttons=(
                Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
                Gtk.STOCK_OPEN, Gtk.ResponseType.OK
            )
        )
        
        filter_md = Gtk.FileFilter()
        filter_md.set_name("Fișiere Markdown")
        filter_md.add_pattern("*.md")
        filter_md.add_pattern("*.markdown")
        dialog.add_filter(filter_md)
        
        filter_any = Gtk.FileFilter()
        filter_any.set_name("Toate fișierele")
        filter_any.add_pattern("*")
        dialog.add_filter(filter_any)
        
        response = dialog.run()
        
        if response == Gtk.ResponseType.OK:
            self.current_file = dialog.get_filename()
            try:
                with open(self.current_file, 'r') as f:
                    content = f.read()
                    self.text_buffer.set_text(content)
                    self.set_title(f"Editor Markdown - {self.current_file}")
            except Exception as e:
                self.show_error_dialog(f"Eroare la deschidere: {str(e)}")
        
        dialog.destroy()
    
    def on_save_clicked(self, widget):
        if self.current_file:
            self.save_to_file(self.current_file)
        else:
            self.on_save_as_clicked(widget)
    
    def on_save_as_clicked(self, widget):
        dialog = Gtk.FileChooserDialog(
            title="Salvează fișier Markdown",
            parent=self,
            action=Gtk.FileChooserAction.SAVE,
            buttons=(
                Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
                Gtk.STOCK_SAVE, Gtk.ResponseType.OK
            )
        )
        
        filter_md = Gtk.FileFilter()
        filter_md.set_name("Fișiere Markdown")
        filter_md.add_pattern("*.md")
        filter_md.add_pattern("*.markdown")
        dialog.add_filter(filter_md)
        
        filter_any = Gtk.FileFilter()
        filter_any.set_name("Toate fișierele")
        filter_any.add_pattern("*")
        dialog.add_filter(filter_any)
        
        response = dialog.run()
        
        if response == Gtk.ResponseType.OK:
            filename = dialog.get_filename()
            if not filename.endswith(('.md', '.markdown')):
                filename += '.md'
            self.current_file = filename
            self.save_to_file(filename)
            self.set_title(f"Editor Markdown - {self.current_file}")
        
        dialog.destroy()
    
    def save_to_file(self, filename):
        start_iter = self.text_buffer.get_start_iter()
        end_iter = self.text_buffer.get_end_iter()
        text = self.text_buffer.get_text(start_iter, end_iter, False)
        
        try:
            with open(filename, 'w') as f:
                f.write(text)
        except Exception as e:
            self.show_error_dialog(f"Eroare la salvare: {str(e)}")
    
    def show_error_dialog(self, message):
        dialog = Gtk.MessageDialog(
            parent=self,
            flags=0,
            message_type=Gtk.MessageType.ERROR,
            buttons=Gtk.ButtonsType.OK,
            text="Eroare"
        )
        dialog.format_secondary_text(message)
        dialog.run()
        dialog.destroy()
    
    def on_quit_clicked(self, widget):
        Gtk.main_quit()

if __name__ == "__main__":
    app = MarkdownEditor()
    app.connect("destroy", Gtk.main_quit)
    app.show_all()
    Gtk.main()
