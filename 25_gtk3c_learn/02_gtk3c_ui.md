Cu siguranță! Integrarea suportului UI prin fișiere XML (folosind **GtkBuilder** și Glade) este un aspect esențial pentru dezvoltarea modernă de aplicații GTK3 și trebuie inclus într-un curs avansat. Vom adăuga un modul dedicat acestui subiect și vom actualiza structura cursului.

---

### Structura Cursului GTK3 în C pentru Avansați (Actualizată)

**Titlu:** Dezvoltare Avansată de Aplicații Desktop cu GTK3 și C

**Public Țintă:** Programatori C cu experiență, familiarizați cu fundamentele GTK3 (creare de ferestre, butoane, semnale simple).

**Obiective:**
* Aprofundarea arhitecturii GTK3 și a librăriei GLib.
* Înțelegerea și utilizarea widget-urilor complexe și a containerelor avansate.
* Manipularea datelor și a model-view.
* **Proiectarea interfețelor utilizator cu GtkBuilder (XML).**
* Desenare personalizată cu Cairo.
* Internaționalizare și localizare.
* Integrare cu alte tehnologii (GStreamer, WebKitGTK).
* Optimizare și bune practici.
* Depanare și testare.

**Module:**

**Modulul 1: Recapitulare și Aprofundare GLib & GObject**
* **GLib Data Structures:** `GList`, `GSList`, `GHashTable`, `GQueue`, `GTree`, `GArray`, `GString`, `GError`.
* **GObject Fundamentals:** Arhitectura bazată pe obiecte, proprietăți, semnale, clase și instanțe.
* **Memorie Management:** Referințe, contează, `g_object_ref()`, `g_object_unref()`, `g_clear_object()`.
* **Threads & Concurrency:** `GMutex`, `GCond`, `GThreadPool`, `GAsyncQueue`, `GTask`.
* **GIO:** Sistemul de intrare/ieșire asincron.
* **GSettings:** Persistența setărilor aplicației.

**Modulul 2: Arhitectura și Widget-uri Avansate GTK3**
* **Model-View-Controller (MVC) în GTK3:** `GtkTreeModel`, `GtkTreeStore`, `GtkListStore`, `GtkTreeView`, `GtkCellRenderer`.
* **Containeri Complecși:** `GtkGrid`, `GtkStack`, `GtkStackSwitcher`, `GtkNotebook`, `GtkPaned`.
* **Custom Widgets:** Crearea de widget-uri proprii prin subclasarea `GtkWidget` sau alte widget-uri existente.
* **Dialoguri Avansate:** `GtkFileChooserDialog`, `GtkColorChooserDialog`, `GtkFontChooserDialog`.
* **GtkApplication & GtkApplicationWindow:** Structura modernă a aplicațiilor GTK.

---

### Noul Modul: Modulul 3: Proiectarea Interfețelor cu GtkBuilder și Glade

**Descriere:** Acest modul se concentrează pe separarea designului interfeței utilizator de logica aplicației, folosind fișiere XML (UI files) și instrumentul vizual Glade. Această abordare îmbunătățește mentenabilitatea, colaborarea și flexibilitatea dezvoltării UI.

**Subiecte:**
* **Introducere în GtkBuilder:**
    * Ce este GtkBuilder și de ce este important?
    * Avantajele separării UI de cod (XML vs. cod hardcodat).
    * Sintaxa de bază a fișierelor UI (XML).
* **Glade - Instrumentul de Design Vizual:**
    * Instalarea și utilizarea Glade pentru a crea interfețe GTK.
    * Adăugarea și configurarea widget-urilor în Glade.
    * Setarea proprietăților, ambalarea widget-urilor în containere.
    * Crearea de semnale și atașarea de *handleri* (funcții de apel invers) în Glade.
    * Exportarea interfeței ca fișier `.ui`.
* **Încărcarea UI din Fișiere XML în C:**
    * Utilizarea funcției `gtk_builder_new()` pentru a crea un obiect `GtkBuilder`.
    * Încărcarea fișierului `.ui` cu `gtk_builder_add_from_file()` sau `gtk_builder_add_from_resource()`.
    * Obținerea referințelor la widget-uri specifice folosind `gtk_builder_get_object()`.
    * Conectarea semnalelor (din Glade) la funcțiile de *callback* din codul C folosind `gtk_builder_connect_signals()`.
    * Tratarea erorilor la încărcarea fișierelor UI.
