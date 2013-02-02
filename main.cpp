/**
 * The entry point for our compiler.
 * 
 * @author: Dan Albert <dan@gingerhq.net>
 *
 * This compiler translates from IBTL (Itty Bitty Teaching Language) to GForth.
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "lexer.h"
#include "messages.h"
 
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(x, l, h) (((x) > (h)) ? (h) : (((x) < (l)) ? (l) : (x)))

void translate(const char* file);

void usage(void);

int main(int argc, char **argv)
{
	int c;
 
	while (1)
	{
		static struct option long_options[] =
		{
			{"help",    no_argument, 0, 'h'},
			{"verbose", no_argument, 0, 'v'},
			{0, 0, 0, 0}
		};

		int option_index = 0;
 
		c = getopt_long(argc, argv, "hv",
		                long_options, &option_index);
 
		/* Detect the end of the options. */
		if (c == -1)
			break;
 
		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			printf ("option %s", long_options[option_index].name);
			if (optarg)
				printf(" with arg %s", optarg);
			printf ("\n");
			break;
		case 'h':
			usage();
			exit(EXIT_SUCCESS);
			break;
		case 'v':
			set_log_level(CLAMP(get_log_level() + 1,
			                    LOGLEVEL_MIN, LOGLEVEL_MAX));
			break;
		case '?':
			/* getopt_long already printed an error message. */
			break;
		default:
			CRITICAL("Should never reach default case");
			exit(EXIT_FAILURE);
			break;
		}
	}
 
	if (optind < argc)
	{
		while (optind < argc)
		{
			translate(argv[optind++]);
		}
	}
	else
	{
		usage();
		exit(EXIT_FAILURE);
	}
 
	return 0;
}

void translate(const char* file)
{
	Lexer lex;
	if (!lex.Init(file))
	{
		CRITICAL("Could not open file %s", file);
		exit(EXIT_FAILURE);
	}

	while (lex.HasNext())
	{
		lex.Next().Print();
		printf("\n");
	}
}

void usage(void)
{
    printf("usage: compiler OPTIONS FILES\n");
    printf("OPTIONS:\n");
    printf("    -h, --help      - display the message\n");
    printf("    -v, --verbose   - increase output verbosity\n");
}

