/*************************************************************************
**
**	PROGRAM:		JRD file split utility program
**	MODULE:			spit.c
**	DESCRIPTION:	Command line interpreter for backup file split/join
**					utility program
**
**
**************************************************************************
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

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include <stdarg.h>

#include <fcntl.h>
#include <ctype.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include "../burp/spit.h"
#include "../burp/burpswi.h"
#include "../burp/std_desc.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN_NT
// usage of non-ANSI open/read/write/close functions
#include <io.h>
#endif

#define	MODE_READ	O_RDONLY

#define	MODE_WRITE	O_WRONLY | O_CREAT
#define	MASK		0666

/* Definitions for GSPLIT */
#define	IN_SW_SPIT_0	0   /* the unknowable switch */
#define	IN_SW_SPIT_SP	30  /* splits back up files */
#define	IN_SW_SPIT_JT	31  /* joins back up files */


static in_sw_tab_t spit_in_sw_table [] =
{
	{IN_SW_SPIT_SP,	0,	"SPLIT_BK_FILE", 0, 0, 0, FALSE, 0, 0, NULL},
	{IN_SW_SPIT_JT,	0,	"JOIN_BK_FILE",	 0, 0, 0, FALSE, 0, 0, NULL},
	{IN_SW_SPIT_0,	0,	NULL,			 0, 0, 0, FALSE, 0, 0, NULL}
};

/*************************************
** backup files header record
**************************************
*/

typedef struct header_rec {
	TEXT name[18];
	TEXT date_time[30];
	TEXT text1[11];
	TEXT num[4];
	TEXT text2[4];
	TEXT total[4];
	TEXT text3[2];
	TEXT fl_name[27];
} HEADER_REC;

#define	HEADER_REC_LEN			sizeof(header_rec)
#define	HEADER_REC_NAME			"InterBase/gsplit, "

/*************************************
** backup files structure
**************************************
*/

typedef struct b_fil {
	b_fil *b_fil_next;
	TEXT *b_fil_name;
	SLONG b_fil_number;
	double b_fil_size;
} *B_FIL;

#define	B_FIL_LEN	sizeof(b_fil)


	/*****************************************************
	**
	** local function declarations
	**
	******************************************************
	*/

static int conv_ntoc(SLONG, TEXT *);

static int free_file_list(B_FIL);

static int final_flush_io_buff(UCHAR *, SLONG, FILE_DESC);

static int final_read_and_write(FILE_DESC, FILE_DESC,
								TEXT *, SLONG, UCHAR **, bool *);

static int flush_io_buff(UCHAR *, SLONG,
						 FILE_DESC, double, SLONG *, bool *);

static int get_file_name(SCHAR *, double, B_FIL *);

static int get_file_size(SCHAR *, SCHAR *, double *);

static int get_function_option(SCHAR *, USHORT *, SCHAR *, IN_SW_TAB);

static int gen_multy_bakup_files(B_FIL, FILE_DESC, SLONG);

static int set_hdr_str(TEXT *, TEXT *, SLONG, SLONG);

static int join_multy_bakup_files(B_FIL);

static int print_clo(TEXT *);

static int read_and_write(FILE_DESC, FILE_DESC,
						  TEXT *, SLONG,
						  double, UCHAR **, bool *, double *, SLONG *);

static int read_and_write_for_join(FILE_DESC, TEXT *,
								   UCHAR **, SLONG, SLONG *);

static int write_header(B_FIL, HEADER_REC, FILE_DESC, TEXT *);



/*****************************************************
**
**	M A I N              P R O G R A M
**
******************************************************
*/

