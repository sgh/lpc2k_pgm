/*
 * LPC 2000 Loader, http://www.pjrc.com/arm/lpc2k_pgm
 * Copyright (c) 2004, PJRC.COM, LLC, <paul@pjrc.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

/* If this code fails to build, please provide at least the following
 * information when requesting (free) technical support.
 * 
 * 1: Complete copy of all messages during the build.
 * 2: Output of "gtk-config --version"
 * 3: Output of "gtk-config --libs"
 * 4: Output of "gtk-config --cflags"
 * 5: Output of "uname -a"
 * 6: Version of GTK installed... eg, type: ls -l /lib/libgtk*
 * 7: Other info... which linux distribution, version, other software
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>


#include "lpc2k_pgm.h"
#include "gui.h"
#include "serial.h"
#include "settings.h"


int term_fd=-1;

int open_tty(void);
void run_as_emulator(char *program_name);


int main(int argc, char **argv)
{
	run_as_emulator(argv[0]);
	init_settings();
	term_fd = open_tty();
	if (term_fd < 0) {
		printf("Unable to open I/O to terminal (/dev/tty), r=%d\n",
			term_fd);
		sleep(1);
		exit(1);
	}
	create_window(&argc, &argv);
	run_gui();
	close_serial_port();
	close(term_fd);
	return 0;
}


void change_baud(const char *baud_name)
{
	int r;

	r = set_baud(baud_name);
	if (r == 0) {
		printf("Baud rate set to %s\r\n", baud_name);
		new_baud_setting(baud_name);
	} else {
		printf("Unable to set baud to %s\r\n", baud_name);
	}
}



int open_tty(void)
{
	int r, fd;
	struct termios term_setting;

	fd = open("/dev/tty", O_RDWR);
	if (fd < 0) return -1;
	r = tcgetattr(fd, &term_setting);
	if (r != 0) return -2;
	term_setting.c_oflag &= ~OPOST;
	term_setting.c_iflag |= (IGNBRK | IGNPAR);
	term_setting.c_iflag &= ~(ISTRIP | BRKINT);
	term_setting.c_lflag &= ~(ICANON | ISIG | ECHO);
	term_setting.c_cflag |= CREAD;
	term_setting.c_cc[VMIN] = 1;
	term_setting.c_cc[VTIME] = 1;
	r = tcsetattr(fd, TCSANOW, &term_setting);
	if (r != 0) return -3;
	return fd;
}


void run_as_emulator(char *program_name)
{
	const char *is_child;
	char *xterm_arg[20];

	is_child = getenv("LPC2K_RUNNING_AS_EMULATOR");
	if (is_child == NULL || strcmp(is_child, "yep")) {
		//printf("not running as emulator\n");
		setenv("LPC2K_RUNNING_AS_EMULATOR", "yep", 1);
		xterm_arg[0] = "xterm";
		xterm_arg[1] = "-e";
		xterm_arg[2] = program_name;
		xterm_arg[3] = NULL;
		execvp("xterm", xterm_arg);
		exit(1);
	}
}


