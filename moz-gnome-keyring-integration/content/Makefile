CFLAGS+=`pkg-config --cflags "gnome-keyring-1"`  `pkg-config --cflags "glib-2.0"`
LDFLAGS += -shared `pkg-config --libs "gnome-keyring-1"`  `pkg-config --libs "glib-2.0"`

libgnome-keyring-helper.so.1.0.1: gnome-keyring.o
	$(CC) -Wall -Wl,-soname,libgnome-keyring-helper.so.1 $< -o $@ $(LDFLAGS)

gnome-keyring.o: gnome-keyring.c
	$(CC) -Wall -c $< -o $@ $(CFLAGS) -fPIC



