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
/* The first process continuously reads characters
from the keyboard and stores them in a local array as a character string. Each time the array is filled
with a string the process sends the array as a message to the second process via a previously created
message queue. If the queue is full the process blocks waiting for space on the queue.  */

int receive() { 									/* this function waits for a message and then processes it */
	struct msgbuf msg;
	int msgid, i;
	msgid = msgget(MSG_KEY, 0777|IPC_CREAT);	/* ask the kernel to set up a message queue, IPC_CREAT creates a message queue*/
	while(1){
		msgrcv(msgid, &msg, 128, 1, 0);				/* check for a message of type 1. The kernel will copy the message into the msg structure. */
		size_t cLength = strlen(msg.mtext);
		for(i = 0; i < cLength; i++){
			printf("%s", msg.mtext);
		}
	}
}

/*
msgget(key, msgflag): returns id of specified key(msgqid)
key: 		user specified number
msgflag: 	who has permission

msgrcv(msgqid, msgbuf*msgptr, msgsize, msgtype, flag)
return:		when completed, returns 0

*/ 