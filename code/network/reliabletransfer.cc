#ifdef CHANGED
#include "synch.h"
#include "post.h"
#include "system.h"
#define TEMP0 100
#define MAXREEMISSIONS 1500
static void AlertSenderThread(int param);	

class ReliableTransfer
{
public:
	ReliableTransfer()
	{
		mutexlock = new Semaphore("transfer lock", 1);
		reply = 0;
	}
    bool Send(PacketHeader pktHdr, MailHeader mailHdr, const char *msgData)
    {

	for (int i = 0; i < MAXREEMISSIONS; i++)
	{
		
		postOffice->Send(pktHdr, mailHdr, msgData);
		interrupt->Schedule(AlertSenderThread, (int)this, TEMP0, NetworkRecvInt);
		mutexlock->P();
		if(reply)
		return TRUE;

	}
	return FALSE;
    }
	void Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr,  char *msgData)
	{
			// Wait for the first message from the other machine
	postOffice->Receive(box, pktHdr, mailHdr, msgData);
	//printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
	//fflush(stdout);

	PacketHeader outPktHdr;
	MailHeader outMailHdr;
	const char *ack = "Got it!";
	// Send acknowledgement to the other machine (using "reply to" mailbox
	// in the message that just arrived
	outPktHdr.to = pktHdr->from;
	outMailHdr.to = mailHdr->from;
	outMailHdr.length = strlen(ack) + 1;
	postOffice->Send(outPktHdr, outMailHdr, ack);
	
	postOffice->Receive(box, pktHdr, mailHdr, msgData);
	//printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
	//fflush(stdout);
	
	reply=1;
	mutexlock->V();
	
	
	}
	
	Semaphore* mutexlock;
	int reply;
};


static void AlertSenderThread(int param)
{ 
	ReliableTransfer *rt = (ReliableTransfer *)param; 
	rt->mutexlock->V(); 
} 
#endif
