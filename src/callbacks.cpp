/*
 * ©K. D. Hedger. Fri 27 Nov 11:42:28 GMT 2015 kdhedger68713@gmail.com
 *  Victor Nabatov Sun 12 Dec 10:32:00 GMT 2016 greenray.spb@gmail.com
 *
 * This file (callbacks.cpp) is part of ManPageEditor.
 *
 * ManPageEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * ManPageEditor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ManPageEditor. If not, see <http://www.gnu.org/licenses/>.
 */

#include "globals.h"
#include "guis.h"
#include "callbacks.h"
#include "searchcallbacks.h"
#include "spellcheck.h"
#include "script.h"

GtkWidget*        tabMenu;
GtkPrintSettings* settings = NULL;

int boldnum   = 0;
int italicnum = 0;

void showHideWidget(GtkWidget* widget, bool show) {
	if (show == false)
		 gtk_widget_hide(widget);
	else gtk_widget_show(widget);
}

void refreshMainWindow(void) {
	gtk_widget_show_all(window);
	showHideWidget(liveSearchWidget, showLiveSearch);
}

int yesNo(char* question, char* file) {
	GtkWidget* dialog;
	int        result;

	dialog = gtk_message_dialog_new(
        GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_NONE,
        "%s %s",
        question,
        file
    );

#ifdef _USEGTK3_
	gtk_dialog_add_buttons(
        (GtkDialog*)dialog,
        _("Yes"), GTK_RESPONSE_YES,
        _("No"),  GTK_RESPONSE_CANCEL,
        NULL
    );
#else // not _USEGTK3_
	gtk_dialog_add_buttons(
        (GtkDialog*)dialog,
         GTK_STOCK_YES,
         GTK_RESPONSE_YES,
         GTK_STOCK_NO,
         GTK_RESPONSE_CANCEL,
         NULL
    );
#endif // _USEGTK3_

	gtk_window_set_title(GTK_WINDOW(dialog), _("What Do You Want To Do?"));
	result = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	return(result);
}

int show_question(char* filename) {
	GtkWidget* dialog;
	gint       result;
	char*      message;

	asprintf(&message, _("Save file %s before closing?"), filename);
    dialog = gtk_message_dialog_new(
        GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_NONE,
        "%s",
        message
    );

#ifdef _USEGTK3_
	gtk_dialog_add_buttons(
        (GtkDialog*)dialog,
        _("Save"),   GTK_RESPONSE_YES,
        _("Cancel"), GTK_RESPONSE_CANCEL,
        _("No"),     GTK_RESPONSE_NO,
        NULL
    );
#else // not _USEGTK3_
	gtk_dialog_add_buttons(
        (GtkDialog*)dialog,
        GTK_STOCK_SAVE,
        GTK_RESPONSE_YES,
        GTK_STOCK_CANCEL,
        GTK_RESPONSE_CANCEL,
        GTK_STOCK_NO,
        GTK_RESPONSE_NO,
        NULL
    );
#endif // _USEGTK3_

	gtk_window_set_title(GTK_WINDOW(dialog), _("Warning: unsaved data!"));
	result = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	g_free(message);
	return(result);
}

void setSensitive(void) {
//
// Toolbar
//
	gtk_widget_set_sensitive((GtkWidget*)undoButton, dirty);
	gtk_widget_set_sensitive((GtkWidget*)redoButton, dirty);
	gtk_widget_set_sensitive((GtkWidget*)saveButton, dirty);
//
// Menu
//
	gtk_widget_set_sensitive((GtkWidget*)undoMenu, dirty);
	gtk_widget_set_sensitive((GtkWidget*)redoMenu, dirty);
	gtk_widget_set_sensitive((GtkWidget*)saveMenu, dirty);

	gtk_widget_set_sensitive((GtkWidget*)saveAsMenu,   pageOpen);
	gtk_widget_set_sensitive((GtkWidget*)exportMenu,   pageOpen);
	gtk_widget_set_sensitive((GtkWidget*)exportAsMenu, pageOpen);
	gtk_widget_set_sensitive((GtkWidget*)previewMenu,  pageOpen);

	refreshMainWindow();
}

