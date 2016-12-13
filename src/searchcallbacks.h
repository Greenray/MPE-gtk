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

#ifndef _FUNCCALLBACKS_
#define _FUNCCALLBACKS_

#include "globals.h"
#include "files.h"

void doFindReplace(GtkDialog *dialog, gint response_id, gpointer user_data);
void find(GtkWidget* widget, gpointer data);
void doSearchPrefs(GtkWidget* widget, gpointer data);
void docSearchFromBar(GtkWidget* widget, gpointer data);
void doLiveSearch(GtkWidget* widget,GdkEvent *event, gpointer data);

#endif // _FUNCCALLBACKS_
