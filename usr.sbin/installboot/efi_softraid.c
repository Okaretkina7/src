/*	$OpenBSD: efi_softraid.c,v 1.3 2022/10/05 09:58:43 kn Exp $	*/
/*
 * Copyright (c) 2012 Joel Sing <jsing@openbsd.org>
 * Copyright (c) 2022 Klemens Nanni <kn@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/disklabel.h>
#include <sys/ioctl.h>

#include <dev/biovar.h>
#include <dev/softraidvar.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <util.h>
#include <unistd.h>

#include "installboot.h"

void
sr_install_bootblk(int devfd, int vol, int disk)
{
	struct bioc_disk bd;
	char *realdev;
	int diskfd;
	char part;

	/* Get device name for this disk/chunk. */
	memset(&bd, 0, sizeof(bd));
	bd.bd_volid = vol;
	bd.bd_diskid = disk;
	if (ioctl(devfd, BIOCDISK, &bd) == -1)
		err(1, "BIOCDISK");

	/* Check disk status. */
	if (bd.bd_status != BIOC_SDONLINE && bd.bd_status != BIOC_SDREBUILD) {
		fprintf(stderr, "softraid chunk %u not online - skipping...\n",
		    disk);
		return;
	}

	/* Keydisks always have a size of zero. */
	if (bd.bd_size == 0) {
		fprintf(stderr, "softraid chunk %u is keydisk - skipping...\n",
		    disk);
		return;
	}

	if (strlen(bd.bd_vendor) < 1)
		errx(1, "invalid disk name");
	part = bd.bd_vendor[strlen(bd.bd_vendor) - 1];
	if (part < 'a' || part >= 'a' + MAXPARTITIONS)
		errx(1, "invalid partition %c\n", part);
	bd.bd_vendor[strlen(bd.bd_vendor) - 1] = '\0';

	/* Open device. */
	if ((diskfd = opendev(bd.bd_vendor, (nowrite ? O_RDONLY : O_RDWR),
	    OPENDEV_PART, &realdev)) == -1)
		err(1, "open: %s", realdev);

	if (verbose)
		fprintf(stderr, "%s%c: %s boot blocks on %s\n", bd.bd_vendor,
		    part, (nowrite ? "would install" : "installing"), realdev);

	/* Write boot blocks to device. */
	md_installboot(diskfd, realdev);

	close(diskfd);
}

void
sr_install_bootldr(int devfd, char *dev)
{
	/*
	 * EFI platforms have a single stage bootstrap.
	 * sr_install_bootblk() installs it on each softraid chunk.
	 * The softraid volume does not require any bootstrap code.
	 */
}