void closeTab(GtkWidget* widget, gpointer data) {
	long        thispage;
	pageStruct* page;

	if (data == NULL)
		 thispage = 0;
	else thispage = gtk_notebook_get_current_page(notebook);

	page = getPageStructPtr(thispage);
	if (page == NULL)
		return;

	if (page->filePath != NULL) g_free(page->filePath);
	if (page->fileName != NULL) g_free(page->fileName);
	if (page != NULL)           g_free(page);

	currentPage--;
	gtk_notebook_remove_page(notebook, thispage);
	setSensitive();
}

void closeAllTabs(GtkWidget* widget, gpointer data) {
	int	numtabs = gtk_notebook_get_n_pages(notebook);
	for (int loop = 0; loop < numtabs; loop++)
		closeTab(NULL, 0);
}

void closePage(GtkWidget* widget, gpointer data) {
	int   retval;
	char* command;

	if (checkForDirty() == true) {
        retval = yesNo((char*) _("Do you want to save?"), manName);
		if (retval == GTK_RESPONSE_YES)
			saveManpage(NULL, NULL);
    }

	closeAllTabs(NULL, NULL);

	if (manFilename != NULL) {
        asprintf(&command, "rm -r \"%s\"", manFilename);
		system(command);
		g_free(command);
		manFilename = NULL;
    }

	if (manName     != NULL) g_free(manName);
	if (manSection  != NULL) g_free(manSection);
	if (manVersion  != NULL) g_free(manVersion);
	if (manAuthor   != NULL) g_free(manAuthor);
	if (manFilePath != NULL) g_free(manFilePath);
	if (exportPath  != NULL) g_free(exportPath);

	manName     = NULL;
	manSection  = NULL;
	manVersion  = NULL;
	manAuthor   = NULL;
	manFilePath = NULL;
	exportPath  = NULL;
	manFilename = NULL;

	pageOpen = false;
	dirty    = false;
	setSensitive();
}

void copyToClip(GtkWidget* widget, gpointer data) {
	pageStruct*	page = getPageStructPtr(-1);
	if (page == NULL)
		return;
	gtk_text_buffer_copy_clipboard((GtkTextBuffer*)page->buffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
}

void cutToClip(GtkWidget* widget, gpointer data) {
	pageStruct* page = getPageStructPtr(-1);
	if (page == NULL)
		return;
	gtk_text_buffer_cut_clipboard((GtkTextBuffer*)page->buffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), true);
	setSensitive();
}

void pasteFromClip(GtkWidget* widget, gpointer data) {
	pageStruct* page = getPageStructPtr(-1);
	if (page == NULL)
		return;
	gtk_text_buffer_paste_clipboard((GtkTextBuffer*)page->buffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), NULL, true);
	setSensitive();
}

void undo(GtkWidget* widget, gpointer data) {
	pageStruct*	page = getPageStructPtr(-1);
	if (page == NULL)
		return;

	if (gtk_source_buffer_can_undo(page->buffer)) {
        gtk_source_buffer_undo(page->buffer);
		page->isFirst = true;
		setSensitive();
    }
}

void redo(GtkWidget* widget, gpointer data) {
	pageStruct* page = getPageStructPtr(-1);
	if (page == NULL)
		return;

	if (gtk_source_buffer_can_redo(page->buffer)) {
		gtk_source_buffer_redo(page->buffer);
		page->isFirst = true;
		setSensitive();
	}
}

void openHelp(GtkWidget* widget, gpointer data) {
	char* runhelp;
	asprintf(&runhelp, "xdg-open %s/help/help.html", DATADIR);
	system(runhelp);
}

void copyToClipboard(GtkWidget* widget, gpointer data) {
	GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(clipboard, (char*)data,-1);
}

