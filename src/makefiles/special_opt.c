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
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "NeXT" port
 *
 */
#include <stdio.h>

/* The purpose of this program is to simulate the actions of
   the special_opt shell script.  It can be used on brain dead
   systems that don't have a reasonable scripting language. */

#ifdef WIN_NT
#define FLAGS_OPT	"-Ob2gtp"
#else
#define FLAGS_OPT	"-O"
#endif
#define FLAGS_DEBUG	"-g"

main (argc, argv)
    int		argc;
    char	*argv [];
{
int	i, length, included;
char	*compile_line, *remove, *include, *prior;

if (argc < 2)
    {
    usage();
    exit (1);
    }

/* Determine the potential length of the command and allocate enough
   space to hold it. */

for (i = 1, length = 0; i < argc;)
    length += strlen (argv [i++]) + 1;

compile_line = malloc (length);
*compile_line = 0;

/* If the first argument isn't YES or NO, just execute the command, as is. */

if (strcmp (argv [1], "YES") && strcmp (argv [1], "NO"))
    {
    for (i = 1; i < argc;)
	{
	strcat (compile_line, argv [i++]);
	strcat (compile_line, " ");
	}

    printf ("%s\n", compile_line);
    system (compile_line);
    exit (0);
    }

if (!strcmp (argv [1], "YES"))
    {
    remove = FLAGS_DEBUG;
    include = FLAGS_OPT;
    }
else
    {
    remove = FLAGS_OPT;
    include = " ";
    }

prior = NULL;
included = 0;

for (i = 2; i < argc; i++)
    {
    if (prior)
	{
	strcat (compile_line, prior);
	strcat (compile_line, " ");
	}
    if (strcmp (argv [i], remove))
	{
	prior = argv [i];
	if (!strcmp (argv [i], include))
	    included = 1;
	}
    else
	prior = NULL;
    }

if (!included)
    {
    strcat (compile_line, include);
    strcat (compile_line, " ");
    }

if (prior)
    {
    strcat (compile_line, prior);
    strcat (compile_line, " ");
    }

printf ("%s\n", compile_line);
system (compile_line);

exit (0);
}

usage ()
{

fprintf (stderr, "Usage: special_opt [YES] compile_line");
fprintf (stderr, "                   [NO ]");
}