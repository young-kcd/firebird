/*
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */
#include <curses.h>
#include "../jrd/ib_stdio.h"
#include <string.h>

SCHAR get_line();

main(argc, argv)
	 int argc;
	 SCHAR *argv[];
{
	int i;
	SSHORT x, y, sw_read;
	FILE *log_file;
	SCHAR log_file_name[128], line[128], *command, *s;

	sw_read = 0;
	if (argc > 1)
		strcpy(log_file_name, argv[1]);
	else {
		fprintf(stderr, "Must give log file name.\n");
		exit(1);
	}
	if (argc > 2) {
		s = argv[2];
		if ((*s++ == '-') && ((*s == 'r') || (*s == 'R')))
			sw_read = 1;
		else {
			fprintf(stderr, "unrecognized switch");
			exit(1);
		}
	}
	if (!(log_file = fopen(log_file_name, "r"))) {
		fprintf(stderr, "Error opening log file\n");
		exit(1);
	}

	while (get_line(log_file, line) != EOF) {
		command = strtok(line, " ");
		if (!strcmp(command, "initscr"))
			initscr();
		else if (!strcmp(command, "endwin"))
			endwin();
		else if (!strcmp(command, "saveterm")) {
			saveterm();
		}
		else if (!strcmp(command, "resetterm")) {
			resetterm();
		}
		else if (!strcmp(command, "fixterm")) {
			fixterm();
		}
		else if (!strcmp(command, "clear"))
			clear();
		else if (!strcmp(command, "echo"))
			echo();
		else if (!strcmp(command, "noecho"))
			noecho();
		else if (!strcmp(command, "crmode"))
			crmode();
		else if (!strcmp(command, "nocrmode"))
			nocrmode();
		else if (!strcmp(command, "keypad")) {
			if (!strcmp(strtok(NULL, " "), "TRUE"))
				keypad(stdscr, TRUE);
			else
				keypad(stdscr, FALSE);
		}
		else if (!strcmp(command, "wrefresh"))
			wrefresh(stdscr);
		else if (!strcmp(command, "wstandend"))
			wstandend(stdscr);
		else if (!strcmp(command, "wstandout"))
			wstandout(stdscr);
		else if (!strcmp(command, "wmove")) {
			y = atoi(strtok(NULL, " "));
			x = atoi(strtok(NULL, " "));
			wmove(stdscr, y, x);
		}
		else if (!strcmp(command, "mvwaddstr")) {
			command = strtok(NULL, " ");
			y = atoi(command);
			command = strtok(NULL, " ");
			x = atoi(command);
			command += strlen(command) + 1;
			mvwaddstr(stdscr, y, x, command);
		}
		else if (!strcmp(command, "input")) {
			printf("enter something: ");
			get_line(stdin, line);
		}
		else if (!strcmp(command, "read")) {
			if (sw_read) {
				i = wgetch(stdscr);
				printf("read %d\n", i);
			}
			else
				sleep(5);
		}
		else
			fprintf(stderr, "unknown command %s\n", command);
	}
	fclose(log_file);
}

SCHAR get_line(f, s)
	 FILE *f;
	 SCHAR *s;
{
	SCHAR c;

	while (((c = getc(f)) != '\n') && (c != EOF))
		*s++ = c;

	*s = 0;

	return c;
}
