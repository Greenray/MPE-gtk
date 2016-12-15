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

#ifndef _FILES_
#define _FILES_

void resetAllFilePrefs(void);
void saveFile(GtkWidget* widget, gpointer data);
void setFilePrefs(GtkSourceView* sourceview);
void newSection(GtkWidget* widget, gpointer data);
void newManpage(GtkWidget* widget, gpointer data);
void saveAs(GtkWidget* widget, gpointer data);
void makeDirty(GtkWidget* widget, gpointer data);
void deleteSection(GtkWidget* widget, gpointer data);
void renameSection(GtkWidget* widget, gpointer data);
void openManpage(GtkWidget* widget, gpointer data);
void doOpenManpage(char* file);
void openFile(char* file);
void resetAllItalicTags(void);

#endif // _FILES_
