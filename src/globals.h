/*
 * ©K. D. Hedger. Fri 27 Nov 11:42:28 GMT 2015 kdhedger68713@gmail.com
 *  Victor Nabatov Sun 12 Dec 10:32:00 GMT 2016 greenray.spb@gmail.com
 *
 * This file (callbacks.cpp) is part of MPE-gtk2.
 *
 * MPE-gtk2 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or at your
 * option any later version.
 *
 * MPE-gtk2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MPE-gtk2. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GLOBALS_
#define _GLOBALS_

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>
#include <glib/gi18n.h>
#include <locale.h>

#include "config.h"

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourceiter.h>

#ifdef _ASPELL_
#include <aspell.h>
#endif

#define REPLACE   100
#define FINDNEXT  200
#define FINDPREV  300

#define MAXRECENT 10

#define VALIDCHARS         "[]a-zA-Z0-9:/@ ()_-,.*#;[\"]"
#define VALIDFILENAMECHARS "[A-Za-z0-9_-./ ]"
#define VALIDFUNCTIONCHARS "[A-Za-z0-9_*]"

#define BOLDESC    "\e[1m"
#define ITALICESC  "\e[4m"
#define ITALIC1ESC "\e[22m"
#define ITALIC2ESC "\e[24m"

#define NORMALESC  "\e[0m"
#define NORMALESC1 "\e[4m"
#define NORMALESC2 "\e[24m"
#define NORMALESC3 "\e[22m"

enum {NEWVBOX = 0, NEWHBOX};
enum {BOLD = 0, ITALIC, NORMAL};

struct pageStruct {
	GtkWidget*			pane;
	GtkScrolledWindow*	pageWindow;
	GtkScrolledWindow*	pageWindow2;
	GtkSourceBuffer*	buffer;
	GtkSourceView*		view;
	GtkSourceView*		view2;
	char*				filePath;
	GtkWidget*			tabName;
	GtkTextIter			iter;
	GtkTextIter			match_start;
	GtkTextIter			match_end;
	bool				isFirst;
	char*				fileName;
	bool				itsMe;
	bool				inTop;
	bool				isSplit;
	const char*			lang;
	GtkWidget*			tabVbox;
	bool				isSubsection;
};
//
// Manpage stuff
//
extern char*		manFilename;
extern char*		manName;
extern char*		manSection;
extern char*		manVersion;
extern char*		manAuthor;
extern char*		manCategory;
extern char*		manFilePath;

extern bool			pageOpen;
extern bool			dirty;
extern char*		savePath;
extern FILE			*previewFile;

extern GtkWidget*	nameBox;
extern GtkWidget*	sectionBox;
extern GtkWidget*	versionBox;
extern GtkWidget*	authorBox;
extern GtkWidget*	categoryBox;

extern GtkWidget*	window;
extern GtkNotebook*	notebook;

extern GtkWidget*	menubar;
extern GtkWidget*	menufile;
extern GtkWidget*	menuedit;
extern GtkWidget*	menuformat;
extern GtkWidget*	menuhelp;
extern GtkWidget*	menuprint;
extern GtkWidget*	menuclose;

extern GtkWidget*	redoMenu;
extern GtkWidget*	undoMenu;
extern GtkWidget*	saveMenu;
extern GtkWidget*	saveMenu;
extern GtkWidget*	saveAsMenu;
extern GtkWidget*	previewMenu;
extern GtkWidget*	saveAsMenu;
extern GtkWidget*	closeSectionMenu;

extern GtkWidget*	liveSearchWidget;
extern int			currentPage;

extern GtkWidget*	prefswin;
extern GtkWidget*	terminalBox;

extern bool			gzipPages;
extern bool			lineWrap;
extern bool			highLight;
extern int			tabWidth;
extern char*		fontAndSize;
extern char*		terminalCommand;
extern bool			showLiveSearch;
extern bool			useUnderline;

extern GtkWidget*	fontBox;

extern bool			tmpGzipPages;
extern bool			tmpLineWrap;
extern bool			tmpHighLight;
extern int			tmpTabWidth;
extern bool			tmpShowLiveSearch;
extern bool			tmpUseUnderline;

extern int			windowWidth;
extern int			windowHeight;
extern int			windowX;
extern int			windowY;

extern bool			wrapSearch;
extern bool			insensitiveSearch;
extern bool			replaceAll;

extern int			currentTabNumber;

extern GtkToolItem*	newButton;
extern GtkToolItem*	openButton;
extern GtkToolItem*	saveButton;
extern GtkToolItem*	closeButton;
extern GtkToolItem*	redoButton;
extern GtkToolItem*	undoButton;

extern GtkWidget*	findReplaceDialog;
extern GtkWidget*	findBox;
extern GtkWidget*	replaceBox;

extern char*		thePage;

extern GtkWidget*	spellCheckWord;
extern GtkWidget*	wordListDropbox;

extern char*		badWord;
extern char*		goodWord;

#ifdef _ASPELL_
extern AspellConfig*  aspellConfig;
extern AspellSpeller* spellChecker;
extern int            numWords;
extern GtkWidget*     badWordLabel;
#endif

pageStruct* getPageStructPtr(int pagenum);
void scrollToIterInPane(pageStruct* page, GtkTextIter* iter);
bool checkForDirty(void);
GtkWidget* creatNewBox(int orient, bool homog, int spacing);

#endif // _GLOBALS_
