AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS =	-I$(top_srcdir) \
		$(GTK_CFLAGS) \
		$(GTKSRC_CFLAGS) \
		-DGTK_DISABLE_SINGLE_INCLUDES \
		-DGDK_DISABLE_DEPRECATED \
		-DGTK_DISABLE_DEPRECATED \
		-DGSEAL_ENABLE \
		-DPREFIX=\""${prefix}\"" \
		-DDATADIR=\""${pkgdatadir}"\" \
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

bin_PROGRAMS = manpageeditor

manpageeditor_SOURCES = $(COMMONSRC)

appfilesdir = ${datarootdir}/applications
appfiles_DATA = ../resources/applications/*

pixfilesdir = ${datarootdir}/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

man1_MANS = ../resources/man/manpageeditor.1

helpfilesdir = $(pkgdatadir)/help
helpfiles_DATA = ../resources/help/*

examplefilesdir = $(pkgdatadir)/examples
examplefiles_DATA = ../resources/examples/*

sizes = 256x256 128x128 64x64 48x48 32x32 22x22 16x16

install-data-hook:
	for size in ${sizes}; do \
		mkdir -p $(DESTDIR)${prefix}/share/icons/hicolor/"$$size"/apps||true; \
		convert ../resources/pixmaps/ManPageEditor.png -resize "$$size" $(DESTDIR)${prefix}/share/icons/hicolor/"$$size"/apps/ManPageEditor.png; \
	done

	mkdir -p $(DESTDIR)${prefix}/share/icons||true
	mkdir -p $(DESTDIR)${prefix}/share/icons/large||true
	mkdir -p $(DESTDIR)${prefix}/share/icons/mini||true
	convert ../resources/pixmaps/ManPageEditor.png -resize 32x32 $(DESTDIR)${prefix}/share/icons/ManPageEditor.png
	convert ../resources/pixmaps/ManPageEditor.png -resize 48x48 $(DESTDIR)${prefix}/share/icons/large/ManPageEditor.png
	convert ../resources/pixmaps/ManPageEditor.png -resize 16x16 $(DESTDIR)${prefix}/share/icons/mini/ManPageEditor.png

	mkdir -p $(DESTDIR)${prefix}/share/mime/packages||true
	cp -p ../resources/mime/maneditdoc-mime.xml $(DESTDIR)${prefix}/share/mime/packages||true

	mkdir -p $(DESTDIR)${prefix}/share/man/ru/man1||true
	cp -p ../resources/man/ru/manpageeditor.1 $(DESTDIR)${prefix}/share/man/ru/man1||true
	xz -f0 --text $(DESTDIR)${prefix}/share/man/ru/man1/manpageeditor.1||true
	
uninstall-hook:
	for size in ${sizes}; do \
		rm $(DESTDIR)${prefix}/share/icons/hicolor/"$$size"/apps/ManPageEditor.png||true; \
	done

	rm $(DESTDIR)${prefix}/share/icons/ManPageEditor.png||true
	rm $(DESTDIR)${prefix}/share/icons/large/ManPageEditor.png||true
	rm $(DESTDIR)${prefix}/share/icons/mini/ManPageEditor.png||true

	rm $(DESTDIR)${prefix}/share/mime/maneditdoc-mime.xml

install-exec-hook:
	@echo "*******************************************************"
	@echo "* If you have built from source you must now run:     *"
	@echo "* make install-mimetypes as root to install mimetypes *"
	@echo "*******************************************************"

uninstall-hook:
	rm -r $(DESTDIR)${pkgdatadir}
	@echo "***********************************************************"
	@echo "* If you have built from source you should now run:       *"
	@echo "* make uninstall-mimetypes as root to uninstall mimetypes *"
	@echo "***********************************************************"
