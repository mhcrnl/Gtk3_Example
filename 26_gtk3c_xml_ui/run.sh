gcc main.c -o my_app $(pkg-config --cflags --libs gtk+-3.0)
./my_app
