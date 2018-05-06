# Mentor Joe's Robotics Lite
# Jonah Bockbrader <j@pelirrojo.ninja>
# The Ides of December, 2017
#
# TODO:
#  + Make a Makefile generator in a ./configure script

INSTALL=install
DESTDIR=/
INSTALL_PATH=/opt/roboticslite
PACKAGE_TMP=./roboticslite-package
MKDIRP=mkdir -p

EXTRACONF=-DRL_ACE_ALLOWREBOOT
# Potential options (and why they're disabled)
# `-DRL_ACE_ALLOWREBOOT`: in Arcade Cabinet Edition, let there be a reboot command
#    It's disabled because it's implemented by way of `system("sudo reboot");`.
#    The only reason I put it in was just for a convenice to reboot the game
#    without having to exit manually and start a reboot. Only works on UNIXes.

CC=g++
CFLAGS=-Wall -g -c -std=c++11
SOURCES=rr-main.cpp rr-input.cpp rr-entity.cpp rr-level.cpp rr-level-powerup.cpp rr-team.cpp rr-hud.cpp rr-menu.cpp rr-player.cpp rr-ttf.cpp rr-renderable.cpp rr-hscorefile.cpp rr-slideshowpane.cpp rl-confdir.cpp
ifeq ($(OS),Windows_NT) # Windows (MinGW)
  LDFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
  OUTFILE=roboticslite.exe
  RM=del
else # Linux/UNIX
  LDFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf
  OUTFILE=roboticslite
  RM=rm -f
endif

OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS_DEBUG=$(SOURCES:.cpp=.debug.o)
OBJECTS_ACE=$(SOURCES:.cpp=.ace.o)

.PHONY: clean build

build: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(OUTFILE)

debug: $(OBJECTS_DEBUG)
	$(CC) $(OBJECTS_DEBUG) $(LDFLAGS) -o $(OUTFILE)-debug

ace: $(OBJECTS_ACE)
	$(CC) $(OBJECTS_ACE) $(LDFLAGS) -o $(OUTFILE)-ace

install: build
	$(MKDIRP) $(DESTDIR)/$(INSTALL_PATH)/{imgs,ttf}
	$(INSTALL) -m 555 $(OUTFILE) $(DESTDIR)/$(INSTALL_PATH)/
	$(INSTALL) -m 444 ttf/*.ttf $(DESTDIR)/$(INSTALL_PATH)/ttf
	$(INSTALL) -m 444 imgs/*.png $(DESTDIR)/$(INSTALL_PATH)/imgs
	test -d $(DESTDIR)/usr/local/bin || $(MKDIRP) $(DESTDIR)/usr/local/bin
	test -d $(DESTDIR)/usr/local/share/applications || $(MKDIRP) $(DESTDIR)/usr/local/share/applications
	$(INSTALL) -m 555 etc/launch.sh $(DESTDIR)/usr/local/bin/roboticslite
	$(INSTALL) -m 444 etc/roboticslite.desktop $(DESTDIR)/usr/local/share/applications/roboticslite.desktop

%.debug.o: %.cpp
	$(CC) -DDEBUG $(CFLAGS) $(EXTRACONF) $< -o $@

%.ace.o: %.cpp
	$(CC) -DRR_ACE $(CFLAGS) $(EXTRACONF) $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o

# Create a heirarchy to package the game
hier: build
	$(MKDIRP) $(PACKAGE_TMP)/$(INSTALL_PATH)/
	$(MKDIRP) $(PACKAGE_TMP)/$(INSTALL_PATH)/ttf
	$(MKDIRP) $(PACKAGE_TMP)/$(INSTALL_PATH)/imgs
	$(MKDIRP) $(PACKAGE_TMP)/usr/local/bin
	$(MKDIRP) $(PACKAGE_TMP)/usr/local/share/applications/
	$(INSTALL) -m 555 $(OUTFILE) $(PACKAGE_TMP)/$(INSTALL_PATH)/
	$(INSTALL) -m 444 ttf/*.ttf $(PACKAGE_TMP)/$(INSTALL_PATH)/ttf
	$(INSTALL) -m 444 imgs/*.png $(PACKAGE_TMP)/$(INSTALL_PATH)/imgs
	$(INSTALL) -m 555 etc/launch.sh $(PACKAGE_TMP)/usr/local/bin/roboticslite
	$(INSTALL) -m 444 etc/roboticslite.desktop $(PACKAGE_TMP)/usr/local/share/applications/roboticslite.desktop

deb: hier
	test -x `which dpkg-deb` || (echo "Debian/Ubuntu thing. Try again." && return 1)
	mkdir -p $(PACKAGE_TMP)/DEBIAN
	$(INSTALL) -m 444 etc/control.DEBIAN $(PACKAGE_TMP)/DEBIAN/control
	touch $(PACKAGE_TMP)/DEBIAN/postinst
	chmod 555 $(PACKAGE_TMP)/DEBIAN/postinst
	dpkg-deb -b $(PACKAGE_TMP)

archlinux: build
	test -x /usr/bin/makepkg || (echo -e "\033[1mThis requires makepkg installed, try running on Arch Linux\033[0m" && return 1)
	/usr/bin/makepkg