void populatePopupMenu(GtkTextView *entry, GtkMenu *menu, gpointer user_data) {
	pageStruct* page = getPageStructPtr(-1);
	GtkTextIter start;
	GtkTextIter end;
	char*       selection = NULL;
	GtkWidget*  menuitem;

#ifdef _ASPELL_
	GtkWidget* image;
#endif

	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_prepend(GTK_MENU_SHELL(menu), menuitem);

	if (gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer, &start, &end)) {
        selection = gtk_text_buffer_get_text((GtkTextBuffer*)page->buffer, &start, &end, false);
		if (selection != NULL) {
#ifdef _ASPELL_
//
// spell check
//
#ifdef _USEGTK3_
			menuitem = gtk_menu_item_new_with_label(_("Check Spellling"));
#else // _USEGTK3_
			menuitem = gtk_image_menu_item_new_with_label(_("Check Spellling"));
			image    = gtk_image_new_from_stock(GTK_STOCK_SPELL_CHECK, GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem, image);
#endif // _USEGTK3_
			gtk_menu_shell_prepend(GTK_MENU_SHELL(menu), menuitem);
			g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(checkWord), NULL);
#endif // _ASPELL
		}
	}
	gtk_widget_show_all((GtkWidget*)menu);
}

gboolean whatPane(GtkWidget *widget, GdkEvent *event, gpointer data) {
	pageStruct* page = (pageStruct*)getPageStructPtr(-1);

	if ((long)data == 1)
		 page->inTop = true;
	else page->inTop = false;

	return(false);
}

bool tabPopUp(GtkWidget* my_widget, GdkEventButton* event, gpointer user_data) {
	pageStruct* page;
	GtkWidget*  menuitem;
	GtkWidget*  image;

	if (event->button == 3 && event->type == GDK_BUTTON_PRESS) {
		tabMenu = gtk_menu_new();
		page    = (pageStruct*)user_data;
#ifdef _ASPELL_
//
// Check document
//
#ifdef _USEGTK3_
		menuitem = gtk_menu_item_new_with_label(_("Spell Check Document"));
#else // not _USEGTK3_
		image    = gtk_image_new_from_stock(GTK_STOCK_SPELL_CHECK, GTK_ICON_SIZE_MENU);
		menuitem = gtk_image_menu_item_new_with_label(_("Spell Check Document"));
		gtk_image_menu_item_set_image((GtkImageMenuItem*)menuitem, image);
#endif // _USEGTK3_
		gtk_menu_shell_append(GTK_MENU_SHELL(tabMenu), menuitem);
		g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(doSpellCheckDoc), (void*)page->filePath);
#endif // _ASPELL
//
// Rename
//
#ifdef _USEGTK3_
		menuitem = gtk_menu_item_new_with_label(_("Rename Section"));
#else
		image    = gtk_image_new_from_stock(GTK_STOCK_FIND_AND_REPLACE, GTK_ICON_SIZE_MENU);
		menuitem = gtk_image_menu_item_new_with_label(_("Rename Section"));
		gtk_image_menu_item_set_image((GtkImageMenuItem*)menuitem, image);
#endif // _USEGTK3_
		gtk_menu_shell_append(GTK_MENU_SHELL(tabMenu), menuitem);
		g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(renameSection), (void*)page);

		gtk_menu_attach_to_widget(GTK_MENU(tabMenu), my_widget, NULL);
		gtk_menu_popup(GTK_MENU(tabMenu), NULL, NULL, NULL, NULL, event->button, event->time);
		gtk_widget_show_all((GtkWidget*)tabMenu);

		   return(true);
    } else return(false);
}

