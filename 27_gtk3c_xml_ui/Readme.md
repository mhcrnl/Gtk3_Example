---
## Aplicație GTK3 simplă cu interfață în XML și buton "Close"

Iată un exemplu de aplicație GTK3 pentru începători, care folosește un fișier XML pentru a defini interfața grafică și include un `GtkGrid` cu un buton "Close" pentru a închide aplicația.

### 1. Structura proiectului

Vom avea două fișiere:

* `main.c`: Codul sursă C pentru aplicația GTK.
* `ui.xml`: Fișierul XML care descrie interfața grafică.

### 2. Fișierul `ui.xml`

Creați un fișier numit `ui.xml` cu următorul conținut:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <object class="GtkWindow" id="main_window">
    <property name="title">Aplicația Mea GTK3</property>
    <property name="default-width">300</property>
    <property name="default-height">200</property>
    <child>
      <object class="GtkGrid" id="main_grid">
        <property name="visible">true</property>
        <property name="row-spacing">10</property>
        <property name="column-spacing">10</property>
        <child>
          <object class="GtkButton" id="close_button">
            <property name="label">Închide Aplicația</property>
            <property name="visible">true</property>
            <property name="halign">center</property>
            <property name="valign">center</property>
          </object>
          <layout>
            <property name="column">0</property>
            <property name="row">0</property>
            <property name="column-span">1</property>
            <property name="row-span">1</property>
          </layout>
        </child>
      </object>
    </child>
  </object>
</interface>

```

**Explicații `ui.xml`:**

* `<interface>`: Elementul rădăcină pentru definirea interfeței.
* `<object class="GtkWindow" id="main_window">`: Definește fereastra principală a aplicației cu ID-ul `main_window`.
    * `<property name="title">`: Setează titlul ferestrei.
    * `<property name="default-width">` și `<property name="default-height">`: Setează dimensiunile implicite ale ferestrei.
* `<child>`: Conținutul ferestrei.
* `<object class="GtkGrid" id="main_grid">`: Definește un `GtkGrid` cu ID-ul `main_grid`. `GtkGrid` este un container care aranjează widget-urile în rânduri și coloane.
    * `<property name="visible">true</property>`: Face grila vizibilă.
    * `<property name="row-spacing">` și `<property name="column-spacing">`: Adaugă spațiu între rânduri și coloane.
* `<child>` (în interiorul `GtkGrid`): Conținutul grilei.
* `<object class="GtkButton" id="close_button">`: Definește un buton cu ID-ul `close_button`.
    * `<property name="label">`: Setează textul afișat pe buton.
    * `<property name="halign">center</property>` și `<property name="valign">center</property>`: Centrează butonul orizontal și vertical în celula grilei.
* `<layout>`: Specifică poziția butonului în `GtkGrid`.
    * `<property name="column">0</property>` și `<property name="row">0</property>`: Plasează butonul în prima coloană și primul rând (indexarea începe de la 0).
    * `<property name="column-span">1</property>` și `<property name="row-span">1</property>`: Specifică faptul că butonul ocupă o singură coloană și un singur rând.

### 3. Fișierul `main.c`

Creați un fișier numit `main.c` cu următorul conținut:

```c
#include <gtk/gtk.h>

// Funcția de callback care se apelează la apăsarea butonului "Închide"
static void on_close_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul 'Închide Aplicația' a fost apăsat.\n");
    GtkWindow *window = GTK_WINDOW(user_data); // Obținem referința la fereastră
    gtk_window_close(window); // Închidem fereastra
}

// Funcția de activare a aplicației
static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;

    // Creăm un GtkBuilder nou
    builder = gtk_builder_new();

    // Încărcăm fișierul XML cu interfața grafică
    if (gtk_builder_add_from_file(builder, "ui.xml", NULL) == 0) {
        g_warning("Nu s-a putut încărca fișierul ui.xml. Asigurați-vă că este în același director cu executabilul.\n");
        g_object_unref(builder);
        return;
    }

    // Obținem referința la fereastra principală după ID-ul său
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (window == NULL) {
        g_warning("Nu s-a putut găsi fereastra cu ID-ul 'main_window' în ui.xml.\n");
        g_object_unref(builder);
        return;
    }

    // Obținem referința la butonul "Închide Aplicația" după ID-ul său
    button = GTK_WIDGET(gtk_builder_get_object(builder, "close_button"));
    if (button == NULL) {
        g_warning("Nu s-a putut găsi butonul cu ID-ul 'close_button' în ui.xml.\n");
        g_object_unref(builder);
        return;
    }

    // Conectăm semnalul "clicked" al butonului la funcția noastră de callback
    // Transmitem fereastra ca user_data pentru a o putea închide
    g_signal_connect(button, "clicked", G_CALLBACK(on_close_button_clicked), window);

    // Adăugăm fereastra la aplicație (pentru a o gestiona corect de GTK)
    gtk_application_add_window(app, GTK_WINDOW(window));

    // Arătăm toate widget-urile din fereastră (inclusiv cele din grilă)
    gtk_widget_show_all(window);

    // Eliberăm GtkBuilder-ul (nu mai avem nevoie de el după ce am construit interfața)
    g_object_unref(builder);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Creăm o nouă aplicație GTK
    app = gtk_application_new("org.gtk.aplicatie-simpla", G_APPLICATION_FLAGS_NONE);

    // Conectăm semnalul "activate" la funcția noastră activate
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Rulăm aplicația
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Eliberăm aplicația
    g_object_unref(app);

    return status;
}