* **Manevrarea Widget-urilor Obținute:**
    * Convertirea tipurilor de obiecte (`GTK_WIDGET()`, `GTK_WINDOW()`, etc.).
    * Manipularea proprietăților și metodelor widget-urilor obținute.
* **GResource - Împachetarea Fișierelor UI:**
    * De ce să folosim GResource? (Embed fișierele UI direct în executabil).
    * Crearea fișierelor `.gresource.xml` și `.c`.
    * Compilarea și legarea resurselor la aplicație.
    * Încărcarea UI din resurse (`gtk_builder_add_from_resource()`).
* **Exemple Practic:**
    * Crearea unei ferestre simple cu butoane și *entry fields* folosind Glade și C.
    * Dezvoltarea unui dialog complex cu multiple widget-uri.
    * Integrarea unui `GtkTreeView` (sau alte widget-uri avansate) definite în Glade.

---

**Modulul 4: Desenare Personalizată cu Cairo**
* **Introducere în Cairo:** Contexturi, căi, transformări, culori, surse, operații.
* **Desenarea în GTK3:** Utilizarea `GtkDrawingArea` și a semnalului "draw".
* **Desenare Vectorială Complexă:** Grafice, diagramă, elemente vizuale personalizate.
* **Utilizarea Pixbuf-urilor:** Manipularea imaginilor bitmap.

**Modulul 5: Internaționalizare și Localizare (i18n & l10n)**
* **Gettext:** Utilizarea `gettext` pentru traducerea șirurilor de caractere.
* **Crearea și Gestionarea Fișierelor PO/MO:** Integrarea în sistemul de build.
* **Suport pentru Limbile RTL (Right-to-Left).**

**Modulul 6: Integrare cu Alte Tehnologii**
* **GStreamer:** Integrarea multimedia în aplicațiile GTK3 (redare audio/video).
* **WebKitGTK:** Încorporarea unui browser web în aplicația dvs.
* **D-Bus:** Comunicarea între procese.
* **Libhandy (pentru aplicații responsive/adaptive):** Opțional, pentru aplicații care țintesc și dispozitive mobile/factori de formă diferiți.

**Modulul 7: Optimizare, Depanare și Bune Practici**
* **Profilarea Performanței:** `perf`, `valgrind`, `gprof`.
* **Gestionarea Resurselor:** Utilizarea `GResource` pentru a împacheta resurse (imagini, UI files).
* **Bune Practici de Cod:** Structură, lizibilitate, securitate.
* **Testare Unitară:** Folosind GLib Testing Framework.
* **Depanare:** GDB și instrumente specifice GTK.

**Modulul 8: Proiect Final**
* Dezvoltarea unei aplicații complexe care integrează majoritatea conceptelor învățate, cu accent pe utilizarea fișierelor UI.

---

### Exemplu de Integrare UI prin XML (Glade/GtkBuilder)

**1. Fișierul `ui_example.ui` (creat cu Glade):**

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk+" version="3.24"/>
  <object class="GtkWindow" id="main_window">
    <property name="can-focus">False</property>
    <property name="title" translatable="yes">Aplicație UI XML</property>
    <property name="default-width">300</property>
    <property name="default-height">200</property>
    <child>
      <object class="GtkBox">
        <property name="visible">True</property>
        <property name="can-focus">False</property>
        <property name="orientation">vertical</property>
        <property name="spacing">10</property>
        <child>
          <object class="GtkLabel">
            <property name="visible">True</property>
            <property name="can-focus">False</property>
            <property name="label" translatable="yes">Salut din Glade!</property>
            <property name="justify">center</property>
          </object>
          <packing>
            <property name="expand">False</property>
            <property name="fill">True</property>
            <property name="position">0</property>
          </packing>
        </child>
        <child>
          <object class="GtkButton" id="my_button">
            <property name="label" translatable="yes">Apasa-ma!</property>
            <property name="visible">True</property>
            <property name="can-focus">True</property>
            <property name="receives-default">True</property>
            <signal name="clicked" handler="on_my_button_clicked" swapped="no"/>
          </object>
          <packing>
            <property name="expand">False</property>
            <property name="fill">True</property>
            <property name="position">1</property>
          </packing>
        </child>
      </object>
    </child>
  </object>