int main( int argc, char *argv[])
{

	SCHAR **end, *prog_name, *string;
	IN_SW_TAB in_sw_tab;
	USHORT sw_replace;
	B_FIL file_ptr;
	B_FIL file_list;
	B_FIL prev_file;
	bool file_nm_sw = false;
	SLONG ret_cd;
	SLONG file_num = 0;
	double file_size;
	FILE_DESC input_file_desc;

	prog_name = argv[0];

	if (argc < 2) {
		ib_fprintf(ib_stderr, "%s: No Command Line Option Specified\n",
				   argv[0]);
		ret_cd = print_clo(prog_name);
		return 1;
	}

/************************
** Fields initialization
*************************
*/

	file_ptr = file_list = prev_file = NULL;
	file_size = -1;
	sw_replace = 0;
	end = argv + argc;
	++argv;

/*******************************
 ** Initialize in_sw_table table.
 *******************************
*/

	for (in_sw_tab = spit_in_sw_table; in_sw_tab->in_sw_name; in_sw_tab++)
		in_sw_tab->in_sw_state = FALSE;

	/**********************************
	** validating command line options
	***********************************
	*/

	while (argv < end) {
		string = *argv;
		if (*string == '-') {
			argv++;
			ret_cd = get_function_option(prog_name, &sw_replace,
										 string, spit_in_sw_table);
			if (ret_cd == FB_FAILURE) {
				ret_cd = free_file_list(file_list);
				return FB_FAILURE;
			}
		}						// end of processing (*string == '-')
		else {					// processing function specific command line options

			switch (sw_replace) {
			case (IN_SW_SPIT_SP):
				if (file_nm_sw == false) {	// process file name
					file_size = 0;
					file_num = file_num + 1;

					if (file_num > MAX_NUM_OF_FILES) {
						ib_fprintf(ib_stderr,
								   "%s: maximum of files is %d\n",
								   prog_name, MAX_NUM_OF_FILES);
						ret_cd = print_clo(prog_name);
						ret_cd = free_file_list(file_list);
						return FB_FAILURE;
					}

					if (strlen(string) > MAX_FILE_NM_LEN) {
						ib_fprintf(ib_stderr,
								   "%s: file name %s is too long\n",
								   prog_name, string);
						ib_fprintf(ib_stderr,
								   "%s: maximum length of file name is %d bytes\n",
								   prog_name, MAX_FILE_NM_LEN);
						ret_cd = print_clo(prog_name);
						ret_cd = free_file_list(file_list);
						return FB_FAILURE;
					}

					ret_cd = get_file_name(string, file_size, &file_ptr);

					if (ret_cd == FB_FAILURE) {
						ret_cd = free_file_list(file_list);
						return FB_FAILURE;
					}

					file_nm_sw = true;
					file_ptr->b_fil_number = file_num;

					if (file_list == NULL)
						file_list = prev_file = file_ptr;
					else {
						prev_file->b_fil_next = file_ptr;
						prev_file = file_ptr;
					}

				}				/* processing file name */
				else {			/* processing file size */

					file_nm_sw = false;
					ret_cd = get_file_size(prog_name, string, &file_size);
					if (ret_cd == FB_FAILURE) {
						ret_cd = free_file_list(file_list);
						return FB_FAILURE;
					}
				}				/* end of processing file size specification */
				file_ptr->b_fil_size = file_size;
				break;

			case (IN_SW_SPIT_JT):
				ret_cd = get_file_name(string, file_size, &file_ptr);

				if (ret_cd == FB_FAILURE) {
					ret_cd = free_file_list(file_list);
					return FB_FAILURE;
				}

				file_num = file_num + 1;
				file_ptr->b_fil_number = file_num;

				if (file_list == NULL)
					file_list = prev_file = file_ptr;
				else {
					prev_file->b_fil_next = file_ptr;
					prev_file = file_ptr;
				}				/* end of processing file size specification */
				break;

			default:
				ib_fprintf(ib_stderr, "%s: invalid option '%s'\n",
						   prog_name, string);
				ret_cd = print_clo(prog_name);
				ret_cd = free_file_list(file_list);
				return FB_FAILURE;
				break;
			}					/* and of switch (sw_replace) */

			argv++;
		}						/* processing function specific command line options */
	}							/* while (argv < end) */

	if ((file_list == NULL) && (sw_replace)) {
		ib_fprintf(ib_stderr,
				   "%s: invalid option '%s', rest of parameters is missing\n",
				   prog_name, string);
		ret_cd = print_clo(prog_name);
		ret_cd = free_file_list(file_list);
		return FB_FAILURE;
	}

	switch (sw_replace) {
	case (IN_SW_SPIT_SP):
		input_file_desc = reinterpret_cast<FILE_DESC>(GBAK_STDIN_DESC);
		ret_cd = gen_multy_bakup_files(file_list, input_file_desc, file_num);
		if (ret_cd == FB_FAILURE) {
			ib_fprintf(ib_stderr,
					   "%s: progam fails to generate multi-volumn back-up files\n",
					   prog_name);
			ret_cd = free_file_list(file_list);
			return FB_FAILURE;
		}
		break;

	case (IN_SW_SPIT_JT):
		ret_cd = join_multy_bakup_files(file_list);
		if (ret_cd == FB_FAILURE) {
			ib_fprintf(ib_stderr,
					   "%s: progam fails to join multi-volumn back-up files\n",
					   prog_name);
			ret_cd = free_file_list(file_list);
			return FB_FAILURE;
		}
		break;

	default:
		break;
	}

/********************************************************
**
**	free all the storage allocated for backup files
**
*********************************************************
*/

	ret_cd = free_file_list(file_list);
	return FB_SUCCESS;

}								/* end of man() */