void writeExitData(void) {
	GtkAllocation alloc;

	FILE* fd = NULL;
	char* filename;

	int winx;
	int winy;

	gtk_widget_get_allocation(window, &alloc);
	gtk_window_get_position((GtkWindow*)window, &winx, &winy);

	asprintf(&filename, "%s/.config/ManPageEditor", getenv("HOME"));
	g_mkdir_with_parents(filename, 493);
	g_free(filename);
	asprintf(&filename, "%s/.config/ManPageEditor/editor.window.rc", getenv("HOME"));
	fd = fopen(filename, "w");
	if (fd != NULL) {
		fprintf(fd, "windowsize	%i %i %i %i\n", alloc.width, alloc.height, winx, winy);
		fclose(fd);
    }
	g_free(filename);
}

void writeConfig(void) {
	GtkAllocation alloc;

	FILE* fd=NULL;
	char* filename;

	int winx;
	int winy;

	gtk_widget_get_allocation(window, &alloc);
	gtk_window_get_position((GtkWindow*)window, &winx, &winy);

	asprintf(&filename, "%s/.config/ManPageEditor", getenv("HOME"));
	g_mkdir_with_parents(filename, 493);
	g_free(filename);
	asprintf(&filename, "%s/.config/ManPageEditor/editor.rc", getenv("HOME"));
	fd = fopen(filename, "w");
	if (fd != NULL) {
		fprintf(fd, "wrapline %i\n",             (int)lineWrap);
		fprintf(fd, "highlightcurrentline %i\n", (int)highLight);
		fprintf(fd, "useundeline %i\n",          (int)useUnderline);
		fprintf(fd, "insenssearch %i\n",         (int)insensitiveSearch);
		fprintf(fd, "wrapsearch %i\n",           (int)wrapSearch);
		fprintf(fd, "showlivesearch %i\n",       (int)showLiveSearch);
		fprintf(fd, "gzip %i\n",                 (int)gzipPages);
		fprintf(fd, "tabwidth %i\n",                  tabWidth);
		fprintf(fd, "font %s\n",                      fontAndSize);
		fprintf(fd, "terminalcommand %s\n",           terminalCommand);
		fclose(fd);
    }
	g_free(filename);
}

void doShutdown(GtkWidget* widget, gpointer data) {
	char* command;
	int   result;

	if (dirty == true) {
		if (manFilePath != NULL)
			 result = show_question(g_path_get_basename(manFilePath));
		else result = show_question((char*)"manpage");

		switch(result) {
			case GTK_RESPONSE_YES:
				saveManpage(NULL, NULL);
				break;
			case GTK_RESPONSE_NO:
				break;
			default:
				return;
			break;
		}
    }
	if (manFilename != NULL) {
		asprintf(&command, "rm -r \"%s\"", manFilename);
		system(command);
		g_free(command);
		manFilename = NULL;
    }

#ifdef _ASPELL_
	delete_aspell_config(aspellConfig);
	delete_aspell_speller(spellChecker);
#endif

	writeExitData();
	gtk_main_quit();
}

void setPrefs(GtkWidget* widget, gpointer data) {
	if (strcmp(gtk_widget_get_name(widget), "gzip")       == 0) tmpGzipPages      = gtk_toggle_button_get_active((GtkToggleButton*)data);
	if (strcmp(gtk_widget_get_name(widget), "wrap")       == 0) tmpLineWrap       = gtk_toggle_button_get_active((GtkToggleButton*)data);
	if (strcmp(gtk_widget_get_name(widget), "high")       == 0) tmpHighLight      = gtk_toggle_button_get_active((GtkToggleButton*)data);
	if (strcmp(gtk_widget_get_name(widget), "underline")  == 0) tmpUseUnderline   = gtk_toggle_button_get_active((GtkToggleButton*)data);
	if (strcmp(gtk_widget_get_name(widget), "livesearch") == 0) tmpShowLiveSearch = gtk_toggle_button_get_active((GtkToggleButton*)data);
	if (strcmp(gtk_widget_get_name(widget), "tabs")       == 0) tmpTabWidth       = (int)gtk_spin_button_get_value((GtkSpinButton*)data);
	if (strcmp(gtk_widget_get_name(widget), "cancel")     == 0) gtk_widget_destroy(prefswin);
	if (strcmp(gtk_widget_get_name(widget), "apply")      == 0) {
		gzipPages      = tmpGzipPages;
		lineWrap       = tmpLineWrap;
		highLight      = tmpHighLight;
		useUnderline   = tmpUseUnderline;
		showLiveSearch = tmpShowLiveSearch;
		showHideWidget(liveSearchWidget, showLiveSearch);

		if (terminalCommand != NULL) {
			g_free(terminalCommand);
			terminalCommand = strdup(gtk_entry_get_text((GtkEntry*)terminalBox));
		}

		if (fontAndSize != NULL) {
            g_free(fontAndSize);
			fontAndSize = strdup(gtk_entry_get_text((GtkEntry*)fontBox));
        }

		tabWidth = tmpTabWidth;
		gtk_widget_destroy(prefswin);
		resetAllFilePrefs();
		writeConfig();
		resetAllItalicTags();
    }
}

