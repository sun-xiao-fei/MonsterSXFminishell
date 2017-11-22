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
/* 主循环*/
void shell_loop(void)
{
	while (1)
	{
		/* 初始化环境 */
		init();
		/* 获取命令 */
		if (read_command() == -1)
			break;
		/* 解析命令 */
		parse_command();
		/* 执行命令 */
		execute_command();
	}
	
	printf("\nexit\n");
}

/* 读取命令 */
int read_command(void)
{
	/* 按行读取命令，cmdline中包含\n字符 */
	if (fgets(cmdline, MAXLINE, stdin) == NULL)
		return -1;
	return 0;
}

/* 解析命令 */
int parse_command(void)
{
	if (check("\n"))
		return 0;
	/* 判断是否内部命令并执行 */
	if (builtin())
		return 0;
	/* 解析简单命令 */
	get_command(0);
	/* 是否存在管道 */
	int i;
	for (i=1; i<PIPELINE; ++i)
	{
		if (check("|"))
			get_command(i);
		else
			break;
	}
	/* 是否后台作业 */
	if (check("&"))
		backgnd = 1;
	/* 命令结束‘\n’*/
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
 * 执行命令
 * 成功返回0，失败返回-1
 */
int execute_command(void)
{
	execute_disk_command();
	return 0;
}


/*
 * 解析简单命令至cmd[i]
 * 提取cmdline中的命令参数到avline数组中，
 * 并且将COMMAND结构中的args[]中的每个指针指向这些字符串
 */
void get_command(int i)
{
	/*   cat < test.txt | grep -n public > test2.txt & */

	int j = 0;
	int inword;
	while (*lineptr != '\0')
	{
		/* 去除空格 */
		while (*lineptr == ' ' || *lineptr == '\t')
			lineptr++;

		/* 将第i条命令第j个参数指向avptr */
		cmd[i].args[j] = avptr;
		/* 提取参数 */
		while (*lineptr != '\0'
			&& *lineptr != ' '
			&& *lineptr != '\t'
			&& *lineptr != '|'
			&& *lineptr != '&'
			&& *lineptr != '\n')
		{
				/* 参数提取至avptr指针所向的数组avline */
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

/* 将lineptr中的字符串与str进行匹配 */
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