static int get_function_option(SCHAR* prog_name,
							   USHORT * sw_replace,
							   SCHAR* string,
							   IN_SW_TAB in_sw_table)
{
/********************************************************************
**
**	g e t _ f u n c t i o n _ o p t i o n
**
*********************************************************************
**
**	Functional description:
**
**		processing command line option and set proper function switch
**
*********************************************************************
*/

	SCHAR c, *p, *q;
	IN_SW_TAB in_sw_tab;
	SLONG ret_cd;

	if (strlen(string) == 1) {
		ib_fprintf(ib_stderr, "%s: invalid option '%s'\n", prog_name, string);
		ret_cd = print_clo(prog_name);
		return FB_FAILURE;
	}

	for (in_sw_tab = in_sw_table; q = in_sw_tab->in_sw_name; in_sw_tab++) {
		for (p = string + 1; c = *p++;)
			if (UPPER(c) != *q++)
				break;
		if (!c) {
			if (*sw_replace == 0) {
				*sw_replace = (in_sw_tab->in_sw);
				return FB_SUCCESS;
			}
			else {
				if (*sw_replace != in_sw_tab->in_sw) {
					ib_fprintf(ib_stderr,
							   "%s: invalid option '%s', incompatible option\n",
							   prog_name, string);
					ret_cd = print_clo(prog_name);
					return FB_FAILURE;
				}
				else			/* compatible option */
					break;
			}
		}						/* end of if (!c) */
	}							/* end of for loop */

	if (!in_sw_tab->in_sw) {
		ib_fprintf(ib_stderr, "%s: invalid option '%s'\n", prog_name, string);
		ret_cd = print_clo(prog_name);
		return FB_FAILURE;
	}

	return FB_SUCCESS;
}


static int get_file_size(SCHAR * prog_name, SCHAR * string, double *file_size)
{
/********************************************************************
**
**	g e t _ f i l e _ s i z e
**
*********************************************************************
**
**	Functional description:
**
**		processing file size specification and returns calculated
**		file size.
**
*********************************************************************
*/

	SCHAR c, *p;
	SLONG size_indicator;
	SLONG ret_cd;

	size_indicator = 0;

	for (p = string; c = *p++;) {
		if (c == '\0')
			break;

		if (isdigit(c))
			*file_size = *file_size * 10 + (c - '0');
		else {
			if ((isalpha(c)) &&
				(UPPER(c) == 'G' || UPPER(c) == 'K' || UPPER(c) == 'M')) {
				switch (UPPER(c)) {
				case ('K'):
					size_indicator = K_BYTES;
					break;

				case ('M'):
					size_indicator = M_BYTES;
					break;

				case ('G'):
					size_indicator = G_BYTES;
					break;

				default:
					break;
				}				/* end of switch( UPPER (c) ) */

				*file_size = *file_size * size_indicator;
			}
			else {				/* invalid size indicator */

				ib_fprintf(ib_stderr,
						   "%s: invalid size indicator '%s'\n", prog_name,
						   string);
				ret_cd = print_clo(prog_name);
				return FB_FAILURE;
			}
		}
	}
	if (*file_size < MIN_FILE_SIZE) {	/* handling user specifies file size 0 */
		ib_fprintf(ib_stderr,
				   "%s: invalid option '%s', minimum file size is 1 megabyte\n",
				   prog_name, string);
		ret_cd = print_clo(prog_name);
		return FB_FAILURE;
	}

	return FB_SUCCESS;
}


static int get_file_name( SCHAR * string, double file_size, B_FIL * file_ptr)
{
/********************************************************************
**
**	g e t _ f i l e _ n a m e
**
*********************************************************************
**
**	Functional description:
**
**		processing file name specification and returns file structure
**		pointer.
**
*********************************************************************
*/

	B_FIL temp_ptr;

	*file_ptr = temp_ptr = (B_FIL) malloc(B_FIL_LEN);
	temp_ptr->b_fil_name = (TEXT *) malloc(strlen(string) + 1);

	temp_ptr->b_fil_next = NULL;
	strcpy(temp_ptr->b_fil_name, string);
	temp_ptr->b_fil_number = 0;
	temp_ptr->b_fil_size = file_size;

	return FB_SUCCESS;
}


