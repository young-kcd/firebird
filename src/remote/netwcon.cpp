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
#include <nwsnut.h>
#include <time.h>
#include <signal.h>
#include <process.h>
#include <conio.h>

#define ALLOC_SIGNATURE 0x54524C41
#define DB_LINE       3
#define DB_COLUMN     0
#define DB_HEIGHT     11
#define DB_WIDTH      80
#define DB_VHEIGHT    11
#define DB_VWIDTH     80

#define HELP_BOX				    0x0000
#define UTILITY_MSG				 0x0000
#define VERSION_40				 0x0001
#define CONFIRM_HEADER  		 0x0002

#define SCREEN_SAVER_TIMEOUT 60000

SCHAR *hdr[] =
	{ "Interbase Development v4.0 (c) 1993 - Borland International, Inc.",
	"Information for InterBase"
};

SCHAR *message_table[] = {
	"InterBase Server Dev (c) 1993",
	"v4.0",
	"Available Options"
};

SCHAR help[500];

ULONG nlm_handle;
NUTInfo *handle;
ULONG alloc_tag;

ULONG header_portal_number;
PCB *header_portal_ptr;

int screen_id;
WORD screen_height;
WORD screen_width;

ULONG saver_column = 0;
ULONG saver_portal_number;
PCB *saver_portal_ptr;
ULONG saver_line = 0;
SCHAR screen_saver_on = 0;

PCB *main_portal_ptr;
ULONG main_portal_number;

SCHAR console_active = 0;;
SCHAR console_exit = 0;
SCHAR menu_active = 0;

void main(void);
void main_menu(void);
int main_action(int option, void *junk);
void console_processor(void);
void menu_processor(void);
void remove_screen_saver(void);
void screen_saver(void);
void update_screen_saver(void);
void terminate_console(int sig);

void main()
{
	ULONG pal;

	nlm_handle = GetNLMHandle();
	screen_id = GetCurrentScreen();
	alloc_tag = AllocateResourceTag(nlm_handle, "InterBase Monitor",
									ALLOC_SIGNATURE);

	NWSInitializeNut(UTILITY_MSG, VERSION_40, NO_HEADER,
					 NUT_REVISION_LEVEL, &message_table,
					 help, screen_id, alloc_tag, &handle);

	pal = NWSGetScreenPalette(handle);

	NWSSetScreenPalette(2, handle);

	header_portal_number = NWSCreatePortal(0, 0,
										   3, 80, 3,
										   80, SAVE, 0, 0,
										   SINGLE, VREVERSE, CURSOR_OFF,
										   VIRTUAL, handle);

	NWSGetPCB(&header_portal_ptr, header_portal_number, handle);
	NWSClearPortal(header_portal_ptr);
	NWSFillPortalZoneAttribute(0, 0, 2, 80, VREVERSE, header_portal_ptr);
	NWSDisplayTextJustifiedInPortal(JCENTER, 0, 40, 0, hdr[0],
									VREVERSE, header_portal_ptr);
	NWSUpdatePortal(header_portal_ptr);
	NWSFillPortalZoneAttribute(0, 0, 2, 80, VREVERSE, header_portal_ptr);


	NWSUpdatePortal(header_portal_ptr);
	NWSSetScreenPalette(pal, handle);
	GetSizeOfScreen(&screen_height, &screen_width);

	main_portal_number =
		NWSCreatePortal(DB_LINE, DB_COLUMN, DB_HEIGHT, DB_WIDTH, DB_VHEIGHT,
						DB_VWIDTH, SAVE, hdr[1], 0, SINGLE, 0, CURSOR_OFF,
						VIRTUAL, handle);

	NWSGetPCB(&(main_portal_ptr), main_portal_number, handle);
	NWSUpdatePortal(main_portal_ptr);

	signal(SIGTERM, terminate_console);
	BeginThread((void *) console_processor, 0, 0, 0);
	BeginThread((void *) menu_processor, 0, 0, 0);
	ExitThread(TSR_THREAD, 0);
	ExitThread(EXIT_NLM, 1);
}

