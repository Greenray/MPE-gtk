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

#include <gdk/gdkkeysyms.h>

#include "callbacks.h"
#include "searchcallbacks.h"
#include "spellcheck.h"

void doPrefs(void) {
	GtkWidget* vbox;
	GtkWidget* hbox;
	GtkWidget* item;

	prefswin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow*)prefswin, _("Preferences"));
	vbox = creatNewBox(NEWVBOX, false, 8);
	//
	// Wraplines
	//
	item = gtk_check_button_new_with_label(_("Wrap Lines"));
	gtk_widget_set_name(item, "wrap");
	gtk_toggle_button_set_active((GtkToggleButton*)item, lineWrap);
	gtk_box_pack_start(GTK_BOX(vbox), item, true, true, 0);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(setPrefs), (void*)item);
	//
	// Highlight
	//
	item = gtk_check_button_new_with_label(_("Highlight Current Line"));
	gtk_widget_set_name(item, "high");
	gtk_toggle_button_set_active((GtkToggleButton*)item, highLight);
	gtk_box_pack_start(GTK_BOX(vbox), item, true, true, 0);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(setPrefs), (void*)item);
	//
	// Use underline
	//
	item = gtk_check_button_new_with_label(_("Use Underline"));
	gtk_widget_set_name(item, "underline");
	gtk_toggle_button_set_active((GtkToggleButton*)item, useUnderline);
	gtk_box_pack_start(GTK_BOX(vbox), item, true, true, 0);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(setPrefs), (void*)item);
	//
	// gzip manpages
	//
	item = gtk_check_button_new_with_label(_("GZip Man Pages"));
	gtk_widget_set_name(item, "gzip");
	gtk_toggle_button_set_active((GtkToggleButton*)item, gzipPages);
	gtk_box_pack_start(GTK_BOX(vbox), item, true, true, 0);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(setPrefs), (void*)item);
	//
	// Show live search in toolbar
	//
	item = gtk_check_button_new_with_label(_("Show 'Live Search' in toolbar"));
	gtk_widget_set_name(item, "livesearch");
	gtk_toggle_button_set_active((GtkToggleButton*)item, showLiveSearch);
	gtk_box_pack_start(GTK_BOX(vbox), item, true, true, 0);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(setPrefs), (void*)item);
	//
	// Tabwidth
	//
#if 1
	GtkObject* adj = gtk_adjustment_new(tmpTabWidth, 1, 64, 1, 1, 0);
	hbox = creatNewBox(NEWHBOX, true, 0);
	item = gtk_spin_button_new((GtkAdjustment*)adj, 1, 0);
	gtk_widget_set_name(item, "tabs");
	gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(_("Tab width: ")), true, true, 0);
	gtk_container_add(GTK_CONTAINER(hbox), item);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, true, true, 0);
	g_signal_connect(G_OBJECT(item), "value-changed", G_CALLBACK(setPrefs), (void*)item);
#endif // 1
	//
	// Font
	//
	fontBox = gtk_entry_new();
	hbox    = creatNewBox(NEWHBOX, true, 0);
	gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(_("Font And Size: ")), true, true, 0);
	gtk_container_add(GTK_CONTAINER(hbox), fontBox);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, true, true, 0);
	gtk_entry_set_text((GtkEntry*)fontBox, fontAndSize);
	//
	// Terminal command
	//
	terminalBox = gtk_entry_new();
	hbox        = creatNewBox(NEWHBOX, true, 0);
	gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(_("Terminal Command: ")), true, true, 0);
	gtk_container_add(GTK_CONTAINER(hbox), terminalBox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, true, true, 0);
	gtk_entry_set_text((GtkEntry*)terminalBox, terminalCommand);
	gtk_widget_show_all(hbox);
	//
	// Buttons
	//
	gtk_box_pack_start(GTK_BOX(vbox), gtk_hseparator_new(), true, true, 0);
	hbox = creatNewBox(NEWHBOX, true, 4);
	item = gtk_button_new_from_stock(GTK_STOCK_APPLY);
	gtk_box_pack_start(GTK_BOX(hbox), item, true, false, 2);
	gtk_widget_set_name(item, "apply");
	g_signal_connect(G_OBJECT(item), "clicked", G_CALLBACK(setPrefs), (void*)item);

	item = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_box_pack_start(GTK_BOX(hbox), item, true, false, 2);
	gtk_widget_set_name(item, "cancel");
	g_signal_connect(G_OBJECT(item), "clicked", G_CALLBACK(setPrefs), (void*)item);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, true, true, 2);
	//
	// Show preferences window
	//
	gtk_container_add(GTK_CONTAINER(prefswin), (GtkWidget*)vbox);
	gtk_widget_show_all(prefswin);
}

