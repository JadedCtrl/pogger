/*
 * Copyright 2020, Jaidyn Levesque <jadedctrl@teknik.io>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "Invocation.h"

#include <StorageKit.h>
#include <String.h>

#include <getopt.h>

#include "App.h"
#include "AtomFeed.h"
#include "Config.h"
#include "Entry.h"
#include "Feed.h"
#include "Mimetypes.h"
#include "RssFeed.h"
#include "Util.h"


int
usage()
{
	fprintf(stderr, "%s", usageMsg.String());
	return 2;
}


int
invocation(int argc, char** argv)
{
	Config* cfg = ((App*)be_app)->cfg;
	BDateTime maxDate;
	BDateTime minDate;

	static struct option sLongOptions[] = {
		{ "help", no_argument, 0, 'h' },
		{ "verbose", no_argument, 0, 'v' },
		{ "config", required_argument, 0, 'c' },
		{ "cache", required_argument, 0, 'C' },
		{ "before", required_argument, 0, 't' },
		{ "after", required_argument, 0, 'T' },
		{ "output", required_argument, 0, 'O' },
		{ "foreground", no_argument, 0, 'D' },
		{ "update", no_argument, 0, 'u' },
		{ 0, 0, 0, 0 }
	};

	while (true) {
		opterr = 0;
		int c = getopt_long(argc, argv, "+hsuvDm:O:T:t:c:C:", sLongOptions, NULL);

		switch (c) {
			case -1:
				freeargInvocation(argc, argv, optind);
				return 0;
			case 'h':
				return usage();
			case 'c':
				cfg->configDir = BString(optarg);
				break;
			case 'C':
				cfg->cacheDir = BString(optarg);
				break;
			case 's':
				cfg->will_save = true;
			case 't':
				minDate = dateRfc3339ToBDate(optarg);
				if (minDate != NULL) 
					cfg->minDate = minDate;
				else {
					fprintf(stderr, "Invalid date format for `-%c'.\n", optopt);
					return 2;
				}
				break;
			case 'T':
				maxDate = dateRfc3339ToBDate(optarg);
				if (maxDate != NULL) 
					cfg->maxDate = maxDate;
				else {
					fprintf(stderr, "Invalid date format for `-%c'.\n", optopt);
					return 2;
				}
				break;
			case 'O':
				cfg->outDir = BString(optarg);
				break;
			case 'u':
				cfg->updateFeeds = true;
				break;
			case 'v':
				cfg->verbose = true;
				break;
			case 'D':
				cfg->daemon = false;
				break;
			case '?':
				if (optopt == 'O' || optopt == 'm')
					fprintf(stderr, "Option `-%c` requires an argument.\n",
					         optopt);
				else
					fprintf(stderr, "Unknown option `-%c`.\n", optopt);
				return 2;
		}
	}
}


void
freeargInvocation(int argc, char** argv, int optind)
{
	Config* cfg = ((App*)be_app)->cfg;
	if (optind < argc) {
		int freeargc = argc - optind;
		cfg->targetFeeds =  BList(freeargc);

		for (int i = 0; i < freeargc; i++) {
			BString* newFeed = new BString(argv[optind + i]);
			cfg->targetFeeds.AddItem(newFeed);
		}
	}
}


