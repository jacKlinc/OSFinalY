#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MSG_KEY 2288

struct msgbuf {
	long mtype;
	char mtext[128];
};

/* this function waits for a message and then processes it */
receive() {
	struct msgbuf msg;
	int msgid;
	/* ask the kernel to set up a message queue */
	msgid = msgget(MSG_KEY, 0777|IPC_CREAT);
	/* check for a message of type 1. The kernel will copy the message into the msg structure. */
	msgrcv(msgid, &msg, 128, 1, 0);
}

/* this function sends a message */
send() {
	struct msgbuf msg;
	int msgid;
	/* ask the kernel for the id of the message queue set up by the send function above */
	msgid = msgget(MSG_KEY, 0);
	msg.mtype = 1;
	strcpy(msg.mtext, “hello”);
	/* send the message. The kernel copies the contents of msg into the kernel area and appends it to the
	specified queue */
	msgsnd(msgid, &msg, 6, 0); ;
}