GtkToolItem* makeNewToolItem(const char* stock, const char* label) {
	GtkToolItem* button;
	GtkWidget*   image;

	button = gtk_tool_button_new_from_stock(stock);
	return(button);
}

void recentFileMenu(GtkRecentChooser *chooser, gpointer *data) {
	gchar *uri = NULL;
	char  *filename;

	uri = gtk_recent_chooser_get_current_uri((GtkRecentChooser*)chooser);
	if (uri != NULL) {
		filename = g_filename_from_uri((const gchar*)uri, NULL, NULL);
		closePage(NULL, NULL);
		openFile(filename);
		dirty = false;
		setSensitive();
		g_free(uri);
		g_free(filename);
    }
}

void setupRecent(GtkMenuItem *menu) {
	GtkRecentFilter *filter;
	GtkWidget       *recent;

	recent = gtk_recent_chooser_menu_new();
	gtk_recent_chooser_set_local_only(GTK_RECENT_CHOOSER(recent), false);
	gtk_recent_chooser_set_sort_type(GTK_RECENT_CHOOSER(recent), GTK_RECENT_SORT_MRU);
	gtk_recent_chooser_set_limit(GTK_RECENT_CHOOSER(recent), 10);
	filter = gtk_recent_filter_new();
	gtk_recent_filter_add_mime_type (filter, "application/x-maneditdoc");
	gtk_recent_chooser_set_filter(GTK_RECENT_CHOOSER(recent), filter);
	g_signal_connect(recent, "item_activated", G_CALLBACK(recentFileMenu), NULL);
	gtk_menu_item_set_submenu(menu, recent);
}