void doAbout(GtkWidget* widget, gpointer data) {
    const char* authors[] = {
        _("Developer: K.D.Hedger <keithhedger@keithhedger.darktech.org> http://khapplications.darktech.org"),
        _("Internationalization: Victor Nabatov <greenray.spb@gmail.com>"),
        _("\nMore by K.D.Hedger:\n"),
        "KKEdit\n" KKEDITPAGE "\n",
        "KKTerminal\n" KKTERMINALPAGE "\n",
        "XDecorations\n" XDECORPAGE "\n",
        "Xfce-Theme-Manager\n" THEMEMANAGER "\n",
        "Xfce4-Composite-Editor\n" COMPMANAGER "\n",
        "Manpage Editor\n" MANEDITPAGE "\n",
        "GtkSu\n" GTKSUPAGE "\n",
        "ASpell GUI\n" ASPELLPAGE "\n",
        "Clipboard Viewer\n" CLIPVIEW "\n",
        _("\nDevelopment versions can be found here:\nhttp://github.com/KeithDHedger")
    };
	const char* copyright      = COPYRIGHT "\n" MYEMAIL;
	const char* aboutboxstring = _("ManPage Editor - Editor for linux manpages");
	char*       license;
	const char* translators    = _("Russian: Victor Nabatov <greenray.spb@gmail.com>");

	g_file_get_contents(DATADIR "/docs/gpl-3.0.txt", &license, NULL, NULL);

	gtk_show_about_dialog(
        NULL,
        "authors",            authors,
        "comments",           aboutboxstring,
        "copyright",          copyright,
        "version",            VERSION,
        "website",            MANEDITPAGE,
        "program-name",       "ManPage Editor",
        "website-label",      _("Manpage Editor Page"),
        "logo-icon-name",     "manpageeditor",
		"license",            license,
		"translator-credits", translators,
		"wrap-license",       true,
        NULL
    );

	g_free(license);
}

void printFile(GtkWidget* widget, gpointer data) {
	char* holdpath;
	char* command;
	bool  holdgzipages = gzipPages;

	holdpath  = exportPath;
	gzipPages = false;

	if ((long)data == 1) {
		exportPath = (char*)"/tmp/printexportfile";
		exportFile(NULL, NULL);
		system("cat /tmp/printexportfile | groff -man -Tps|ps2pdf - |lp");
	} else {
		exportPath = NULL;
		exportFile(NULL, NULL);
		if (exportPath != NULL) {
            asprintf(&command, "cat %s | groff -man -Tps|ps2pdf - > \"%s\".pdf", exportPath, exportPath);
			system(command);
			g_free(command);
			unlink(exportPath);
			g_free(exportPath);
        }
    }

	unlink("/tmp/printexportfile");
	exportPath = holdpath;
	gzipPages  = holdgzipages;
}

void newEditor(GtkWidget* widget, gpointer data) {
	system("manpageeditor &");
}

