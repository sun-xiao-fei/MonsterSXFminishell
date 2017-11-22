#include "execute.h"
#include "main.h"
#include "externs.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <string.h>
/*��������*/
void forkexec(int i)
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
		ERR_EXIT("fork");

	if (pid > 0)
	{
		/* ������ */
		if (backgnd == 1)
        {
           /*�����jobs������*/
               NODE *p=(NODE*)malloc(sizeof(NODE));
               p->npid=pid;
               printf("%s",cmd[0].args[0]);
               strcpy(p->backcn,cmd[0].args[0]);

               NODE* tmp=head->next;
               head->next=p;
               p->next=tmp;
        }
		lastpid = pid;
        usleep(800);

	}
	else if (pid == 0)
	{

		/* ǰ̨��ҵ�ܹ�����SIGINT��SIGQUIT�ź� */
		/* �������ź�Ҫ�ָ�ΪĬ�ϲ��� */
		if (backgnd == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		}
		execvp(cmd[i].args[0], cmd[i].args);
		exit(EXIT_FAILURE);
	}
}

int execute_disk_command(void)
{
	if (cmd_count == 0)
		return 0;

	/* ��Ϊ��̨��ҵ�������wait�ȴ��ӽ����˳� */
	/* Ϊ���⽩�����̣����Ժ���SIGCHLD�ź� */
	if (backgnd == 1)
		signal(SIGCHLD, SIG_IGN);
	else
		signal(SIGCHLD, SIG_DFL);

	int i;
	int fd;
	int fds[2];
	for (i=0; i<cmd_count; ++i)
	{
		/* ����������һ���������Ҫ�����ܵ� */
		if (i<cmd_count-1)
		{
			pipe(fds);
			cmd[i].outfd = fds[1];
			cmd[i+1].infd = fds[0];
		}
                
		forkexec(i);

		if ((fd = cmd[i].infd) != 0)
			close(fd);

		if ((fd = cmd[i].outfd) != 1)
			close(fd);
	}

	if (backgnd == 0)
	{
		/* ǰ̨��ҵ����Ҫ�ȴ��ܵ������һ�������˳� */
		while (wait(NULL) != lastpid)
			;
	}

	return 0;
}