void main_menu()
{
	NWSInitMenu(handle);
	NWSSetDynamicMessage(DYNAMIC_MESSAGE_ONE, "Users", &handle->messages);
	NWSAppendToMenu(DYNAMIC_MESSAGE_ONE, 1, handle);

	NWSSetDynamicMessage(DYNAMIC_MESSAGE_TWO, "Databases", &handle->messages);
	NWSAppendToMenu(DYNAMIC_MESSAGE_TWO, 1, handle);

	NWSSetDynamicMessage(DYNAMIC_MESSAGE_THREE, "Resources",
						 &handle->messages);
	NWSAppendToMenu(DYNAMIC_MESSAGE_THREE, 1, handle);

	NWSMenu(2, 18, 40, 0, main_action, handle, (void *) handle);
}

int main_action(int option, void *junk)
{
	switch (option) {
	case 1:
		RingTheBell();
		RingTheBell();
		break;
	case 2:
		RingTheBell();
		RingTheBell();
		RingTheBell();
		break;
	case 3:
		RingTheBell();
		RingTheBell();
		RingTheBell();
		RingTheBell();
		break;
	default:
		return (0);
	}
	return (-1);
}

void terminate_console(int sig)
{
	console_exit = 1;

	ConsolePrintf("1 ");
	SetCurrentScreen(screen_id);
	ConsolePrintf("2 ");
	ungetch(27);
	ConsolePrintf("3 ");
	while (menu_active)
		ThreadSwitch();
	NWSDestroyMenu(handle);

	if (screen_saver_on)
		remove_screen_saver();

	clrscr();

	NWSDestroyPortal(header_portal_number, handle);

	NWSDestroyPortal(main_portal_number, handle);

	NWSRestoreNut(handle);

	ConsolePrintf("4 ");
	while (console_active)
		ThreadSwitch();
	ConsolePrintf("5 ");
}

void menu_processor()
{
	menu_active = 1;
	ConsolePrintf("b4 main menu\n");
	main_menu();
	ConsolePrintf("after main menu\n");
	while (!console_exit) {
		ThreadSwitch();
	}
	menu_active = 0;
	ConsolePrintf("exit main menu\n");
}

void console_processor()
{
	clock_t seconds;
	clock_t prev_clock;
	clock_t cur_clock;

	prev_clock = clock();
	seconds = 0;

	console_active = 1;
	while (!console_exit) {
		ThreadSwitch();
		cur_clock = clock();
		seconds = cur_clock - prev_clock;

		if ((seconds > SCREEN_SAVER_TIMEOUT) && (!screen_saver_on)) {
			screen_saver_on = 1;
			screen_saver();
		}

		if (screen_saver_on)
			update_screen_saver();
	}

	console_active = 0;
}

void remove_screen_saver()
{
	NWSDestroyPortal(saver_portal_number, handle);
}

void screen_saver()
{
	int pal;

	saver_portal_number = NWSCreatePortal(0, 0,
										  25, 80, 25,
										  80, SAVE, 0, 0,
										  NOBORDER, VNORMAL, CURSOR_OFF,
										  VIRTUAL, handle);
	NWSGetPCB(&saver_portal_ptr, saver_portal_number, handle);
	saver_line = 0;
	saver_column = 0;
}

void update_screen_saver()
{
	int pal;

	delay(200);

	if (saver_line == 21)
		saver_line = 0;

	if (saver_column == 80)
		saver_column = 0;

	NWSClearPortal(saver_portal_ptr);

	pal = NWSGetScreenPalette(handle);
	NWSSetScreenPalette(0, handle);
	NWSFillPortalZoneAttribute(0, 0, 25, 80, VNORMAL, saver_portal_ptr);
	NWSSetScreenPalette(8, handle);
	NWSDisplayTextInPortal(saver_line++, saver_column++, "InterBase",
						   VREVERSE, saver_portal_ptr);
	NWSUpdatePortal(saver_portal_ptr);
	NWSSetScreenPalette(pal, handle);
}
