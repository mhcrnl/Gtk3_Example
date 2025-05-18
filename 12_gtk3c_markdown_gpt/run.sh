gcc -o markdown_preview markdown_preview.c `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0 libcmark`
./markdown_preview
