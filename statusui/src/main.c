/*
 *  Status monitor for Canon CAPT Printer.
 *  Copyright (C) 2004-2013 CANON INC.
 *  All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "dialog.h"
#include "uimain.h"
#include "widgets.h"
#include "data_process.h"

#define POLLING_COUNT	500
#define REQUEST_COUNT	1000


int LaunchOption(int argc, char *argv[], char *printer_name)
{
	if(argc == 3){
		if(strcmp(argv[1], "-P") == 0){
			if(argv[2] != NULL)
				strncpy(printer_name, argv[2], 127);
			return 1;
		}
	}else if(argc == 4){
		if(strcmp(argv[1], "-P") == 0){
			if(argv[2] != NULL)
				strncpy(printer_name, argv[2], 127);
			if(strcmp(argv[3], "-e") == 0)
				return 0;
			printf("*** WARNING *** Unknown switch, %s\n", argv[3]);
			return 1;
		}
	}
	printf("captstatusui --- Status monitor for Canon CAPT Printer.\n");
	printf("Usage  :  captstatusui -P printer [-e]\n");
	printf(" -e	\"Status Monitor is showed only when errors occur\".\n\n"); 
	return -1;
}

int StartProcess(mode)
{
	gint PollingFlag = 0;
	gint RequestFlag = 0;

	SigDisable();
	UpdateWidgets(g_status_window);
	SigEnable();

	if(mode)
		gtk_widget_show(UI_DIALOG(g_status_window)->window);

	PollingFlag = gtk_timeout_add(POLLING_COUNT, (GtkFunction)DataProcess, NULL);
	RequestFlag = gtk_timeout_add(REQUEST_COUNT, (GtkFunction)StatusRequest, NULL);
	gtk_main ();

	if(PollingFlag)
		gtk_timeout_remove(PollingFlag);
	if(RequestFlag)
		gtk_timeout_remove(RequestFlag);

	return 0;
}

int StartProcess2(mode)
{
	gint PollingFlag = 0;
	gint RequestFlag = 0;

	SigDisable();
	UpdateWidgets2(g_status_window);
	SigEnable();

	if(mode)
		gtk_widget_show(UI_DIALOG(g_status_window)->window);

	PollingFlag = gtk_timeout_add(POLLING_COUNT, (GtkFunction)DataProcess2, NULL);
	RequestFlag = gtk_timeout_add(REQUEST_COUNT, (GtkFunction)StatusRequest, NULL);
	gtk_main ();

	if(PollingFlag)
		gtk_timeout_remove(PollingFlag);
	if(RequestFlag)
		gtk_timeout_remove(RequestFlag);

	return 0;
}

int
main (int argc, char *argv[])
{
	char printer_name[128];
	int mode = 0;

#ifdef ENABLE_NLS
	bindtextdomain(PACKAGE, PACKAGE_LOCALE_DIR);
#ifndef OLD_GTK
	/* changes for GTK2 */
	bind_textdomain_codeset(PACKAGE, "UTF-8");
#endif
	textdomain(PACKAGE);
#endif
	gtk_set_locale ();
	gtk_init (&argc, &argv);

	add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
#ifdef OLD_GTK
	/* changes for GTK2 */
	/* If it is necessary, "PACKAGE_SOURCE_DIR" is defined. */
	/* Example: add to configure.in                                */
	/*  dnl Set PACKAGE_SOURCE_DIR in config.h.                    */
	/*  packagesrcdir=`cd $srcdir && pwd`                          */
	/*  AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}") */
	add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");
#endif

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
	memset(printer_name, 0, sizeof(printer_name));
	mode = LaunchOption(argc, argv, printer_name);
	if(mode != -1){
UI_DEBUG("[%s]\n", printer_name);
		SigInit();
		g_status_window = CreateStatusWnd(printer_name, mode);
		if(g_status_window == NULL){
UI_DEBUG("Window Create Error\n");
			return -1;
		}
		gtk_widget_realize(UI_DIALOG(g_status_window)->window);

		switch(g_status_window->nIFVersion){
		case CCPD_IF_VERSION_110:
			StartProcess2(mode);
			break;
		case CCPD_IF_VERSION_100:
			StartProcess(mode);
			break;
		default:
			break;
		}

		DisposeDialog(UI_DIALOG(g_status_window));
	}
UI_DEBUG("Finish\n");

	return 0;
}
