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

/*The second process should get the ID of the same message queue, send a message of type 1
 with the string “hello from the sending process” to this queue and terminate. 
The first process should be run in the background.*/

int main() { 										/* this function sends a message */
	struct msgbuf msg;
	int msgid = msgget(MSG_KEY, 0777); 				/* ask the kernel for the id of the message queue set up by the send function above */
	msg.mtype = 1;									// message of type 1
	strcpy(msg.mtext, "hello\n");
	size_t cLength = strlen(msg.mtext);
	msgsnd(msgid, &msg, cLength+1, 0); 					/* send the message. The kernel copies the contents of msg into the kernel area and appends it to the specified queue */
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
*/  