void doFormat(GtkWidget* widget, gpointer data) {
	pageStruct*  page = getPageStructPtr(-1);
	GtkTextMark* mark;
	GtkTextIter  iter;
	GtkTextIter  start;
	GtkTextIter  end;
	GtkTextTag*  tag = NULL;

	char tagname[64];

    GtkTextTagTable* tagtable = gtk_text_buffer_get_tag_table((GtkTextBuffer*)page->buffer);

	mark = gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer);
	gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer, &iter, mark);

	switch((long)data) {
		case BOLD:
			boldnum++;
			sprintf((char*)&tagname, "bold-%i", boldnum);
			tag = gtk_text_tag_table_lookup(tagtable, tagname);
			while(tag != NULL) {
                boldnum++;
				sprintf((char*)&tagname, "bold-%i", boldnum);
				tag = gtk_text_tag_table_lookup(tagtable, tagname);
            }
			tag = gtk_text_buffer_create_tag((GtkTextBuffer*)page->buffer, tagname, "weight", PANGO_WEIGHT_BOLD, NULL);
			gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer, &start, &end);
			gtk_text_buffer_apply_tag((GtkTextBuffer*)page->buffer, tag, &start, &end);
			break;

		case NORMAL:
			gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer, &start, &end);
			gtk_text_buffer_remove_all_tags((GtkTextBuffer*)page->buffer, &start, &end);
			break;

		case ITALIC:
			italicnum++;
			sprintf((char*)&tagname, "italic-%i", italicnum);
			tag = gtk_text_tag_table_lookup(tagtable, tagname);
			while(tag != NULL) {
				italicnum++;
				sprintf((char*)&tagname, "bold-%i", italicnum);
				tag = gtk_text_tag_table_lookup(tagtable, tagname);
            }
			if (useUnderline == true)
				 tag = gtk_text_buffer_create_tag((GtkTextBuffer*)page->buffer, tagname, "underline", PANGO_UNDERLINE_SINGLE, NULL);
			else tag = gtk_text_buffer_create_tag((GtkTextBuffer*)page->buffer, tagname, "style",     PANGO_STYLE_ITALIC,     NULL);

			gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer, &start, &end);
			gtk_text_buffer_apply_tag((GtkTextBuffer*)page->buffer, tag, &start, &end);
		break;
    }

	dirty = true;
	setSensitive();
	refreshMainWindow();
}