static int gen_multy_bakup_files(B_FIL file_list,
								 FILE_DESC input_file_desc, SLONG file_num)
{
/********************************************************************
**
**	g e n _ m u l t y _ b a c k u p _ f i l e s
**
*********************************************************************
**
**	Functional description:
**
**		processing input data from ib_stdin and splits the data into
**		multiple back-up files.
**
**		allocates an 16K bytes I/O buffer
**		intilializes header record common fields
**		do forever
**			walk through the backup file chain
**			intilializes header record unique fields
**			open backup file
**			writes out header record to backup file
**			points to the next backup file in the chain
**			calculates the actual file size ( minus header record length )
**			if the actual file size less than 16K bytes
**				set I/O size to actual file size
**			otherwise
**				set I/O size to 16K byte long
**			when it is the last backup file
**				reads data from standard input as much as indicated by I/O size
**					and writes it out to the last backup file until no EOF.
**				issues error message when disk space full condition is detected
**			otherwise reads and writes to backup files util EOF
**				if disk full cobdition is detected
**					flush the remaining data in the I/O buffer to subsequence
**							backup files
**				go back to normal read and write process util EOF
**
*********************************************************************
*/

	FILE_DESC output_fl_desc;
	SLONG byte_write, clock, indx, io_size, remaining_io_len, ret_cd, pos;
	B_FIL fl_ptr = file_list;
	bool end_of_input = false;
	bool flush_done = false;
	TEXT *file_name, header_str[HEADER_REC_LEN], num_arr[5];
	UCHAR *remaining_io;
	HEADER_REC hdr_rec;
	double byte_read, file_size;
	UCHAR *io_buffer = (UCHAR *) malloc(IO_BUFFER_SIZE);


	if (io_buffer == 0) {
		ib_fprintf(ib_stderr, "I/O buffer allocation failed\n");
		return FB_FAILURE;
	}

	for (pos = 0; pos < HEADER_REC_LEN; pos++)
		header_str[pos] = BLANK;

	pos = 0;
	ret_cd = set_hdr_str(header_str, HEADER_REC_NAME,
						 pos, sizeof(hdr_rec.name));
	for (indx = 0; indx < sizeof(hdr_rec.name); indx++)
		hdr_rec.name[indx] = BLANK;

	pos = pos + sizeof(hdr_rec.name);
	clock = time(0);
	ret_cd = set_hdr_str(header_str, ctime(&clock),
						 pos, sizeof(hdr_rec.date_time));
	for (indx = 0; indx < sizeof(hdr_rec.date_time); indx++)
		hdr_rec.date_time[indx] = BLANK;

	pos = pos + sizeof(hdr_rec.date_time);
	ret_cd = set_hdr_str(header_str, ", file No. ",
						 pos, sizeof(hdr_rec.text1));
	for (indx = 0; indx < sizeof(hdr_rec.text1); indx++)
		hdr_rec.text1[indx] = BLANK;

	for (indx = 0; indx < sizeof(hdr_rec.num); indx++)
		hdr_rec.num[indx] = BLANK;

	pos = pos + sizeof(hdr_rec.text1) + sizeof(hdr_rec.num);
	ret_cd = set_hdr_str(header_str, " of ", pos, sizeof(hdr_rec.text2));
	for (indx = 0; indx < sizeof(hdr_rec.text2); indx++)
		hdr_rec.text2[indx] = BLANK;

	ret_cd = conv_ntoc(file_num, num_arr);
	if (ret_cd == FB_FAILURE) {
		free(io_buffer);
		ib_fprintf(ib_stderr,
				   "gsplit could not convert numeric data to character data\n");
		return FB_FAILURE;
	}
	num_arr[sizeof(num_arr) - 1] = TERMINAL;
	pos = pos + sizeof(hdr_rec.text2);
	ret_cd = set_hdr_str(header_str, num_arr, pos, sizeof(hdr_rec.total));
	for (indx = 0; indx < sizeof(hdr_rec.total); indx++)
		hdr_rec.total[indx] = BLANK;

	pos = pos + sizeof(hdr_rec.total);
	ret_cd = set_hdr_str(header_str, ", ", pos, sizeof(hdr_rec.text3));
	for (indx = 0; indx < sizeof(hdr_rec.text3); indx++)
		hdr_rec.text3[indx] = BLANK;

	for (indx = 0; indx < sizeof(hdr_rec.fl_name); indx++)
		hdr_rec.fl_name[indx] = BLANK;

	while (true) {
		if (fl_ptr != NULL) {
			byte_read = 0;
			byte_write = 0;
			if ((fl_ptr->b_fil_next == NULL)
				&& (fl_ptr->b_fil_size == 0)) fl_ptr->b_fil_size =
					MIN_FILE_SIZE;

			file_size = fl_ptr->b_fil_size - HEADER_REC_LEN;
			file_name = fl_ptr->b_fil_name;

			output_fl_desc = open(file_name, MODE_WRITE, MASK);
			if (output_fl_desc == -1) {
				free(io_buffer);
				ib_fprintf(ib_stderr, "can not open back up file %s\n",
						   file_name);
				return FB_FAILURE;
			}

			ret_cd =
				write_header(fl_ptr, hdr_rec, output_fl_desc, header_str);
			if (ret_cd == FB_FAILURE) {
				free(io_buffer);
				ib_fprintf(ib_stderr,
						   "could not write header record to file %s\n",
						   file_name);
				return FB_FAILURE;
			}

			fl_ptr = fl_ptr->b_fil_next;
		}

		if (file_size < IO_BUFFER_SIZE)
			io_size = (SLONG) file_size;
		else
			io_size = IO_BUFFER_SIZE;

		if (fl_ptr == NULL) {
			while (end_of_input == false) {
				ret_cd = final_read_and_write(input_file_desc, output_fl_desc,
											  file_name, io_size, &io_buffer,
											  &end_of_input);
				if (ret_cd == FB_FAILURE) {
					free(io_buffer);
					return FB_FAILURE;
				}

				if (end_of_input == true) {
					free(io_buffer);
					return FB_SUCCESS;
				}
			}
		}
		else {
			while ((file_size > byte_read) && (fl_ptr != NULL)) {
				ret_cd = read_and_write(input_file_desc, output_fl_desc,
										file_name, io_size, file_size,
										&io_buffer, &end_of_input,
										&byte_read, &byte_write);
				switch (ret_cd) {
				case (FB_FAILURE):
					{
						free(io_buffer);
						return FB_FAILURE;
						break;
					}

				case (FILE_IS_FULL):
					{
						byte_read = 0;	/* reset byte read count,
										   ** prepare for next read 
										 */
						remaining_io = io_buffer + byte_write;
						remaining_io_len = IO_BUFFER_SIZE - byte_write;

						while ((flush_done == false)
							   && (fl_ptr != NULL)) {
							if ((fl_ptr->b_fil_next == NULL)
								&& (fl_ptr->b_fil_size == 0))
								fl_ptr->b_fil_size = MIN_FILE_SIZE;

							file_size = fl_ptr->b_fil_size - HEADER_REC_LEN;
							file_name = fl_ptr->b_fil_name;

							output_fl_desc =
								open(file_name, MODE_WRITE, MASK);
							if (output_fl_desc == -1) {
								free(io_buffer);
								ib_fprintf(ib_stderr,
										   "can not open back up file %s\n",
										   file_name);
								return FB_FAILURE;
							}
							ret_cd = write_header(fl_ptr, hdr_rec,
												  output_fl_desc, header_str);
							if (ret_cd == FB_FAILURE) {
								free(io_buffer);
								ib_fprintf(ib_stderr,
										   "fail to write header rec to file %s\n",
										   file_name);
								return FB_FAILURE;
							}

							fl_ptr = fl_ptr->b_fil_next;
							if (fl_ptr == NULL) {
								ret_cd = final_flush_io_buff(remaining_io,
															 remaining_io_len,
															 output_fl_desc);
								if (ret_cd == FB_FAILURE) {
									ib_fprintf(ib_stderr,
											   "gsplit could not do backup due");
									ib_fprintf(ib_stderr,
											   " to lack of space or I/O problem\n");
									free(io_buffer);
									return FB_FAILURE;
								}
							}
							else {	/* got a lot of backup files */

								ret_cd = flush_io_buff(remaining_io,
													   remaining_io_len,
													   output_fl_desc,
													   file_size,
													   &byte_write,
													   &flush_done);
								if (ret_cd == FB_FAILURE) {
									ib_fprintf(ib_stderr,
											   "gsplit could not do backup due");
									ib_fprintf(ib_stderr, " I/O problem\n");
									free(io_buffer);
									return FB_FAILURE;
								}
								if (flush_done == true) {
									file_size = file_size - byte_write;
									byte_write = 0;
								}
								else {
									remaining_io = remaining_io + byte_write;
									remaining_io_len = remaining_io_len -
										byte_write;
								}
							}
						}		/* end of while loop */
						break;
					}

				default:
					break;
				}

				if (end_of_input == true) {
					free(io_buffer);
					return FB_SUCCESS;
				}
			}
		}
	} // end of while ( true )
}


