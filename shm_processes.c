#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  ClientProcess(int []);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
     int i;

     if (argc != 11) {
          printf("Use: %s #1 #2 #3 #4 #5 #6 #7 #8 #9 #10\n", argv[0]);
          exit(1);
     }

     ShmID = shmget(IPC_PRIVATE, 10*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
  
  
  
    for (i = 1; i < 11; i++) {
      ShmPTR[i - 1] = atoi(argv[i]);
    }

     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          ClientProcess(ShmPTR);
          exit(0);
     }

     wait(&status);
     printf("Server: %d %d %d %d %d %d %d %d %d %d\n",
            ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3], ShmPTR[4],
            ShmPTR[5], ShmPTR[6], ShmPTR[7], ShmPTR[8], ShmPTR[9]);    
     shmdt((void *) ShmPTR);
     shmctl(ShmID, IPC_RMID, NULL);
     exit(0);
}

void  ClientProcess(int  SharedMem[])
{
     printf("Client: %d %d %d %d %d %d %d %d %d %d\n",
                SharedMem[0], SharedMem[1], SharedMem[2], SharedMem[3],
                SharedMem[4], SharedMem[5], SharedMem[6], SharedMem[7],
                SharedMem[8], SharedMem[9]);
     return;
}