</interface>
```

**Explicație Glade XML:**
* `interface`: Elementul rădăcină.
* `object class="GtkWindow" id="main_window"`: Definește o fereastră principală cu ID-ul `main_window`. Acest ID va fi folosit în codul C pentru a obține o referință la fereastră.
* `object class="GtkButton" id="my_button"`: Definește un buton cu ID-ul `my_button`.
* `signal name="clicked" handler="on_my_button_clicked" swapped="no"`: Aceasta este partea cheie. Specifică faptul că la semnalul "clicked" al butonului, funcția C numită `on_my_button_clicked` ar trebui apelată.

**2. Fișierul `main.c`:**

```c
#include <gtk/gtk.h>

// Declararea funcției de callback (handler-ul pentru semnalul "clicked")
static void on_my_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul a fost apasat!\n");
    // Aici puteti adauga logica aplicatiei, de exemplu, actualizarea unui label
    GtkLabel *label = GTK_LABEL(user_data); // Presupunem ca user_data este un GtkLabel
    if (label) {
        gtk_label_set_text(label, "Ai apasat butonul!");
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label; // Vom obtine referinta la label pentru a-l actualiza

    // 1. Creare GtkBuilder
    builder = gtk_builder_new();

    // 2. Încărcare UI din fișierul XML
    // Asigurati-va ca "ui_example.ui" este in aceeasi locatie cu executabilul
    // sau specificati calea completa.
    if (!gtk_builder_add_from_file(builder, "ui_example.ui", NULL)) {
        g_warning("Nu s-a putut încărca fișierul UI!");
        g_object_unref(builder);
        return;
    }

    // 3. Obținere referințe la widget-uri prin ID-urile lor
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    button = GTK_WIDGET(gtk_builder_get_object(builder, "my_button"));
    label = GTK_WIDGET(gtk_builder_get_object(builder, "my_label")); // Daca avem un label cu ID "my_label" in Glade

    // Conectarea ferestrei la aplicație (necesar pentru GtkApplication)
    gtk_application_add_window(app, GTK_WINDOW(window));

    // Exemplu de conectare manuala (daca nu a fost facuta in Glade)
    // g_signal_connect(button, "clicked", G_CALLBACK(on_my_button_clicked), NULL);

    // Conectarea semnalelor definite în Glade.
    // Argumentul user_data va fi pasat la handler-ul semnalului.
    // In acest caz, vom conecta butonul la functie si vom pasa label-ul ca user_data
    gtk_builder_connect_signals(builder, label); // Acest lucru va conecta TOATE semnalele specificate in UI la functii cu acelasi nume

    // 4. Afisare fereastra
    gtk_widget_show_all(window);

    // Eliberare referinta la GtkBuilder (după ce toate widget-urile au fost instanțiate)
    g_object_unref(builder);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example.GtkBuilderExample", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
```

**Compilare (cu `pkg-config`):**

```bash
gcc main.c -o my_app $(pkg-config --cflags --libs gtk+-3.0)
```

Asigură-te că fișierul `ui_example.ui` se află în același director cu executabilul sau specifică o cale completă.

**Explicația Codului C:**
1.  **`gtk_builder_new()`**: Creează un nou obiect `GtkBuilder`.
2.  **`gtk_builder_add_from_file(builder, "ui_example.ui", NULL)`**: Încarcă interfața utilizator definită în fișierul XML.
3.  **`gtk_builder_get_object(builder, "id_widget")`**: Se folosește pentru a obține o referință la un widget specific din arborele UI, folosind ID-ul definit în fișierul XML. Este esențial să convertiți tipul returnat la tipul corect de widget (e.g., `GTK_WIDGET()`, `GTK_BUTTON()`).
4.  **`gtk_builder_connect_signals(builder, label)`**: Aceasta este o funcție puternică. Parcurge toate semnalele definite în fișierul XML și le conectează automat la funcțiile C cu numele specificat ca *handler*. Argumentul `user_data` (în acest caz, referința la `label`) va fi pasat tuturor funcțiilor de *callback* conectate prin `gtk_builder_connect_signals`.
5.  **`g_object_unref(builder)`**: Eliberează referința la `GtkBuilder` după ce toate widget-urile au fost create și conectate. Widget-urile în sine au acum referințe la ele.

---

Prin adăugarea acestui modul, cursul va oferi o perspectivă completă asupra tehnicilor moderne de dezvoltare UI în GTK3, permițând dezvoltatorilor să creeze aplicații mai robuste și mai ușor de gestionat.

---

Ce alte aspecte crezi că ar fi utile de aprofundat în contextul unui curs avansat GTK3 în C?