void buildMainGui(void) {
	GtkWidget*     vbox;
	GtkWidget*     menuitem;
	GtkWidget*     menu;
	GtkWidget*     toolbar;
	GtkToolItem*   toolbutton;
	GtkAccelGroup* accgroup;
	GtkWidget*     image;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow*)window, _("Man Pages Editor MPE-GTK2"));
	gtk_window_set_default_size((GtkWindow*)window, windowWidth, windowHeight);
	if (windowX != -1 && windowY != -1) {
		gtk_window_move((GtkWindow *)window, windowX, windowY);
    }
	g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(doShutdown), NULL);
	accgroup = gtk_accel_group_new();
	gtk_window_add_accel_group((GtkWindow*)window, accgroup);

	notebook = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_set_scrollable(notebook, true);
	g_signal_connect(G_OBJECT(notebook), "page-reordered", G_CALLBACK(reorderDirty), NULL);

	vbox    = creatNewBox(NEWVBOX, false, 0);
	menubar = gtk_menu_bar_new();
	toolbar = gtk_toolbar_new();

	gtk_box_pack_start(GTK_BOX(vbox), (GtkWidget*)menubar,  false, true, 0);
	gtk_box_pack_start(GTK_BOX(vbox), (GtkWidget*)toolbar,  false, true, 0);
	gtk_box_pack_start(GTK_BOX(vbox), (GtkWidget*)notebook, true,  true, 0);
	//
	// MENU
    //==========================================================================
    // FILE
    //--------------------------------------------------------------------------
	//
	menufile = gtk_menu_item_new_with_label(_("File"));
	menu     = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menufile), menu);
	//
	// New
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(newManpage), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, 'N', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//
	// New section
	//
	menuitem = gtk_image_menu_item_new_with_label(_("New Section"));
	image    = gtk_image_new_from_stock(GTK_STOCK_NEW, GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem, image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(newSection), NULL);
	//
	// Open
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(openManpage), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, 'O', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Open recent
	//
	menuitem = gtk_menu_item_new_with_mnemonic(_("_Recent"));
	setupRecent((GtkMenuItem*)menuitem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// New editor
	//
	menuitem = gtk_image_menu_item_new_with_label(_("New Editor"));
	image    = gtk_image_new_from_stock(GTK_STOCK_NEW, GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem, image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(newEditor), NULL);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Save
	//
	saveMenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), saveMenu);
	g_signal_connect(G_OBJECT(saveMenu), "activate", G_CALLBACK(saveFile), NULL);
	gtk_widget_add_accelerator((GtkWidget *)saveMenu, "activate", accgroup, 'S', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//
	// Save as
	//
	saveAsMenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), saveAsMenu);
	g_signal_connect(G_OBJECT(saveAsMenu), "activate", G_CALLBACK(saveFile), (void*)1);
	gtk_widget_set_sensitive((GtkWidget*)saveAsMenu, false);
	//
	// Preview
	//
	image       = gtk_image_new_from_stock(GTK_STOCK_FIND, GTK_ICON_SIZE_MENU);
	previewMenu = gtk_image_menu_item_new_with_label(_("Preview Page"));
	gtk_image_menu_item_set_image((GtkImageMenuItem *)previewMenu, image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), previewMenu);
	g_signal_connect(G_OBJECT(previewMenu), "activate", G_CALLBACK(previewPage), NULL);
	//
	// Properties
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_PROPERTIES, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(redoProps), NULL);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Print manpage
	//
	image     = gtk_image_new_from_stock(GTK_STOCK_PRINT, GTK_ICON_SIZE_MENU);
	menuprint = gtk_image_menu_item_new_with_label(_("Print Manpage to LP"));
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuprint, image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuprint);
	g_signal_connect(G_OBJECT(menuprint), "activate", G_CALLBACK(printFile), (void*)1);
	//
	// Print manpage to pdf
	//
	image     = gtk_image_new_from_stock(GTK_STOCK_PRINT, GTK_ICON_SIZE_MENU);
	menuprint = gtk_image_menu_item_new_with_label(_("Print Manpage To PDF"));
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuprint, image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuprint);
	g_signal_connect(G_OBJECT(menuprint), "activate", G_CALLBACK(printFile), (void*)2);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Close
	//
	menuclose = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuclose);
	g_signal_connect(G_OBJECT(menuclose), "activate", G_CALLBACK(closePage), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuclose, "activate", accgroup, 'W', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//
	// Close section
	//
	image = gtk_image_new_from_stock(GTK_STOCK_DELETE, GTK_ICON_SIZE_MENU);
	closeSectionMenu = gtk_image_menu_item_new_with_label(_("Delete Section"));
	gtk_image_menu_item_set_image((GtkImageMenuItem *)closeSectionMenu, image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), closeSectionMenu);
	g_signal_connect(G_OBJECT(closeSectionMenu), "activate", G_CALLBACK(deleteSection), NULL);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Quit
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doShutdown), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, 'Q', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    //
    // EDIT
    //--------------------------------------------------------------------------
	//
	// Edit menu
	//
	menuedit = gtk_menu_item_new_with_label(_("Edit"));
	menu=gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuedit),menu);
	//
	// Undo
	//
	undoMenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_UNDO, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),undoMenu);
	g_signal_connect(G_OBJECT(undoMenu),"activate",G_CALLBACK(undo), NULL);
	gtk_widget_add_accelerator((GtkWidget *)undoMenu, "activate", accgroup, 'Z', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//
	// Redo
	//
	redoMenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_REDO, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), redoMenu);
	g_signal_connect(G_OBJECT(redoMenu), "activate", G_CALLBACK(redo), NULL);
	gtk_widget_add_accelerator((GtkWidget *)redoMenu, "activate", accgroup, 'Z', (GdkModifierType)(GDK_SHIFT_MASK | GDK_CONTROL_MASK), GTK_ACCEL_VISIBLE);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Cut
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(cutToClip), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, 'X', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//
	// Copy
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(copyToClip), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, 'C', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	//
	// Paste
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(pasteFromClip), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, 'V', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Find
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(find), NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, 'F', GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    // Separator
	menuitem = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// Prefs
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(doPrefs), NULL);
    //
    // FORMATING
    //--------------------------------------------------------------------------
	//
	menuformat = gtk_menu_item_new_with_label(_("Formating"));
	menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuformat), menu);
	//
	// Bold
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_BOLD, NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, GDK_KEY_B, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(doFormat), (void*)BOLD);
	//
	// Italic
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_ITALIC, NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,GDK_KEY_I, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)ITALIC);
	//
	// Normal
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLEAR, NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem, "activate", accgroup, GDK_KEY_R, (GdkModifierType)GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)NORMAL);
    //
    // HELP
    //--------------------------------------------------------------------------
	//
	menuhelp = gtk_menu_item_new_with_label(_("Help"));
	menu     = gtk_menu_new();
	//
	// About
	//
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuhelp),menu);
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(doAbout), NULL);
	//
	// Help
	//
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP, NULL);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(openHelp), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	//
	// TOOLBAR
    //==========================================================================
	// New
	//
	newButton = makeNewToolItem(GTK_STOCK_NEW, _("New"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, newButton, -1);
	g_signal_connect(G_OBJECT(newButton), "clicked", G_CALLBACK(newSection), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)newButton, _("New File"));
	//
	// Save
	//
	saveButton = makeNewToolItem(GTK_STOCK_SAVE, _("Save"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, saveButton, -1);
	g_signal_connect(G_OBJECT(saveButton), "clicked", G_CALLBACK(saveFile), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)saveButton, _("Save File"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, gtk_separator_tool_item_new(), -1);
	//
	// Cut
	//
	toolbutton = makeNewToolItem(GTK_STOCK_CUT, _("Cut"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, toolbutton, -1);
	g_signal_connect(G_OBJECT(toolbutton), "clicked", G_CALLBACK(cutToClip), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton, _("Cut"));
	//
	// Copy
	//
	toolbutton = makeNewToolItem(GTK_STOCK_COPY, _("Copy"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, toolbutton,-1);
	g_signal_connect(G_OBJECT(toolbutton), "clicked", G_CALLBACK(copyToClip), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton, _("Copy"));
	//
	// Paste
	//
	toolbutton = makeNewToolItem(GTK_STOCK_PASTE, _("Paste"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, toolbutton, -1);
	g_signal_connect(G_OBJECT(toolbutton), "clicked", G_CALLBACK(pasteFromClip), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton, _("Paste"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, gtk_separator_tool_item_new(), -1);
	//
	// Undo
	//
	undoButton = makeNewToolItem(GTK_STOCK_UNDO, _("Undo"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, undoButton, -1);
	g_signal_connect(G_OBJECT(undoButton), "clicked", G_CALLBACK(undo), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)undoButton, _("Undo"));
	//
	// Redo
	//
	redoButton = makeNewToolItem(GTK_STOCK_REDO, _("Redo"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, redoButton, -1);
	g_signal_connect(G_OBJECT(redoButton), "clicked", G_CALLBACK(redo), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)redoButton, _("Redo"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, gtk_separator_tool_item_new(), -1);
	//
	// Find
	//
	toolbutton = makeNewToolItem(GTK_STOCK_FIND, _("Find"));
	gtk_toolbar_insert((GtkToolbar*)toolbar, toolbutton, -1);
	g_signal_connect(G_OBJECT(toolbutton), "clicked", G_CALLBACK(find), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton, _("Find/Replace"));
	//
	// Live search
	//
	liveSearchWidget = gtk_entry_new();
	toolbutton       = gtk_tool_item_new();
	gtk_container_add((GtkContainer *)toolbutton, liveSearchWidget);
	gtk_toolbar_insert((GtkToolbar*)toolbar, toolbutton, -1);
	g_signal_connect_after(G_OBJECT(liveSearchWidget), "key-release-event", G_CALLBACK(doLiveSearch), NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton, _("Live Search"));
    //
    // End of menu and toolbar
    //
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menufile);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuedit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuformat);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuhelp);

	gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)vbox);
	gtk_widget_set_sensitive((GtkWidget*)saveButton, false);
	gtk_widget_set_sensitive((GtkWidget*)saveMenu,   false);
}

