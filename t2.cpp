#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
 
/************************************************************
This is a simple isend/ireceive program in MPI
************************************************************/
using namespace std;

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

    const int size = 2;

 	char * buff_in = new char[size];
 	char * buff_out = new char[size];
 	for (unsigned int i = 0; i < sizeof(buff_in)/sizeof(buff_in[0]); ++i){
 		buff_in[i] = i;
 		buff_out[i] = i;
 	}

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);


    tag=1234;
    source=0;
    destination1=1;
    destination2=2;
    destination3=3;
    count=2;
    request=MPI_REQUEST_NULL;
    int iterations = 2;

    while (iterations > 0){
	    if(myid == source){

	      for (unsigned int i = 0; i < sizeof(buff_in)/sizeof(buff_in[0]); ++i){
	      	buff_in[i] = 0;
	      }

	      MPI_Isend(&buff_in,count,MPI_CHAR,destination1,tag,MPI_COMM_WORLD,&request);
	      MPI_Isend(&buff_in,count,MPI_CHAR,destination2,tag,MPI_COMM_WORLD,&request);
	      MPI_Isend(&buff_in,count,MPI_CHAR,destination3,tag,MPI_COMM_WORLD,&request);
	    }

	    if(myid == destination1){
	        MPI_Irecv(&buff_out,count,MPI_CHAR,source,tag,MPI_COMM_WORLD,&request);
	    }
	    if(myid == destination2){
	        MPI_Irecv(&buff_out,count,MPI_CHAR,source,tag,MPI_COMM_WORLD,&request);
	    }
	    if(myid == destination3){
	        MPI_Irecv(&buff_out,count,MPI_CHAR,source,tag,MPI_COMM_WORLD,&request);
	    }

	    MPI_Wait(&request,&status);
	    if(myid == source){
	      printf("processor %d  sent %d\n",myid,(int)buff_in[0]);
	      printf("processor %d  sent %d\n",myid,(int)buff_in[1]);
	      printf("processor %d  sent %d\n",myid,(int)buff_in[2]);
	      --iterations;
	    }
	    if(myid == destination1){
	      
	      for (unsigned int i = 0; i < sizeof(buff_out)/sizeof(buff_out[0]); ++i){

	      	printf("%d processor %d  got %d - %d\n",iterations, myid,i,(int)buff_out[i]);
	      }
	      --iterations;
	    }
	    if(myid == destination2){
	      
	      for (unsigned int i = 0; i < sizeof(buff_out)/sizeof(buff_out[0]); ++i){
	      	printf("%d processor %d  got %d - %d\n",iterations,myid,i,(int)buff_out[i]);
	      }
	      --iterations;
	    }
	    if(myid == destination3){
	      for (unsigned int i = 0; i < sizeof(buff_out)/sizeof(buff_out[0]); ++i){
	      	printf("%d processor %d  got %d - %d\n",iterations,myid,i,(int)buff_out[i]);
	      }
	      --iterations;
	    }
	}
    MPI_Finalize();
}