```

**Explicații `main.c`:**

* `#include <gtk/gtk.h>`: Include biblioteca GTK.
* `on_close_button_clicked`: Aceasta este o **funcție de callback**. Când butonul "Închide Aplicația" este apăsat, GTK va apela automat această funcție.
    * `GtkButton *button`: Referința la butonul care a declanșat evenimentul.
    * `gpointer user_data`: Date suplimentare pe care le-am transmis la conectarea semnalului. În cazul nostru, este referința la fereastră.
    * `gtk_window_close(window);`: Această funcție închide fereastra GTK.
* `activate`: Această funcție este apelată când aplicația este "activată" (adică, când este gata să afișeze o fereastră).
    * `gtk_builder_new()`: Creează un nou obiect `GtkBuilder`, care este folosit pentru a încărca fișierele XML.
    * `gtk_builder_add_from_file(builder, "ui.xml", NULL)`: Încearcă să încarce interfața grafică din fișierul `ui.xml`. Dacă fișierul nu este găsit sau este invalid, va afișa o avertizare.
    * `gtk_builder_get_object(builder, "main_window")` și `gtk_builder_get_object(builder, "close_button")`: Aceste funcții sunt esențiale. Ele ne permit să obținem o referință (un "pointer") la widget-urile definite în fișierul XML, folosind ID-urile lor (`main_window` și `close_button`).
    * `g_signal_connect(button, "clicked", G_CALLBACK(on_close_button_clicked), window);`: Aceasta este linia cheie pentru a face butonul funcțional.
        * `button`: Widget-ul la care conectăm semnalul.
        * `"clicked"`: Numele semnalului pe care dorim să-l ascultăm (în cazul butoanelor, acesta este semnalul de apăsare).
        * `G_CALLBACK(on_close_button_clicked)`: Funcția de callback care va fi apelată.
        * `window`: Datele suplimentare (`user_data`) pe care le transmitem funcției de callback. În acest caz, transmitem fereastra principală pentru a o putea închide.
    * `gtk_application_add_window(app, GTK_WINDOW(window));`: Adaugă fereastra la aplicație.
    * `gtk_widget_show_all(window);`: Face vizibile toate widget-urile din fereastră, inclusiv grila și butonul.
    * `g_object_unref(builder);`: Eliberează memoria alocată pentru `GtkBuilder` după ce nu mai avem nevoie de el.
* `main`: Funcția principală a programului.
    * `gtk_application_new("org.gtk.aplicatie-simpla", G_APPLICATION_FLAGS_NONE)`: Creează o nouă aplicație GTK. Argumentul este un ID unic pentru aplicație.
    * `g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);`: Conectează semnalul "activate" al aplicației la funcția noastră `activate`. Aceasta înseamnă că, atunci când aplicația pornește, funcția `activate` va fi apelată.
    * `g_application_run(G_APPLICATION(app), argc, argv);`: Pornește bucla principală de evenimente a GTK. Această funcție blochează execuția până când aplicația se închide.
    * `g_object_unref(app);`: Eliberează memoria alocată pentru aplicație.

### 4. Compilare și Rulare

Pentru a compila aplicația, aveți nevoie de un compilator C (cum ar fi GCC) și de pachetele de dezvoltare GTK3 instalate pe sistemul dumneavoastră.

**Pe sisteme bazate pe Debian/Ubuntu:**

```bash
sudo apt update
sudo apt install libgtk-3-dev
```

**Pe sisteme bazate pe Fedora/CentOS:**

```bash
sudo dnf install gtk3-devel
```

După instalarea dependențelor, deschideți un terminal în directorul unde ați salvat `main.c` și `ui.xml` și rulați următoarele comenzi:

```bash
gcc -o my_gtk_app main.c -Wall $(pkg-config --cflags --libs gtk+-3.0)
./my_gtk_app
```

* `gcc`: Compilatorul C.
* `-o my_gtk_app`: Specifică numele fișierului executabil rezultat (`my_gtk_app`).
* `main.c`: Fișierul sursă C.
* `-Wall`: Activează toate avertismentele compilatorului (recomandat pentru depanare).
* `$(pkg-config --cflags --libs gtk+-3.0)`: Aceasta este o comandă importantă. `pkg-config` este un utilitar care ajută la găsirea și configurarea librăriilor. Această parte a comenzii îi spune compilatorului unde să găsească fișierele header GTK (`--cflags`) și librăriile necesare pentru a face legătura (`--libs`).

După compilare, veți obține un fișier executabil numit `my_gtk_app`. Rulați-l cu `./my_gtk_app`.

Ar trebui să vedeți o fereastră simplă cu un buton "Închide Aplicația" în centru. Apăsați butonul pentru a închide aplicația.
