AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS =	-I$(top_srcdir) \
		$(GTK_CFLAGS) \
		$(GTKSRC_CFLAGS) \
		-DGTK_DISABLE_SINGLE_INCLUDES \
		-DGDK_DISABLE_DEPRECATED \
		-DGTK_DISABLE_DEPRECATED \
		-DGSEAL_ENABLE \
		-DPREFIX=\""${prefix}\"" \
		-DDATADIR=\""${datarootdir}"\" \
		-DSYSCONFDIR=\""${sysconfdir}/${PACKAGE}"\" \
		-DLIBDIR=\""${libdir}/${PACKAGE}"\" \
		-DLOCALEDIR=\""${localedir}"\"

LIBS =	$(GTK_LIBS) \
	$(GTKSRC_LIBS) \
	$(ASPELL_LIBS)

COMMONSRC = callbacks.cpp \
	files.cpp \
	globals.cpp \
	guis.cpp \
	main.cpp \
	script.cpp \
	searchcallbacks.cpp \
	spellcheck.cpp

bin_PROGRAMS = mpe-gtk2

mpe_gtk2_SOURCES = $(COMMONSRC)

appfilesdir = ${datarootdir}/applications
appfiles_DATA = ../resources/applications/*

pixfilesdir = ${datarootdir}/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

sizes = 256x256 128x128 64x64 48x48 32x32 22x22 16x16

install-data-hook:
	for size in ${sizes}; do \
		mkdir -p $(DESTDIR)${prefix}/share/icons/hicolor/"$$size"/apps||true; \
		convert ../resources/pixmaps/mpe-gtk2.png -resize "$$size" $(DESTDIR)${prefix}/share/icons/hicolor/"$$size"/apps/mpe-gtk2.png; \
	done

	mkdir -p $(DESTDIR)${prefix}/share/icons||true
	mkdir -p $(DESTDIR)${prefix}/share/icons/large||true
	mkdir -p $(DESTDIR)${prefix}/share/icons/mini||true
	convert ../resources/pixmaps/mpe-gtk2.png -resize 32x32 $(DESTDIR)${prefix}/share/icons/mpe-gtk2.png
	convert ../resources/pixmaps/mpe-gtk2.png -resize 48x48 $(DESTDIR)${prefix}/share/icons/large/mpe-gtk2.png
	convert ../resources/pixmaps/mpe-gtk2.png -resize 16x16 $(DESTDIR)${prefix}/share/icons/mini/mpe-gtk2.png
	
uninstall-hook:
	for size in ${sizes}; do \
		rm $(DESTDIR)${prefix}/share/icons/hicolor/"$$size"/apps/mpe-gtk2.png||true; \
	done

	rm $(DESTDIR)${prefix}/share/icons/mpe-gtk2.png||true
	rm $(DESTDIR)${prefix}/share/icons/large/mpe-gtk2.png||true
	rm $(DESTDIR)${prefix}/share/icons/mini/mpe-gtk2.png||true

	rm -r $(DESTDIR)${pkgdatadir}