static int read_and_write(FILE_DESC input_file_desc,
						  FILE_DESC output_fl_desc,
						  TEXT * file_name,
						  SLONG io_size,
						  double file_size,
						  UCHAR ** io_buffer,
						  bool * end_of_input,
						  double *byte_read,
						  SLONG * byte_write)
{

/********************************************************************
**
**	r e a d _ a n d _ w r i t e
**
*********************************************************************
**
**	Functional description:
**
**		Read data from input file and write the data out to output
**		file.
**
*********************************************************************
*/

	SLONG read_cnt, last_read_size, write_cnt;

	/********************************************************
	**	when number of byte read + number of byte goint to
	**	be read is greater then file size, then calculate
	**	the size for the last read and do the last read for
	**	the current backup file. Otherwise read as mush data
	**	as will fit in the current backup file.
	**********************************************************
	*/

	if (*byte_read + io_size > file_size) {
		last_read_size = (SLONG) (file_size - *byte_read);
		read_cnt = read(input_file_desc, *io_buffer, last_read_size);
	}
	else
		read_cnt = read(input_file_desc, *io_buffer, io_size);

	switch (read_cnt) {
	case (0):					/* no more data to be read */
		close(output_fl_desc);
		*end_of_input = true;
		*byte_read = *byte_read + read_cnt;
		return FB_SUCCESS;
		break;

	case (-1):					/* read failed */
		close(output_fl_desc);
		ib_fprintf(ib_stderr,
				   "fail to read input from ib_stdin, errno = %d\n", errno);
		return FB_FAILURE;
		break;

	default:					/* read ok */
		*byte_read = *byte_read + read_cnt;
		break;
	}

	write_cnt = write(output_fl_desc, *io_buffer, read_cnt);

	switch (write_cnt) {
	case (-1):					/* write failed */
		close(output_fl_desc);
		return FB_FAILURE;
		break;

	default:
		if (write_cnt == read_cnt)	/* write ok */
			return FB_SUCCESS;
		else {					/* write less data then it reads in */

			close(output_fl_desc);
			*byte_write = write_cnt;
			return FILE_IS_FULL;
		}
		break;
	}
}


