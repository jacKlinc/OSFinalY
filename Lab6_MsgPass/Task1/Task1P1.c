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
/*The first one should create a message queue, call msgrcv and block waiting for any message of type 1. 
When a message arrives it should print the text of the message on the screen and wait for the next message. */

int main() { 										/* this function waits for a message and then processes it */
	struct msgbuf msg;
	int msgid, i;
	msgid = msgget(MSG_KEY, 0777|IPC_CREAT);		// creates message queue with passed msg key	
	msgrcv(msgid, &msg, 128, 1, 0);				/* check for a message of type 1. The kernel will copy the message into the msg structure. */
	printf("%s", msg.mtext);
}

/*
msgget(key, msgflag): returns id of specified key(msgqid)
key: 		user specified number
msgflag: 	who has permission

msgrcv(msgqid, msgbuf*msgptr, msgsize, msgtype, flag)
return:		when completed, returns 0
mtype:		received message type, specified by sender
mtext:		text of the message 
msgsize:	bytes of mtext
msgtype:	==0 (first msg rx), >0 (first msg of type rx), <0 (1st msg of lowest type >= (abs val of type is rx))
flag:		action taken if kernel runs out of space

*/