void buildFindReplace(void) {
	GtkWidget* content_area;
	GtkWidget* replace;
	GtkWidget* image;
	GtkWidget* label;
	GtkWidget* vbox;
	GtkWidget* hbox;
	GtkWidget* item;

	findReplaceDialog = gtk_dialog_new_with_buttons(
        _("Find/Replace"),
        (GtkWindow*)window,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_STOCK_GO_FORWARD,
        FINDNEXT,
        GTK_STOCK_GO_BACK,
        FINDPREV,
        _("Replace"),
        REPLACE,
        NULL
    );

	gtk_dialog_set_default_response((GtkDialog*)findReplaceDialog, GTK_RESPONSE_OK);
	g_signal_connect(G_OBJECT(findReplaceDialog), "response", G_CALLBACK(doFindReplace), NULL);
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(findReplaceDialog));

	vbox = creatNewBox(NEWVBOX, false, 0);
	hbox = creatNewBox(NEWVBOX, false, 0);

	label = gtk_label_new(_("Find"));
	gtk_container_add(GTK_CONTAINER(content_area), label);
	gtk_widget_show(label);

	findBox = gtk_entry_new();
	gtk_entry_set_text((GtkEntry*)findBox, "");
	gtk_entry_set_activates_default((GtkEntry*)findBox, true);
	gtk_container_add(GTK_CONTAINER(content_area), findBox);

	label = gtk_label_new(_("Replace With"));
	gtk_container_add(GTK_CONTAINER(content_area), label);
	gtk_widget_show(label);

	replaceBox=gtk_entry_new();
	gtk_entry_set_text((GtkEntry*)replaceBox, "");
	gtk_entry_set_activates_default((GtkEntry*)replaceBox, true);
	gtk_container_add(GTK_CONTAINER(content_area), replaceBox);

	item = gtk_check_button_new_with_label(_("Case insensitive"));
	gtk_toggle_button_set_active((GtkToggleButton*)item, insensitiveSearch);
	gtk_box_pack_start(GTK_BOX(hbox), item, true, true, 0);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(doSearchPrefs), (void*)1);

	item = gtk_check_button_new_with_label(_("Wrap"));
	gtk_toggle_button_set_active((GtkToggleButton*)item, wrapSearch);
	gtk_box_pack_start(GTK_BOX(hbox), item, true, true, 0);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(doSearchPrefs), (void*)2);

	item = gtk_check_button_new_with_label(_("Replace All"));
	gtk_toggle_button_set_active((GtkToggleButton*)item, replaceAll);
	gtk_box_pack_start(GTK_BOX(hbox), item, true, true, 0);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item), "toggled", G_CALLBACK(doSearchPrefs), (void*)3);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, true, true, 0);
	gtk_box_pack_start(GTK_BOX(content_area), vbox, true, true, 0);

	replace = gtk_dialog_get_widget_for_response((GtkDialog*)findReplaceDialog, 100);

	gtk_widget_show(findBox);
	gtk_widget_show(replaceBox);
	gtk_widget_show(vbox);
	gtk_widget_show(hbox);