static int final_read_and_write(FILE_DESC input_file_desc,
								FILE_DESC output_fl_desc,
								TEXT * file_name,
								SLONG io_size,
								UCHAR ** io_buffer,
								bool * end_of_input)
{

/********************************************************************
**
**	f i n a l _ r e a d _ a n d _ w r i t e
**
*********************************************************************
**
**	Functional description:
**
**		Read all data from input file and write the data out to output
**		file.
**
*********************************************************************
*/

	SLONG read_cnt, write_cnt;

	read_cnt = read(input_file_desc, *io_buffer, io_size);

	switch (read_cnt) {
	case (0):					/* no more data to be read */
		close(output_fl_desc);
		*end_of_input = true;
		return FB_SUCCESS;
		break;

	case (-1):					/* read failed */
		close(output_fl_desc);
		ib_fprintf(ib_stderr,
				   "problem when reading input file, errno = %d\n", errno);
		return FB_FAILURE;
		break;

	default:					/* read ok */
		break;
	}

	write_cnt = write(output_fl_desc, *io_buffer, read_cnt);

	switch (write_cnt) {
	case (-1):					/* write failed */
		close(output_fl_desc);
		return FB_FAILURE;
		break;

	default:
		if (write_cnt == read_cnt)	/* write ok */
			return FB_SUCCESS;
		else {					/* write less data then it reads in */

			ib_fprintf(ib_stderr,
					   "There is no enough space to write to back up file %s\n",
					   file_name);
			close(output_fl_desc);
			return FB_FAILURE;
		}
		break;
	}
}


static int join_multy_bakup_files( B_FIL file_list)
{
/********************************************************************
**
**	j o i n _ m u l t y _ b a c k u p _ f i l e s
**
*********************************************************************
**
**	Functional description:
**
**		allocates I/O buffer and walks through backup files` chain.
**		calls read_and_write_for_join routine to read data from
**		backup file and to write data to standard output file
**		which later to be processed by gbak. Finally frees up
**		I/O buffer
**
*********************************************************************
*/

	FILE_DESC output_fl_desc;
	B_FIL fl_ptr, next_fl;
	TEXT *file_name;
	UCHAR* io_buffer = 0;
	SLONG cnt, ret_cd, total_int;

	output_fl_desc = reinterpret_cast<FILE_DESC>(GBAK_STDOUT_DESC);

	io_buffer = (UCHAR *) malloc(IO_BUFFER_SIZE);
	cnt = total_int = 0;

	if (io_buffer == 0) {
		ib_fprintf(ib_stderr, "I/O buffer allocation failed\n");
		return FB_FAILURE;
	}

	for (fl_ptr = file_list; fl_ptr; fl_ptr = next_fl) {
		cnt++;
		next_fl = fl_ptr->b_fil_next;
		file_name = fl_ptr->b_fil_name;

		ret_cd =
			read_and_write_for_join(output_fl_desc, file_name, &io_buffer,
									cnt, &total_int);

		if (ret_cd == FB_FAILURE) {
			free(io_buffer);
			return FB_FAILURE;
		}

	}							/* end of for loop */

	free(io_buffer);
	return FB_SUCCESS;
}


