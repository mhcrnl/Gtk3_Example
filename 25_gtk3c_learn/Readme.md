Îmi cer scuze, dar nu pot crea un "curs complet" despre GTK3 în C într-un singur răspuns. Un curs complet ar însemna sute de pagini de conținut, exerciții, exemple detaliate, și ar acoperi o gamă largă de widget-uri, concepte și bune practici.

Cu toate acestea, pot să structurez o **schită detaliată a unui curs GTK3 în C** și să îți ofer **exemple fundamentale pentru fiecare secțiune majoră**, care să te ghideze în învățare. Acest lucru ar echivala cu primele capitole ale unui manual.

---

### **Schită Detaliată: Curs GTK3 în C**

**Introducere în GTK3 și C**

* **Ce este GTK3?**
    * Toolkit de widget-uri multi-platformă.
    * Folosit pentru aplicații grafice (GUI).
    * De ce C? Performanță, control, portabilitate.
* **Mediul de Dezvoltare**
    * Compilator GCC.
    * Biblioteci GTK3.
    * `pkg-config` pentru compilare.
    * Structura de bază a unui program GTK.
* **Compilare și Rulare**
    * Exemplu de comandă `gcc` cu `pkg-config`.

**Secțiunea 1: Fundamentele GTK3**

* **Inițializarea GTK:** `gtk_init()` sau `gtk_application_new()`.
* **Fereastra Principală (`GtkWindow`)**
    * Creare, titlu, dimensiuni, poziție.
    * Proprietăți comune.
* **Gestionarea Semnalelor (`GObject` și `G_CALLBACK`)**
    * Ce sunt semnalele? Mecanismul de evenimente.
    * Conectarea semnalelor la funcții de callback (`g_signal_connect`).
    * Semnalul `destroy` pentru închiderea ferestrei.
* **Bucla Principală a Aplicației (`gtk_main()` sau `g_application_run()`)**
    * Importanța acesteia.
    * Procesarea evenimentelor.
* **Widget-uri de Bază:**
    * `GtkLabel`: Afișare text.
    * `GtkButton`: Buton clickabil.
    * `GtkEntry`: Câmp de introducere text.
* **Ambalarea (Packing) Widget-urilor:**
    * `GtkBox` (`GtkHBox`, `GtkVBox`): Aranjare orizontală/verticală.
    * `gtk_box_pack_start`, `gtk_box_pack_end`.
    * `padding` și `expand`.

**Secțiunea 2: Organizarea Interfeței cu Layout-uri**

* **`GtkGrid`:** Aranjare pe bază de rânduri și coloane.
    * `gtk_grid_attach`.
    * Span-uri.
* **`GtkScrolledWindow`:** Adăugarea de bare de derulare.
* **`GtkStack` și `GtkStackSwitcher`:** Schimbarea dinamică a vizualizărilor.
* **`GtkNotebook`:** Tab-uri.
* **`GtkPaned`:** Fereastre redimensionabile.

**Secțiunea 3: Widget-uri de Intrare și Ieșire Avansate**

* **`GtkTextView`:** Editare text pe mai multe linii.
    * `GtkTextBuffer`.
* **`GtkComboBoxText` / `GtkComboBox`:** Liste derulante.
* **`GtkSpinButton`:** Selectare valori numerice.
* **`GtkScale`:** Slider-uri.
* **`GtkCheckButton` și `GtkRadioButton`:** Butoane de selecție.
* **`GtkProgressBar`:** Bare de progres.
* **`GtkImage`:** Afișare imagini.

**Secțiunea 4: Meniuri, Bare de Instrumente și Dialoguri**

* **`GtkMenuBar` și `GtkMenuItem`:** Meniuri tradiționale.
* **`GtkToolbar` și `GtkToolButton`:** Bare de instrumente.
* **`GtkDialog`:** Crearea de dialoguri personalizate.
* **Dialoguri Pre-definite:**
    * `GtkFileChooserDialog`: Deschidere/Salvare fișiere.
    * `GtkMessageDialog`: Mesaje de informare/avertizare/eroare.
    * `GtkAboutDialog`: Despre aplicație.

**Secțiunea 5: Concepte Avansate**

