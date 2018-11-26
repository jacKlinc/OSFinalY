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
/*  The second process runs an infinite loop waiting for any messages from the 1st process. 
Each time a message is received, the process extracts the string from the message and sends it to the screen. Activate two
Terminal windows on your desktop. 
Run the first process in 1 Terminal window. Run the 2nd process in the other window. 
Now as you type in 1 window you should see the text also appearing in the other window.  */

int main() { 									/* this function waits for a message and then processes it */
	struct msgbuf msg;
	int msgid, i;
	msgid = msgget(MSG_KEY, 0777|IPC_CREAT);	/* ask the kernel to set up a message queue, IPC_CREAT creates a message queue*/
	size_t cLength = strlen(msg.mtext);
	while(1){
		msgrcv(msgid, &msg, cLength + 1, 1, 0);				/* check for a message of type 1. The kernel will copy the message into the msg structure. */
		printf("%s", msg.mtext);
	}
}

/*
msgget(key, msgflag): returns id of specified key(msgqid)
key: 		user specified number
msgflag: 	who has permission

msgrcv(msgqid, msgbuf*msgptr, msgsize, msgtype, flag)
return:		when completed, returns 0

*/ 