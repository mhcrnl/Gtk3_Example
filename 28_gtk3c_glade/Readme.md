Aplicația pe care o vom crea va folosi **GTK+ 3** și **Glade** pentru a construi o interfață grafică simplă, cu un buton "Close" care va închide aplicația.

---

### Pasul 1: Proiectarea Interfeței cu Glade

1.  **Deschide Glade:** Lansează aplicația Glade.

2.  **Creează o Fereastră (Window):**
    * În panoul din stânga ("Widgets"), trage și plasează un **"Window"** pe suprafața de lucru.
    * Selectează fereastra (dacă nu e deja selectată). În panoul din dreapta ("Properties"), la secțiunea "General", schimbă **"ID"**-ul în `main_window`.
    * De asemenea, la "Window", setează **"Title"**-ul la "Aplicația Mea GTK".

3.  **Adaugă un Grid (GtkGrid):**
    * Din panoul "Widgets", găsește **"Grid"** (este sub "Containers") și trage-l în interiorul ferestrei `main_window`.
    * Selectează Grid-ul și, în "Properties", la secțiunea "Grid", poți seta `Row Spacing` și `Column Spacing` la o valoare mică, de exemplu, `5`.

4.  **Adaugă un Buton (Button):**
    * Din panoul "Widgets", găsește **"Button"** și trage-l în interiorul **Grid**-ului.
    * Selectează butonul și, în "Properties", la secțiunea "General", schimbă **"ID"**-ul în `close_button`.
    * La secțiunea "Button", setează **"Label"**-ul la "Close".

5.  **Configurează Semnalul pentru Buton:**
    * Cu `close_button` selectat, mergi la tab-ul "Signals" din panoul "Properties" (lângă "General" și "Common").
    * Derulează până găsești semnalul `clicked`.
    * În coloana "Handler", tastează `on_close_button_clicked`. Aceasta va fi numele funcției C pe care o vom asocia cu evenimentul de clic pe buton.

6.  **Salvează Proiectul Glade:**
    * Mergi la `File` -> `Save As...`.
    * Salvează fișierul ca `my_app.glade` (sau orice alt nume, dar asigură-te că reții numele).

---

### Pasul 2: Crearea Codului C

Acum vom scrie codul C care va încărca interfața din fișierul `.glade` și va gestiona evenimentul de clic pe buton.

Creează un fișier numit `main.c` și adaugă următorul cod:

```c
#include <gtk/gtk.h>

// Funcția de callback pentru butonul "Close"
void on_close_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Butonul 'Close' a fost apăsat.\n");
    GtkWindow *window = GTK_WINDOW(user_data); // Obținem fereastra principală
    gtk_widget_destroy(GTK_WIDGET(window)); // Închidem fereastra
}

// Funcția de activare a aplicației
void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;

    // Creează un GtkBuilder nou
    builder = gtk_builder_new();

    // Încarcă interfața din fișierul Glade
    // Asigură-te că fișierul my_app.glade este în același director cu executabilul
    if (!gtk_builder_add_from_file(builder, "my_app.glade", NULL)) {
        g_warning("Nu am putut încărca fișierul Glade: my_app.glade");
        return;
    }

    // Obține fereastra principală după ID-ul setat în Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Conectează semnalele din Glade la funcțiile din codul C
    gtk_builder_connect_signals(builder, window); // Pasăm fereastra ca user_data pentru callback

    // Afișează fereastra
    gtk_widget_show_all(window);

    // Eliberează GtkBuilder-ul după ce nu mai este necesar
    g_object_unref(builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    // Creează o nouă aplicație GTK
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

    // Conectează semnalul "activate" la funcția noastră de activare
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Rulează aplicația
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Eliberează aplicația
    g_object_unref(app);

    return status;
}

```

---

### Pasul 3: Compilarea și Rularea Aplicației

Pentru a compila aplicația GTK, ai nevoie de **GCC** și de pachetele de dezvoltare GTK. Pe majoritatea sistemelor Linux bazate pe Debian/Ubuntu, le poți instala cu:

```bash
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev
```

După ce ai instalat dependențele, deschide un terminal în directorul unde ai salvat `main.c` și `my_app.glade` și compilează aplicația folosind `pkg-config` pentru a obține flag-urile necesare GTK:

```bash
gcc `pkg-config --cflags gtk+-3.0` -o my_app main.c `pkg-config --libs gtk+-3.0`
```

Dacă compilarea este reușită, vei avea un fișier executabil numit `my_app`. Rulează-l cu:

```bash
./my_app
```

Ar trebui să vezi o fereastră simplă cu titlul "Aplicația Mea GTK" și un buton "Close". Când apeși butonul "Close", fereastra se va închide, iar în terminal vei vedea mesajul "Butonul 'Close' a fost apăsat."

---

### Explicații pentru Începători:

* **`#include <gtk/gtk.h>`**: Această linie include biblioteca GTK+ în proiectul tău C.
* **`GtkBuilder`**: Este un obiect GTK care te ajută să încarci interfețe grafice create cu Glade. Practic, citește fișierul `.glade` și construiește obiectele GTK corespunzătoare în memorie.
* **`gtk_builder_new()`**: Creează un nou obiect `GtkBuilder`.
* **`gtk_builder_add_from_file(builder, "my_app.glade", NULL)`**: Această funcție încarcă designul interfeței tale din fișierul `my_app.glade` în `builder`.
* **`gtk_builder_get_object(builder, "main_window")`**: Aceasta caută un obiect în designul încărcat care are ID-ul `"main_window"` (setat în Glade) și returnează o referință la el. Noi o convertim la `GtkWidget` (tipul generic pentru orice element de interfață) și apoi la `GtkWindow`.
* **`gtk_builder_connect_signals(builder, window)`**: Această funcție este esențială! Ea scanează fișierul Glade și conectează automat funcțiile de callback (cum ar fi `on_close_button_clicked`) pe care le-ai specificat în Glade la funcțiile corespunzătoare din codul tău C. Parametrul `window` (care este `user_data` în `on_close_button_clicked`) este trimis ca argument către funcțiile de callback, permițându-ți să interacționezi cu fereastra din interiorul funcției.
* **`gtk_widget_show_all(window)`**: Această funcție face vizibilă fereastra și toate widget-urile conținute de ea.
* **`g_signal_connect(app, "activate", G_CALLBACK(activate), NULL)`**: Conectează semnalul "activate" al aplicației la funcția noastră `activate`. Semnalul "activate" este emis când aplicația este lansată.
* **`G_CALLBACK()`**: Este o macro de conversie a tipului, necesară pentru a spune compilatorului că funcția noastră C este o funcție de callback GTK.
* **`gtk_application_new()`**: Creează o nouă aplicație GTK. Domeniul (`"org.gtk.example"`) este folosit pentru identificarea unică a aplicației.
* **`g_application_run()`**: Pornește bucla principală a aplicației GTK, care ascultă evenimente (clicuri, mișcări de mouse, etc.) și le trimite către widget-urile corespunzătoare. Aplicația rămâne deschisă până când bucla principală este oprită (de exemplu, prin închiderea ferestrei).
* **`gtk_widget_destroy(GTK_WIDGET(window))`**: Această funcție este folosită pentru a închide o fereastră sau a distruge un widget.

Acest exemplu simplu ar trebui să te ajute să înțelegi cum să folosești Glade cu GTK+ 3 în C pentru a crea aplicații cu interfață grafică. Poți extinde cu ușurință această bază adăugând mai multe widget-uri, semnale și funcționalități.
