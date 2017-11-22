#include "init.h"
#include "externs.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
void sigint_handler(int sig);

void setup(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void sigint_handler(int sig)
{
	char buf[40];
	getcwd(buf, sizeof(buf));
	printf("\n%s[MonsterSXF-shell]$ ", buf);
	fflush(stdout);
}

void init(void)
{
	memset(cmd, 0, sizeof(cmd));
	int i;
	for (i=0; i<PIPELINE; ++i)
	{
		cmd[i].infd = 0;
		cmd[i].outfd = 1;
	}
	memset(cmdline, 0, sizeof(cmdline));
	memset(avline, 0, sizeof(avline));
	lineptr = cmdline;
	avptr = avline;
	cmd_count = 0;
	backgnd = 0;
	append = 0;
	lastpid = 0;
	char buf[40];
	getcwd(buf, sizeof(buf));
	printf("%s[MonserSXFshell]$ ", buf);
	fflush(stdout);
}