static int read_and_write_for_join(FILE_DESC output_fl_desc,
								   TEXT * file_name,
								   UCHAR ** io_buffer,
SLONG cnt, SLONG * total_int)
{
/********************************************************************
**
**	r e a d _ a n d _ w r i t e _ f o r _ j o i n
**
*********************************************************************
**
**	Functional description:
**
**		Reads data from backup files and writes to standard
**		output file.
**
*********************************************************************
*/

	FILE_DESC input_fl_desc;
	SLONG indx, num_int, ret_cd, read_cnt, write_cnt;
	SLONG skip_to_num, skip_to_total;
	TEXT *char_ptr1, *char_ptr2, num_arr[5], total_arr[5];
	HEADER_REC hdr_rec;

	input_fl_desc = open(file_name, MODE_READ);

	if (input_fl_desc == -1) {
		ib_fprintf(ib_stderr, "can not open input file %s\n", file_name);
		return FB_FAILURE;
	}

	read_cnt = read(input_fl_desc, *io_buffer, HEADER_REC_LEN);
	if (read_cnt != HEADER_REC_LEN) {
		close(input_fl_desc);
		ib_fprintf(ib_stderr,
				   "progam fails to read gsplit header record in back-up file%s\n",
				   file_name);
		return FB_FAILURE;
	}

	char_ptr1 = reinterpret_cast<char*>(*io_buffer);
	ret_cd = strncmp(char_ptr1, HEADER_REC_NAME, sizeof(hdr_rec.name) - 1);
	if (ret_cd != 0) {
		close(input_fl_desc);
		ib_fprintf(ib_stderr, "gsplit: expected GSPLIT description record\n");
		ib_fprintf(ib_stderr,
				   "gsplit: Exiting before completion due to errors\n");
		return FB_FAILURE;
	}

	skip_to_num = sizeof(hdr_rec.name) + sizeof(hdr_rec.date_time) +
		sizeof(hdr_rec.text1);
	skip_to_total = skip_to_num + sizeof(hdr_rec.num) + sizeof(hdr_rec.text2);

	char_ptr1 = reinterpret_cast<char*>(*io_buffer + skip_to_num);
	char_ptr2 = reinterpret_cast<char*>(*io_buffer + skip_to_total);
	for (indx = 0; indx < sizeof(hdr_rec.num); indx++) {
		num_arr[indx] = *char_ptr1;
		char_ptr1++;
		if (cnt == 1) {
			total_arr[indx] = *char_ptr2;
			char_ptr2++;
		}
	}
	num_arr[indx] = '\0';
	num_int = atoi(num_arr);
	if (cnt == 1) {
		total_arr[indx] = '\0';
		*total_int = atoi(total_arr);
	}

	if ((num_int != cnt) || (num_int > *total_int)) {
		close(input_fl_desc);
		ib_fprintf(ib_stderr,
				   "gsplit: join backup file is out of sequence\n");
		ib_fprintf(ib_stderr,
				   "gsplit: Exiting before completion due to errors\n");
		return FB_FAILURE;
	}

	read_cnt = read(input_fl_desc, *io_buffer, IO_BUFFER_SIZE);


	while (true) {
		switch (read_cnt) {
		case (0):				/* no more data to be read */
			close(input_fl_desc);
			return FB_SUCCESS;
			break;

		case (-1):				/* read failed */
			close(input_fl_desc);
			return FB_FAILURE;
			break;

		default:				/* this is the last read */
			break;
		}

		write_cnt = write(output_fl_desc, *io_buffer, read_cnt);

		switch (write_cnt) {
		case (-1):				/* write failed */
			close(input_fl_desc);
			return FB_FAILURE;
			break;

		default:
			fb_assert(write_cnt == read_cnt);
			break;
		}

		read_cnt = read(input_fl_desc, *io_buffer, IO_BUFFER_SIZE);

	} // end of while (true) loop
}


static int conv_ntoc( SLONG numeric_in, TEXT char_out[])
{
/********************************************************************
**
**	c o n v _ n t o c
**
*********************************************************************
**
**	Functional description:
**
**		Convert 4-digit numeric data to a 4-byte character array.
**
*********************************************************************
*/

	SLONG indx, mod, i;

	i = numeric_in;
	indx = 3;

	while (true) {
		mod = i % 10;
		switch (mod) {
		case (0):
			char_out[indx] = '0';
			break;

		case (1):
			char_out[indx] = '1';
			break;

		case (2):
			char_out[indx] = '2';
			break;

		case (3):
			char_out[indx] = '3';
			break;

		case (4):
			char_out[indx] = '4';
			break;

		case (5):
			char_out[indx] = '5';
			break;

		case (6):
			char_out[indx] = '6';
			break;

		case (7):
			char_out[indx] = '7';
			break;

		case (8):
			char_out[indx] = '8';
			break;

		default:
			char_out[indx] = '9';
			break;
		}
		indx = indx - 1;
		i = i / 10;
		if (i <= 0)
			break;
	}

	for (; indx >= 0; indx--) {
		char_out[indx] = ' ';
	}
	return FB_SUCCESS;
}


static int write_header(B_FIL		fl_ptr,
						HEADER_REC	hdr_rec,
						FILE_DESC	output_fl_desc,
						TEXT		header_str[])
{
/********************************************************************
**
**	w r i t e _ h e a d e r
**
*********************************************************************
**
**	Functional description:
**
**		writes out gsplit header record
**
**		assigns backup file name to header record file name array
**		calls conv_ntoc routine to convert numeric data to char data
**			and assigns to header record file number field
**		writes gsplit header record out to backup file
**
*********************************************************************
*/
	TEXT *file_name, num_arr[5];
	SLONG end, indx, pos, ret_cd, write_cnt;

	ret_cd = conv_ntoc(fl_ptr->b_fil_number, num_arr);
	if (ret_cd == FB_FAILURE) {
		ib_printf
			("gsplit could not convert numeric data to character data\n");
		return FB_FAILURE;
	}
	num_arr[sizeof(num_arr) - 1] = TERMINAL;
	pos = sizeof(hdr_rec.name) + sizeof(hdr_rec.date_time) +
		sizeof(hdr_rec.text1);
	ret_cd = set_hdr_str(header_str, num_arr, pos, sizeof(hdr_rec.num));

	file_name = fl_ptr->b_fil_name;
	pos = sizeof(hdr_rec.name) + sizeof(hdr_rec.date_time) +
		sizeof(hdr_rec.text1) + sizeof(hdr_rec.num) +
		sizeof(hdr_rec.text2) + sizeof(hdr_rec.total) + sizeof(hdr_rec.text3);
	ret_cd = set_hdr_str(header_str, file_name, pos, strlen(file_name));
	write_cnt = write(output_fl_desc, header_str, HEADER_REC_LEN);

	switch (write_cnt)
	{
	case (-1):					/* write failed */
		close(output_fl_desc);
		return FB_FAILURE;
		break;

	default:
		end = pos + strlen(file_name);
		for (indx = pos; indx < end; indx++)
			header_str[indx] = BLANK;
		return FB_SUCCESS;
		break;
	}
}


