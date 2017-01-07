static void StartUserThread(int f){
	// here goes the function definition for starting an user thread

}

int do_UserThreadCreate(int f, int arg){
	// here goes the function for creating an user thread

	char* threadname = new char[THREADNAME_SIZE];
	snprintf(threadname,THREADNAME_SIZE,"%d",counter );
	counter++;
	Thread* newThread = new Thread(threadname);
	newThread->Fork(StartUserThread, f);
	delete(threadname);

}

int do_UserThreadExit(){
	// here goes the code for user thread exit
	// Implement Thread::Exit


}