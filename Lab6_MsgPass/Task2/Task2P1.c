#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#define MSG_KEY 2288

struct msgbuf {
	long mtype;
	char mtext[128];
};
/* The first process continuously reads characters from the keyboard and stores them in a local array as a character string. 
Each time the array is filled with a string the process sends the array as a message to the second process via a previously created
message queue. 
If the queue is full the process blocks waiting for space on the queue.  */

int main() { 									/* this function waits for a message and then processes it */
	struct msgbuf msg;
	int msgid, i = 0;
	size_t cLength = strlen(msg.mtext);			// finds length of string array mtext[]
	msgid = msgget(MSG_KEY, 0777|IPC_CREAT);	/* ask the kernel to set up a message queue, IPC_CREAT creates a message queue*/
	
	while(1){
		for (i = 0; i < cLength; i++){
			scanf("%s", msg.mtext); 
		}
		int n = msgsnd(msgid, &msg, cLength + 1, 0);
	}
}

/*
msgget(key, msgflag): returns id of specified key(msgqid)
key: 		user specified number
msgflag: 	who has permission

msgsend(msgqid, msgbuf*msgptr, msgsize, flag)
return: 	no. of bytes written to queue
msgqid:		return value of msgget
msgptr:		structure containing message
msgsize:	0 to system imposed max
flag:		action taken if kernel runs out of space

msgrcv(msgqid, msgbuf*msgptr, msgsize, msgtype, flag)
return:		when completed, returns 0

*/ 