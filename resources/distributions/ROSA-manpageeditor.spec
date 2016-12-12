# ROSA Linux Distribution
# file spec for building package
%define oname ManPageEditor

Name:        manpageeditor
Version:     0.1.2
Release:     1
Summary:     Manual pages editor
Summary(ru): Редактор справочных страниц (man-pages)
License:     GPLv3
Group:       Editors

URL:     https://www.opendesktop.org/content/show.php/Man+Page+Editor+?content=160219
Source0: http://khapplications.darktech.org/zips/%name/%oname-%version.tar.gz

BuildRequires: aspell-devel
BuildRequires: groff-base
BuildRequires: imagemagick
BuildRequires: pkgconfig(gdk-2.0)
BuildRequires: pkgconfig(gtk+-2.0)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gtksourceview-2.0)
BuildRequires: pkgconfig(pango)

Requires: aspell
Requires: groff-base
Requires: pkgconfig(gdk-2.0)
Requires: pkgconfig(gtk+-2.0)
Requires: pkgconfig(glib-2.0)
Requires: pkgconfig(gtksourceview-2.0)
Requires: pkgconfig(pango)

Requires(post): desktop-file-utils
Requires(post): gtk-update-icon-cache
Requires(post): shared-mime-info

Requires(postun): desktop-file-utils
Requires(postun): gtk-update-icon-cache
Requires(postun): shared-mime-info

%description
Create, edit, import, preview man-pages.

%description -l ru
Приложение для создания, импорта,
редактирования и предпросмотра
справочных страниц (man-pages).

%files -f %name.lang
%doc AUTHORS BUGS-ETC ChangeLog COPYING NEWS README
%_bindir/%name
%_datadir/applications/%name.desktop
%_datadir/mime/packages/maneditdoc-mime.xml
%_datadir/%name/help/*
%_datadir/%name/examples/*
%_mandir/man1/%name.1*
%_mandir/ru/man1/%name.1*
%_datadir/pixmaps/%name.png
%_iconsdir/hicolor/256x256/apps/%name.png
%_iconsdir/hicolor/128x128/apps/%name.png
%_iconsdir/hicolor/64x64/apps/%name.png
%_iconsdir/hicolor/48x48/apps/%name.png
%_iconsdir/hicolor/32x32/apps/%name.png
%_iconsdir/hicolor/22x22/apps/%name.png
%_iconsdir/hicolor/16x16/apps/%name.png
%_iconsdir/%name.png
%_miconsdir/%name.png
%_liconsdir/%name.png

#-----------------------------------------------------------------------------

%prep
%setup -q -n %oname-%version

%build
%configure2_5x --prefix=/usr --enable-aspell
%make

%install
%makeinstall_std

%find_lang %name

%post
%_update_desktop_database_bin %_datadir/applications
%_update_icon_cache_bin       %_iconsdir/hicolor
%_update_mime_database_bin -n %_datadir/mime

%postun
if [ $1 = 0 ]; then
    %_update_desktop_database_bin %_datadir/applications
    %_update_icon_cache_bin       %_iconsdir/hicolor
    %_update_mime_database_bin    %_datadir/mime
fi


%changelog
* Sat Dec 12 2016 Victor Nabatov <greenray.spb@gmail.com> 0.1.2-1
= New version 0.1.2
+ Fixed "-Werror=format-security"
+ Fixed "-Wunused-result"
+ Fixed "-Wempty-body"
+ Added internationalization
+ Added translation into Russian
+ Added man page in Russian
