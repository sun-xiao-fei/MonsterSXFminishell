#include "parse.h"
#include "init.h"
#include "main.h"

char cmdline[MAXLINE+1];
char avline[MAXLINE+1];
char *lineptr;
char *avptr;
COMMAND cmd[PIPELINE];

int cmd_count;
int backgnd;
int append;
int lastpid;
/*链表指向当前活跃的进程的指针*/
NODE *head;

int main(void)
{
    head=(NODE*)malloc(sizeof(NODE));
    head->next=NULL;
	/* 安装信号 */
	setup();
	/* 进入shell循环 */
	shell_loop();
	return 0;
}