void redoProps(GtkWidget* widget, gpointer data) {
	GtkWidget* dialog;
	gint       result;
	GtkWidget* content_area;
	GtkWidget* label;
	GtkWidget* hbox;

	dialog = gtk_message_dialog_new(
        GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_NONE,
        _("Set Properties")
    );

#ifdef _USEGTK3_
	gtk_dialog_add_buttons(
        (GtkDialog*)dialog,
        "OK", GTK_RESPONSE_OK,
        _("Cancel"), GTK_RESPONSE_CANCEL,
        NULL
    );
#else // not _USEGTK3_
	gtk_dialog_add_buttons(
        (GtkDialog*)dialog,
        GTK_STOCK_CANCEL,
        GTK_RESPONSE_CANCEL,
        GTK_STOCK_OK,
        GTK_RESPONSE_YES,
        NULL
    );
#endif // _USEGTK3_
	gtk_window_set_title(GTK_WINDOW(dialog), _("Properties"));

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	hbox = creatNewBox(NEWHBOX, false, 0);

	nameBox = gtk_entry_new();
	label   = gtk_label_new(_("Name\t"));
	gtk_box_pack_start(GTK_BOX(hbox), label,   true, true, 0);
	gtk_box_pack_start(GTK_BOX(hbox), nameBox, true, true, 0);
	if (manName != NULL)
		gtk_entry_set_text((GtkEntry*)nameBox, manName);
	gtk_container_add(GTK_CONTAINER(content_area), hbox);

	hbox = creatNewBox(NEWHBOX, false, 0);

	sectionBox = gtk_entry_new();
	label      = gtk_label_new(_("Section\t"));
	gtk_box_pack_start(GTK_BOX(hbox), label,      true, true, 0);
	gtk_box_pack_start(GTK_BOX(hbox), sectionBox, true, true, 0);
	if (manSection != NULL)
		gtk_entry_set_text((GtkEntry*)sectionBox , manSection);
	gtk_container_add(GTK_CONTAINER(content_area), hbox);

	hbox = creatNewBox(NEWHBOX, false, 0);

    versionBox = gtk_entry_new();
	label      = gtk_label_new(_("Version\t"));
	gtk_box_pack_start(GTK_BOX(hbox), label,      true, true, 0);
	gtk_box_pack_start(GTK_BOX(hbox), versionBox, true, true, 0);
	if (manVersion != NULL)
		gtk_entry_set_text((GtkEntry*)versionBox, manVersion);
	gtk_container_add(GTK_CONTAINER(content_area), hbox);

	hbox = creatNewBox(NEWHBOX,false,0);

	authorBox = gtk_entry_new();
	label     = gtk_label_new(_("Author\t"));
	gtk_box_pack_start(GTK_BOX(hbox), label,     true, true, 0);
	gtk_box_pack_start(GTK_BOX(hbox), authorBox, true, true, 0);
	if (manAuthor != NULL)
		gtk_entry_set_text((GtkEntry*)authorBox, manAuthor);
	gtk_container_add(GTK_CONTAINER(content_area), hbox);

	hbox = creatNewBox(NEWHBOX, false, 0);

	categoryBox = gtk_entry_new();
	label       = gtk_label_new(_("Category\t"));
	gtk_box_pack_start(GTK_BOX(hbox), label,       true, true, 0);
	gtk_box_pack_start(GTK_BOX(hbox), categoryBox, true, true, 0);
	if (manCategory != NULL)
		gtk_entry_set_text((GtkEntry*)categoryBox, manCategory);
	gtk_container_add(GTK_CONTAINER(content_area), hbox);


	gtk_widget_show_all(content_area);
	result = gtk_dialog_run(GTK_DIALOG(dialog));

	if (result == GTK_RESPONSE_YES) {
        manName     = strdup(gtk_entry_get_text((GtkEntry*)nameBox));
        manSection  = strdup(gtk_entry_get_text((GtkEntry*)sectionBox));
		manVersion  = strdup(gtk_entry_get_text((GtkEntry*)versionBox));
		manAuthor   = strdup(gtk_entry_get_text((GtkEntry*)authorBox));
		manCategory = strdup(gtk_entry_get_text((GtkEntry*)categoryBox));
		dirty = true;
		setSensitive();
    }
	gtk_widget_destroy(dialog);
}

void previewPage(GtkWidget* widget, gpointer data) {
	char  *command;
	char* holdpath = exportPath;
	char  tplate[] = "/tmp/mpprevXXXXXX";
	int   gotopen  = -1;

	gotopen = mkstemp(tplate);
	if (gotopen != -1) {
		previewFile = fdopen(gotopen, "w");
		if (previewFile != NULL) {
            exportPath = tplate;
			exportFile(NULL, NULL);
			if (gzipPages == true)
				 asprintf(&command, "%s man %s.gz", terminalCommand, tplate);
			else asprintf(&command, "%s man %s",    terminalCommand, tplate);

			system(command);
			exportPath = holdpath;
        }
		previewFile = NULL;
		free(command);
		if (gzipPages == true)
			 asprintf(&command, "%s.gz", tplate);
		else asprintf(&command, "%s",    tplate);
		unlink(command);
		free(command);
    }
}

void reorderDirty(GtkNotebook *notebook, GtkWidget *child, guint page_num, gpointer user_data) {
	pageStruct* page = getPageStructPtr(page_num);
	makeDirty(NULL, (void*)page);
}
