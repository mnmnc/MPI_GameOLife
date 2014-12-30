#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
 
/************************************************************
This is a simple isend/ireceive program in MPI
************************************************************/

int main(int argc, char *argv[])
{
    int myid, numprocs;
    int tag,source,destination1,count,destination2,destination3;
    int buffer1, buffer2, buffer3;
    buffer1=1;
    buffer2=2;
    buffer3=3;
    MPI_Status status;
    MPI_Request request;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);


    tag=1234;
    source=0;
    destination1=1;
    destination2=2;
    destination3=3;
    count=1;
    request=MPI_REQUEST_NULL;
    int iterations = 1000000;

    while (iterations > 0){
	    if(myid == source){
	      buffer1++;
	      buffer2++;
	      buffer3++;
	      MPI_Isend(&buffer1,count,MPI_INT,destination1,tag,MPI_COMM_WORLD,&request);
	      MPI_Isend(&buffer2,count,MPI_INT,destination2,tag,MPI_COMM_WORLD,&request);
	      MPI_Isend(&buffer3,count,MPI_INT,destination3,tag,MPI_COMM_WORLD,&request);
	    }

	    if(myid == destination1){
	        MPI_Irecv(&buffer1,count,MPI_INT,source,tag,MPI_COMM_WORLD,&request);
	    }
	    if(myid == destination2){
	        MPI_Irecv(&buffer2,count,MPI_INT,source,tag,MPI_COMM_WORLD,&request);
	    }
	    if(myid == destination3){
	        MPI_Irecv(&buffer3,count,MPI_INT,source,tag,MPI_COMM_WORLD,&request);
	    }

	    MPI_Wait(&request,&status);
	    if(myid == source){
	      printf("processor %d  sent %d\n",myid,buffer1);
	      printf("processor %d  sent %d\n",myid,buffer2);
	      printf("processor %d  sent %d\n",myid,buffer3);
	      --iterations;
	    }
	    if(myid == destination1){
	      printf("processor %d  got %d\n",myid,buffer1);
	      --iterations;
	    }
	    if(myid == destination2){
	      printf("processor %d  got %d\n",myid,buffer2);
	      --iterations;
	    }
	    if(myid == destination3){
	      printf("processor %d  got %d\n",myid,buffer3);
	      --iterations;
	    }
	}
    MPI_Finalize();
}