* **CSS în GTK:** Stiluri personalizate pentru widget-uri.
* **Resurse GResource:** Încapsularea fișierelor (imagini, UI, CSS) în executabil.
* **Acțiuni și Mape de Acțiuni (`GAction`, `GActionGroup`, `GtkApplicationWindow`)**
    * Separarea logicii de UI.
    * Scurtături de la tastatură.
* **Model-View-Controller (MVC) cu `GtkTreeView` și `GtkListStore`:**
    * Afișarea datelor tabelare.
    * Coloane, randări.
* **Thread-uri și Glib Main Loop:**
    * Cum să efectuezi operații lungi fără a bloca UI-ul.
    * `g_idle_add`, `g_timeout_add`.
* **Internaționalizare (i18n) și Localizare (l10n):** `gettext`.

**Secțiunea 6: Bune Practici și Depanare**

* **Gestionarea Memoriei:** `g_object_unref`, `g_free`.
* **Depanare cu GDB.**
* **Proiecte Mai Mari:** Structura de fișiere, Makefile-uri.

---

### **Exemple Fundamentale pentru un Curs GTK3 în C**

Voi oferi exemple pentru primele secțiuni, care sunt esențiale pentru a începe.

#### **Exemplu 0: Structura Minimală a unei Aplicații GTK3**

Acesta este "Hello World" pentru GTK3.

```c
#include <gtk/gtk.h>

// Funcția de callback care se apelează când aplicația este "activată"
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;

    // Creează o nouă fereastră GTK
    window = gtk_application_window_new(app);
    // Setează titlul ferestrei
    gtk_window_set_title(GTK_WINDOW(window), "Salut, GTK!");
    // Setează dimensiunea implicită a ferestrei
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Afișează toate widget-urile din fereastră (în acest caz, doar fereastra însăși)
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Creează o nouă aplicație GTK.
    // "org.gtk.example.Salut" este un ID unic pentru aplicație.
    app = gtk_application_new("org.gtk.example.Salut", G_APPLICATION_FLAGS_NONE);

    // Conectează semnalul "activate" (când aplicația este gata să afișeze o fereastră)
    // la funcția noastră de callback 'activate'.
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Rulează aplicația. Aceasta intră în bucla principală de evenimente.
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Eliberează resursele aplicației când bucla se încheie
    g_object_unref(app);

    return status;
}
```

**Compilare:**
```bash
gcc -o hello_gtk hello_gtk.c `pkg-config --cflags --libs gtk+-3.0`
```
**Rulare:**
```bash
./hello_gtk
```

#### **Exemplu 1: Adăugarea unui Buton și Manipularea Evenimentelor**

```c
#include <gtk/gtk.h>

// Funcție de callback pentru semnalul "clicked" al butonului
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Butonul a fost apăsat!\n");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box; // Vom folosi o cutie pentru a așeza widget-ul

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Buton Simplu");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Creează o cutie verticală pentru a așeza butonul
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // 0 padding între widget-uri
    gtk_container_add(GTK_CONTAINER(window), box);

    // Creează un buton cu textul "Apăsă-mă!"
    button = gtk_button_new_with_label("Apăsă-mă!");

    // Conectează semnalul "clicked" al butonului la funcția noastră 'on_button_clicked'
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Adaugă butonul în cutia verticală
    // gtk_box_pack_start: adaugă widget-ul la începutul cutiei
    // TRUE: widget-ul se extinde pentru a umple spațiul disponibil
    // TRUE: widget-ul poate umple mai mult spațiu decât dimensiunea sa minimă dorită
    // 0: spațiu suplimentar în jurul widget-ului
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example.ButonSimplu", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
```

**Compilare:**
```bash
gcc -o button_example button_example.c `pkg-config --cflags --libs gtk+-3.0`
```
**Rulare:**
```bash
./button_example
```

#### **Exemplu 2: Utilizarea GtkEntry și GtkLabel**

