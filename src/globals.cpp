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

#include "searchcallbacks.h"
#include "callbacks.h"
#include "script.h"
//
// Manpage
//
char* manFilename = NULL;
char* manName     = NULL;
char* manSection  = NULL;
char* manVersion  = NULL;
char* manAuthor   = NULL;
char* manCategory = NULL;
char* manFilePath = NULL;
bool  pageOpen    = false;
bool  dirty       = false;
char* savePath    = NULL;
FILE *previewFile = NULL;

GtkWidget*   nameBox;
GtkWidget*   sectionBox;
GtkWidget*   versionBox;
GtkWidget*   authorBox;
GtkWidget*   categoryBox;

GtkWidget*   window   = NULL;
GtkNotebook* notebook = NULL;

GtkWidget*   menuBar  = NULL;
GtkWidget*   menuFile;
GtkWidget*   menuEdit;
GtkWidget*   menuFormat;
GtkWidget*   menuHelp;
GtkWidget*   menuNewSection;
GtkWidget*   menuClose;
GtkWidget*   menuUndo;
GtkWidget*   menuRedo;
GtkWidget*   menuSave;
GtkWidget*   menuSaveAs;
GtkWidget*   menuPreview;
GtkWidget*   menuProperties;
GtkWidget*   menuPrint;
GtkWidget*   menuPdf;
GtkWidget*   menuCut;
GtkWidget*   menuCopy;
GtkWidget*   menuPaste;
GtkWidget*   menuFind;
GtkWidget*   menuBold;
GtkWidget*   menuItalic;
GtkWidget*   menuNormal;

GtkWidget*   menuCloseSection;

GtkWidget*   liveSearchWidget;

int currentPage = 0;
int currentTabNumber;
//
// Prefs
//
GtkWidget* prefswin;
GtkWidget* fontBox;
GtkWidget* terminalBox;

bool  gzipPages;
bool  lineWrap;
bool  highLight;
bool  showLiveSearch;
bool  useUnderline;
bool  tmpGzipPages;
bool  tmpLineWrap;
bool  tmpHighLight;
bool  tmpShowLiveSearch;
bool  tmpUseUnderline;
int   tabWidth;
int   tmpTabWidth;
int   windowWidth;
int   windowHeight;
int   windowX = -1;
int   windowY = -1;
bool  wrapSearch;
bool  insensitiveSearch;
bool  replaceAll;
char* fontAndSize;
char* terminalCommand;
char* thePage = NULL;
//
// Toolbar
//
GtkToolItem* newButton;
GtkToolItem* openButton;
GtkToolItem* saveButton;
GtkToolItem* closeButton;
GtkToolItem* redoButton;
GtkToolItem* undoButton;
GtkToolItem* cutButton;
GtkToolItem* copyButton;
GtkToolItem* pasteButton;
GtkToolItem* findButton;
//
// Find replace
//
GtkWidget* findReplaceDialog;
GtkWidget* findBox;
GtkWidget* replaceBox;
//
// Spellcheck
//
GtkWidget* spellCheckWord = NULL;
GtkWidget* wordListDropbox;
char*      badWord  = NULL;
char*      goodWord = NULL;

#ifdef _ASPELL_
AspellConfig*  aspellConfig;
AspellSpeller* spellChecker = 0;
int            numWords     = 0;
GtkWidget*     badWordLabel = NULL;
#endif // _ASPELL_

void scrollToIterInPane(pageStruct* page, GtkTextIter* iter) {
	if (page->inTop == true)
		 gtk_text_view_scroll_to_iter((GtkTextView*)page->view,  iter, 0, true, 0, 0.5);
	else gtk_text_view_scroll_to_iter((GtkTextView*)page->view2, iter, 0, true, 0, 0.5);
}

pageStruct* getPageStructPtr(int pagenum) {
	int        thispage;
	GtkWidget* pageBox;

	if (pagenum == -1)
		 thispage = gtk_notebook_get_current_page(notebook);
	else thispage = pagenum;

	pageBox = gtk_notebook_get_nth_page(notebook, thispage);
	if (pageBox == NULL)
		 return(NULL);
	else return((pageStruct*)g_object_get_data((GObject*)pageBox, "pagedata"));
}

bool checkForDirty(void) {
	int         numtabs = gtk_notebook_get_n_pages(notebook);
	pageStruct* page;

	for (int loop = 0; loop < numtabs; loop++) {
			page = getPageStructPtr(loop);
			if (gtk_text_buffer_get_modified((GtkTextBuffer*)page->buffer) == true)
				return(true);
		}
	return(false);
}

GtkWidget* creatNewBox(int orient, bool homog, int spacing) {
    GtkWidget *retwidg = NULL;

    if (orient == NEWVBOX)
         retwidg = gtk_vbox_new(homog, spacing);
    else retwidg = gtk_hbox_new(homog, spacing);
    return(retwidg);
}