static int flush_io_buff(UCHAR*		remaining_io,
						 SLONG		remaining_io_len,
						 FILE_DESC	output_fl_desc,
						 double		file_size,
						 SLONG*		byte_write,
						 bool*		flush_done)
{
/********************************************************************
**
**	f l u s h _ i o _ b u f f
**
*********************************************************************
**
** Functional description:
**
**	flush out the remaining data in the I/O buffer
**
**	when the file_size is truly has space to write out the remaining data
**		then
**			set and returns flush_done true
**		otherwise
**			closes the output file, set and returns flush_done false.
**	otherwise
**		we can only writes out as much data as file_size indicated
**		if it was able to write out the remaining data
**			then
**				set and returns flush_done true
**			otherwise
**				closes the output file, set and returns flush_done false.
**
*********************************************************************
*/

	SLONG write_cnt;

	if (file_size > remaining_io_len) {
		write_cnt = write(output_fl_desc, remaining_io, remaining_io_len);
	}
	else {						/* file_size <= remaining_io_len */

		write_cnt =
			write(output_fl_desc, remaining_io, (unsigned int) file_size);
	}

	switch (write_cnt) {
	case (-1):					/* write failed */
		close(output_fl_desc);
		*flush_done = false;
		return FB_FAILURE;
		break;

	default:
		if (write_cnt == remaining_io_len)	/* write ok */
			*flush_done = true;
		else {					/* could not write out all remaining data */

			close(output_fl_desc);
			*flush_done = false;
		}
		*byte_write = write_cnt;
		return FB_SUCCESS;
		break;
	}
}


static int final_flush_io_buff(UCHAR * remaining_io,
							   SLONG remaining_io_len,
							   FILE_DESC output_fl_desc)
{
/********************************************************************
**
**	f i n a l _ f l u s h _ i o _ b u f f
**
*********************************************************************
**
** Functional description:
**
**	flush out the remaining data in the I/O buffer
**
*********************************************************************
*/

	SLONG write_cnt;

	write_cnt = write(output_fl_desc, remaining_io, remaining_io_len);
	switch (write_cnt)
	{
	case (-1):					/* write failed */
		close(output_fl_desc);
		return FB_FAILURE;
		break;

	default:
		if (write_cnt == remaining_io_len)	/* write ok */
			return FB_SUCCESS;
		else {					/* could not write out all remaining data */

			close(output_fl_desc);
			return FB_FAILURE;
		}
		break;
	}
}


static int print_clo(TEXT* prog_name)
{
/********************************************************************
**
**	p r i n t _ c l o
**
*********************************************************************
**
**	Functional description:
**
**		print out gsplit utility command line options
**
*********************************************************************
*/

	ib_fprintf(ib_stderr, "%s: Command Line Options Are:\n", prog_name);
	ib_fprintf(ib_stderr,
			   "  gsplit -S[PLIT_BK_FILE] <file> <size>{k|m|g} [... <file> [<size>{k|m|g}]] or\n");
	ib_fprintf(ib_stderr, "  gsplit -J[OINT_BK_FILE] <file> [... <file>]\n");
	ib_fprintf(ib_stderr,
			   "%s: option can be abbreviated to the unparenthesized characters\n",
			   prog_name);
	ib_fprintf(ib_stderr,
			   "%s: Exiting before completion due to errors\n", prog_name);

	return FB_SUCCESS;

}


static int set_hdr_str(TEXT header_str[], TEXT * in_str, SLONG pos, SLONG len)
{
/********************************************************************
**
**	s e t _ h d r _ s t r
**
*********************************************************************
**
**	Functional description:
**
**		initialyze header string
**
*********************************************************************
*/

	TEXT *t_str;
	SLONG end, indx;

	t_str = in_str;
	end = pos + len;

	for (indx = pos; indx < end; indx++) {
		switch (*t_str) {
		case (NEW_LINE):
			header_str[indx] = ' ';
			break;
		case (TERMINAL):
			header_str[indx] = ' ';
			break;
		default:
			header_str[indx] = *t_str;
			break;
		}
		*t_str++;
	}
	return FB_SUCCESS;
}


static int free_file_list( B_FIL file_list)
{
/********************************************************************
**
**	f r e e _ f i l e _ l i s t
**
*********************************************************************
**
**	Functional description:
**
**		free all the storage allocated for backup files structure
**
*********************************************************************
*/

	B_FIL file_ptr, next_file;

	for (file_ptr = file_list; file_ptr != NULL; file_ptr = next_file) {
		next_file = file_ptr->b_fil_next;
		free(file_ptr->b_fil_name);
		free(file_ptr);
	}

	return FB_SUCCESS;
}
