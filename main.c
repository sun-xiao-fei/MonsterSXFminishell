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
/*����ָ��ǰ��Ծ�Ľ��̵�ָ��*/
NODE *head;

int main(void)
{
    head=(NODE*)malloc(sizeof(NODE));
    head->next=NULL;
	/* ��װ�ź� */
	setup();
	/* ����shellѭ�� */
	shell_loop();
	return 0;
}
