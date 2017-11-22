#include "parse.h"
#include "externs.h"
#include "init.h"
#include "execute.h"
#include "builtin.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <fcntl.h>
/* ��ѭ��*/
void shell_loop(void)
{
	while (1)
	{
		/* ��ʼ������ */
		init();
		/* ��ȡ���� */
		if (read_command() == -1)
			break;
		/* �������� */
		parse_command();
		/* ִ������ */
		execute_command();
	}
	
	printf("\nexit\n");
}

/* ��ȡ���� */
int read_command(void)
{
	/* ���ж�ȡ���cmdline�а���\n�ַ� */
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	return 0;
}

/* �������� */
int parse_command(void)
{
	if (check("\n"))
		return 0;
	/* �ж��Ƿ��ڲ����ִ�� */
	if (builtin())
		return 0;
	/* ���������� */
	get_command(0);
	/* �Ƿ���ڹܵ� */
	int i;
	for (i=1; i<PIPELINE; ++i)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	/* �Ƿ��̨��ҵ */
	if (check("&"))
		backgnd = 1;
	/* ���������\n��*/
	if (check("\n"))
	{
		cmd_count = i;
		return cmd_count;
	}
	else
	{
		fprintf(stderr, "Syntax Error!!!\n");
		return -1;
	}
}

/*
 * ִ������
 * �ɹ�����0��ʧ�ܷ���-1
 */
int execute_command(void)
{
	execute_disk_command();
	return 0;
}


/*
 * ������������cmd[i]
 * ��ȡcmdline�е����������avline�����У�
 * ���ҽ�COMMAND�ṹ�е�args[]�е�ÿ��ָ��ָ����Щ�ַ���
 */
void get_command(int i)
{
	/*   cat < test.txt | grep -n public > test2.txt & */

	int j = 0;
	int inword;
	while (*lineptr != '\0')
	{
		/* ȥ���ո� */
		while (*lineptr == ' ' || *lineptr == '\t')
			lineptr++;

		/* ����i�������j������ָ��avptr */
		cmd[i].args[j] = avptr;
		/* ��ȡ���� */
		while (*lineptr != '\0'
			&& *lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '|'
			&& *lineptr != '&'
			&& *lineptr != '\n')
		{
				/* ������ȡ��avptrָ�����������avline */
				*avptr++ = *lineptr++;
				inword = 1;
		}
		*avptr++ = '\0';
		switch (*lineptr)
		{
		case ' ':
		case '\t':
			inword = 0;
			j++;
			break;
		case '|':
		case '&':
		case '\n':
			if (inword == 0)
				cmd[i].args[j] = NULL;
			return;
		default:
			return;
		}
	}
}

/* ��lineptr�е��ַ�����str����ƥ�� */
int check(const char *str)
{
	char *p;
	while (*lineptr == ' ' || *lineptr == '\t')
		lineptr++;

	p = lineptr;
	while (*str != '\0' && *str == *p)
	{
		str++;
		p++;
	}

	if (*str == '\0')
	{
		lineptr = p;	
		return 1;
	}


	return 0;
}



