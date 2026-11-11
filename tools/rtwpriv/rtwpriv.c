/******************************************************************************
 *
 * Copyright(c) 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>		/* for memcpy() et al */
#include <unistd.h>		/* for close() */
#include <sys/socket.h>		/* for "struct sockaddr" et al  */
#include <sys/ioctl.h>		/* for ioctl() */
#include <linux/wireless.h>	/* for "struct iwreq" et al */

#include "rtwpriv.h"


//#define DEBUG
#ifdef DEBUG
#define DBG printf
#else
#define DBG(x,...) do{}while(0)
#endif

/************************ SOCKET SUBROUTINES *************************/

/*------------------------------------------------------------------*/
/*
 * Open a socket.
 * Depending on the protocol present, open the right socket. The socket
 * will allow us to talk to the driver.
 */
int
iw_sockets_open(void)
{
	static const int families[] = {
		AF_INET,
		AF_IPX,
#ifdef PLATFORM_LINUX
		AF_AX25,
#endif
		AF_APPLETALK
		};
	unsigned int	i;
	int		sock;

	/*
	 * Now pick any (exisiting) useful socket family for generic queries
	 * Note : don't open all the socket, only returns when one matches,
	 * all protocols might not be valid.
	 * Workaround by Jim Kaba <jkaba@sarnoff.com>
	 * Note : in 99% of the case, we will just open the inet_sock.
	 * The remaining 1% case are not fully correct...
	 */

	/* Try all families we support */
	for(i = 0; i < sizeof(families)/sizeof(int); ++i)
	{
		/* Try to open the socket, if success returns it */
		sock = socket(families[i], SOCK_DGRAM, 0);
		if(sock >= 0)
			return sock;
	}

	return -1;
}

/*------------------------------------------------------------------*/
/*
 * Close the socket used for ioctl.
 */
static inline void
iw_sockets_close(int skfd)
{
	close(skfd);
}

//-----------------------------------------------------------------------------
int wlan_ioctl_mp(
		int skfd,
		char *ifname,
		void *pBuffer,
		unsigned int BufferSize)
{
	int err;
	struct iwreq iwr;

	err = 0;

	memset(&iwr, 0, sizeof(struct iwreq));
	strncpy(iwr.ifr_ifrn.ifrn_name, ifname, strlen(ifname));

	iwr.u.data.pointer = pBuffer;
	iwr.u.data.length = (unsigned short)BufferSize;

	err = ioctl(skfd, RTW_IOCTL_MP, &iwr);

	if (iwr.u.data.length == 0)
		*(char*)pBuffer = 0;

	return err;
}

#define BUF_SIZE 0x800
int main(int argc, char **argv)
{
	int i;
	char *ifname;
	char input[BUF_SIZE];
	int skfd;
	int err = 0;


	if (argc < 3)
	{
		printf("no enough parameters!\n");
		return -EINVAL;
	}

#ifdef DEBUG
	DBG("input:");
	for (i=0; i<argc; i++)
		DBG(" %s", argv[i]);
	DBG("\n");
#endif

	ifname = argv[1];
	sprintf(input, "%s", argv[2]);
	for (i=3; i<argc; i++)
		sprintf(input, "%s %s", input, argv[i]);

	DBG("ifname = %s\n", ifname);
	DBG("input = %s\n", input);

	skfd = iw_sockets_open();
	err = wlan_ioctl_mp(skfd, ifname, input, strlen(input)+1);

	/* Close the socket. */
	iw_sockets_close(skfd);

	if (err < 0) {
		fprintf(stderr, "Interface doesn't accept private ioctl...\n");
		fprintf(stderr, "%s: %s\n", argv[2], strerror(errno));
	} else {
		if (strlen(input) != 0)
			printf("%-8.16s %s:%s\n", ifname, argv[2], input);
	}

	return err;
}

