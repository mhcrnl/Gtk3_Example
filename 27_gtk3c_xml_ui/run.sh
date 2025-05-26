gcc -o my_gtk_app main.c -Wall $(pkg-config --cflags --libs gtk+-3.0)
./my_gtk_app