// TODO
//	gtk_signal_connect_object(GTK_OBJECT(findReplaceDialog),"delete_event",GTK_SIGNAL_FUNC(gtk_widget_hide),GTK_OBJECT(findReplaceDialog));
//	g_signal_connect(G_OBJECT(findReplaceDialog),"delete_event",GTK_SIGNAL_FUNC(gtk_true),NULL);
}

#ifdef _ASPELL_
void buildWordCheck(int documentCheck) {
#if 1
	GtkWidget* vbox;
	GtkWidget* button;
	GtkWidget* hbox;
	GtkWidget* image;

	char* labeltext[512];
	int   docflag = documentCheck;

	spellCheckWord = gtk_dialog_new();
	gtk_window_set_title((GtkWindow*)spellCheckWord, _("Spell check word"));

	vbox = creatNewBox(NEWVBOX, true, 8);
	hbox = creatNewBox(NEWHBOX, true, 8);

	sprintf((char*)&labeltext, _("Change <i><b>%s</b></i> to: "), badWord);
	badWordLabel = gtk_label_new((char*)&labeltext);
	gtk_label_set_use_markup((GtkLabel*)badWordLabel, true);
	gtk_box_pack_start(GTK_BOX(hbox), badWordLabel, true, true, 0);

	wordListDropbox = gtk_combo_box_text_new();
	gtk_box_pack_start(GTK_BOX(hbox), wordListDropbox, true, true, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, true, true, 0);

	hbox = creatNewBox(NEWHBOX, true, 8);

	button = gtk_button_new_from_stock(GTK_STOCK_APPLY);

	gtk_box_pack_start(GTK_BOX(hbox), button, true, true, 0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(doChangeWord), (gpointer)(long)docflag);

	button = gtk_button_new_with_label(_("Ignore"));

	gtk_box_pack_start(GTK_BOX(hbox), button, true, true, 0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(doAddIgnoreWord), (gpointer)1);

	button = gtk_button_new_from_stock(GTK_STOCK_ADD);

	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(doAddIgnoreWord), (gpointer)2);

	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);

	gtk_box_pack_start(GTK_BOX(hbox), button, true, true, 0);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(doCancelCheck), NULL);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, true, true, 0);

	GtkWidget* content = gtk_dialog_get_content_area((GtkDialog *)spellCheckWord);
	gtk_container_add(GTK_CONTAINER(content), (GtkWidget*)vbox);

#endif // 1
}
#endif // _ASPELL_