```c
#include <gtk/gtk.h>

// Funcție de callback pentru buton
static void on_hello_button_clicked(GtkWidget *widget, gpointer data) {
    // Castăm data la GtkEntry
    GtkEntry *entry = GTK_ENTRY(data);
    const char *text = gtk_entry_get_text(entry); // Obținem textul din GtkEntry

    // Afișăm textul în consolă
    g_print("Salut, %s!\n", text);

    // Putem, de asemenea, să actualizăm un GtkLabel, dacă avem unul:
    // GtkLabel *label = g_object_get_data(G_OBJECT(widget), "my-label-widget");
    // gtk_label_set_text(label, g_strdup_printf("Bună ziua, %s!", text));
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Interacțiune GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);

    // Creează o cutie verticală pentru a aranja widget-urile
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // 10 pixeli spațiu între widget-uri
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10); // Margine de 10 pixeli în jurul cutiei

    // Creează un GtkLabel
    label = gtk_label_new("Introduceți numele dumneavoastră:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0); // FALSE, FALSE pentru a nu se extinde

    // Creează un GtkEntry
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Nume..."); // Text sugestiv
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    // Creează un GtkButton
    button = gtk_button_new_with_label("Spune Salut!");
    // Conectează butonul la funcția de callback, trecând 'entry' ca date utilizator
    g_signal_connect(button, "clicked", G_CALLBACK(on_hello_button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example.InputExample", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
```

**Compilare:**
```bash
gcc -o input_example input_example.c `pkg-config --cflags --libs gtk+-3.0`
```
**Rulare:**
```bash
./input_example
```

#### **Exemplu 3: Utilizarea GtkGrid**

```c
#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button1, *button2, *button3, *button4;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GtkGrid Exemplu");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Creează un GtkGrid
    grid = gtk_grid_new();
    // Setează spațiul dintre celulele grilei
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    // Setează o margine în jurul grilei
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Adaugă grila la fereastră
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Creează butoane
    button1 = gtk_button_new_with_label("Buton 1");
    button2 = gtk_button_new_with_label("Buton 2");
    button3 = gtk_button_new_with_label("Buton 3");
    button4 = gtk_button_new_with_label("Buton 4 (span)");

    // Adaugă butoanele la grilă
    // gtk_grid_attach(grid, child, left, top, width, height)
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1); // Coloana 0, Rândul 0, 1x1 celule
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 0, 1, 1); // Coloana 1, Rândul 0, 1x1 celule
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 1, 1, 1); // Coloana 0, Rândul 1, 1x1 celule
    gtk_grid_attach(GTK_GRID(grid), button4, 1, 1, 2, 1); // Coloana 1, Rândul 1, se întinde pe 2 coloane

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example.GridExample", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
```

**Compilare:**
```bash
gcc -o grid_example grid_example.c `pkg-config --cflags --libs gtk+-3.0`
```
**Rulare:**
```bash
./grid_example
```

---

**Resurse Suplimentare Esențiale:**

1.  **Documentația Oficială GTK3:** Aceasta este cea mai importantă resursă. Caută funcțiile și widget-urile direct pe site-ul GNOME Developer: [https://developer.gnome.org/gtk3/stable/](https://developer.gnome.org/gtk3/stable/)
2.  **Tutoriale GTK3 (Online):**
    * **"GTK+ 3 Reference Manual" (tutorial section):** Deși este mai mult un manual, are exemple utile.
    * **zetcode.com (GTK3 C tutorial):** Un site bun cu o serie de tutoriale pas cu pas.
    * **The Linux Programming Interface (TLPI) - Capitolul despre GTK (dacă îl ai):** Este o resursă excelentă pentru programare Linux în general, inclusiv GTK.
3.  **Exemple de Cod Sursă:** Analizează codul sursă al aplicațiilor GTK3 open-source.

**Sfaturi pentru Învățare:**

* **Practică, Practică, Practică:** Cel mai bun mod de a învăța este să scrii cod. Încearcă să modifici exemplele, să adaugi noi funcționalități.
* **Citește Documentația:** Când întâlnești un widget nou, primul pas este să citești documentația sa.
* **Înțelege Sistemul de Obiecte GObject:** GTK este construit pe GObject. Înțelegerea conceptelor de bază (obiecte, proprietăți, semnale) este crucială.
* **Depanare:** Folosește un depanator (cum ar fi GDB) pentru a înțelege ce se întâmplă în programul tău.
* **Începe Simplu:** Nu încerca să construiești o aplicație complexă de la început. Începe cu ferestre simple, adaugă widget-uri unul câte unul.

Sper că această schiță și exemplele fundamentale îți oferă un bun punct de plecare pentru a învăța GTK3 în C! Succes!
