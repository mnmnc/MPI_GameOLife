#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <array>


 
/************************************************************
This is a simple isend/ireceive program in MPI
************************************************************/
using namespace std;

vector <vector <char>> create_environment(int dimention){
	vector <vector <char>> env;

	for (int i = 0; i < dimention; ++i){
		vector <char> row;
		for (int j = 0; j < dimention; ++j){
			row.push_back(0);
		}
		env.push_back(row);
	}

	return env;
}

vector <vector <char>> set_diagonal(vector <vector <char>> env){
	for (unsigned int i = 0; i < env.size(); ++i){
		for (unsigned int j = 0; j < env.at(i).size(); ++j){
			if (i == j){
				env[i][j] = 1;
			}
		}
	}
	for (unsigned int i = 0; i < env.size(); ++i){
		for (unsigned int j = 0; j < env.at(i).size(); ++j){
			if (j == (env.size() - i - 1)){
				env[i][j] = 1;
			}
		}
	}
	return env;
}

char * vector_to_array(vector<vector<char>> env, int dimention){
	char * arr = new char[dimention * dimention];
	int index = 0;
	for (unsigned int i = 0; i < env.size(); ++i){
		for (unsigned int j = 0; j < env[0].size(); ++j){
			arr[index] = env[i][j];
			++index;
		}
	}
	env.clear();
	return arr;
}

void print_array(char arr[], int full_dimention){
	// PRINTING ARRAY
	cout << endl;
	for (int i = 0; i < full_dimention; ++i){
		cout << (int)arr[i];
		if (((i + 1) % (int)sqrt(full_dimention)) == 0 && i / 1 != 0){
			cout << endl;
		}
	}
}

void print_division_array(char arr[], int full_size, int dimention){
	// PRINTING ARRAY
	cout << endl;
	for (int i = 0; i < full_size; ++i){
		cout << (int)arr[i];
		if (((i + 1) % dimention) == 0 && i / 1 != 0){
			cout << endl;
		}
	}
}

char * array_copy(char arr[], int full_dimention){
	char * new_arr = new char[full_dimention];
	copy(arr, arr + (full_dimention), new_arr);
	return new_arr;
}

int check_neighbours(char arr[], int dimention, int x){
	int count = 0;
	char a, b, c, d, e, f, g, h = 0;
	a = b = c = d = e = f = g = h;

	if ((x - dimention - 1) > -1 && ((int)((x - dimention - 1) / dimention)) == ((int)((x - dimention) / dimention))){
		a = arr[x - dimention - 1];
	}

	if (x - dimention > -1){
		b = arr[x - dimention];
	}

	if ((x - dimention + 1 > 0) && ((int)((x - dimention + 1) / dimention)) == ((int)((x - dimention) / dimention))){
		c = arr[x - dimention + 1];
	}

	if (x - 1 > -1 && ((int)((x - 1) / dimention)) == ((int)(x / dimention))){
		d = arr[x - 1];
	}

	if (x + 1 < dimention*dimention && ((int)((x + 1) / dimention)) == ((int)(x / dimention))){
		e = arr[x + 1];
	}

	if ((x + dimention - 1) < dimention*dimention && ((int)((x + dimention - 1) / dimention)) == ((int)((x + dimention) / dimention))){
		f = arr[x + dimention - 1];
	}

	if ((x + dimention) < dimention*dimention ){
		g = arr[x + dimention];
	}

	if ((x + dimention + 1) < dimention*dimention && ((int)((x + dimention + 1) / dimention)) == ((int)((x + dimention) / dimention))){
		h = arr[x + dimention + 1];
	}

	count = a + b + c + d + e + f + g + h;

	return count;
}

int main(int argc, char *argv[])
{
	 
	int tag = 7;
	int array_broadcast = 9;
	int calculation = 8;
	int count = 100;
	int dimention = 100;
	int full_dimention = 10000;

	//MPI_Status status;
	//MPI_Request request;
	MPI_Request request;
    MPI_Status status;
	request = MPI_REQUEST_NULL;

	MPI_Request	send_request,recv_request;


	const int size = 100;

 	char buffi[10000] = {};
 	char buffo[10000] = {};
 	char buffc[10000];

 	for (int i = 0; i < 10000; ++i){
 		buffc[i] = 0;
 	}

 	//print_array(buffo, 100);

 	vector< vector<char>> env = create_environment(dimention);
	env = set_diagonal(env);

	char * arr = vector_to_array(env, dimention);
	char * b_arr = array_copy(arr, full_dimention);

	int	        buffsize = 10000;
	char       *sendbuff,*recvbuff;
	sendbuff=(char *)malloc(sizeof(char)*buffsize);
	recvbuff=(char *)malloc(sizeof(char)*buffsize);

	for(int i=0;i<buffsize;i++){
     sendbuff[i]=0;
   	}

	//print_array(arr, full_dimention);

	// int test = 0;
	// if (test == 1){

	// 	cout << "ITERATIONS TEST" << endl;

	// 	int iterations = 13;

	// 	char * b_arr = array_copy(arr, full_dimention);

	// 	while (iterations > 0){

	// 		char * c_arr = array_copy(b_arr, full_dimention);

	// 		for (int i = 0; i < full_dimention; ++i){

	// 			int neighbours = check_neighbours(b_arr, dimention, i);

	// 			if (b_arr[i] == 0){
	// 				if (neighbours == 1 || neighbours == 2){
	// 					c_arr[i] = 1;
	// 				}
	// 			}
	// 			else {
	// 				if (neighbours > 2 || neighbours < 1){
	// 					c_arr[i] = 0;
	// 				}
	// 			}
	// 		}
	// 		cout << iterations << endl;
	// 	 	print_array(c_arr, full_dimention);

	// 	 	copy(c_arr, c_arr + (full_dimention), b_arr);
	// 	 	--iterations;
	// 	 	free(c_arr);

	// 	}
	// }

	// CORRECT ABOVE

	// MPI INIT
	MPI_Init(&argc,&argv);

	int numprocs;
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	int id;
	MPI_Comm_rank(MPI_COMM_WORLD,&id);

	int iterations = 200;

	while (iterations > 0){
		if (id == 1){
			
			//MPI_Irecv(&buffc,full_dimention,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
			int ierr=MPI_Irecv(b_arr,buffsize,MPI_CHAR,0,array_broadcast,MPI_COMM_WORLD,&recv_request);
			ierr=MPI_Wait(&recv_request,&status);
			//print_array(recvbuff, full_dimention);
			//cout << "Buff received" << endl;

		}
		if (id == 0){

			//MPI_Isend(&arr,full_dimention,MPI_CHAR,1,tag,MPI_COMM_WORLD,&request);
			int ierr=MPI_Isend(arr,buffsize,MPI_CHAR, 1,array_broadcast,MPI_COMM_WORLD,&send_request);
			//print_array(arr, full_dimention);
			ierr=MPI_Wait(&send_request,&status);
			//cout << "Buff sent" << endl;
		}
		
		//MPI_Wait(&request,&status);

		
   		

		if (id == 1){
			--iterations;
			
			//print_array(b_arr, full_dimention);
			// for (int i = id; i < full_dimention/dimention; i+=numprocs){
			// 	cout << "P1 Checking " << i << endl;
			// 	for (int j = 0; j < dimention; ++j){
			// 		cout << "\t -> " << (i*dimention)+j << endl; 
			// 	}
			// }
			int division = full_dimention/numprocs;

			char * local_arr = new char[division];

			int index = 0;
			for (int i = id * division; i < ((id + 1)*division); ++i){
				int neighbours = check_neighbours(b_arr, dimention, i);
				local_arr[index] = 0;
				if (b_arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						local_arr[index] = 1;
					}
				}
				else {
					if (neighbours > 2 || neighbours < 1){
						local_arr[index] = 0;
					}
				}
				++index;
			}
			//cout << "Process 1" << endl;
			//print_division_array(local_arr, division, dimention);


			int ierr=MPI_Isend(local_arr,division,MPI_CHAR, 0,calculation,MPI_COMM_WORLD,&send_request);
			ierr=MPI_Wait(&send_request,&status);
			//cout << "Buff sent" << endl;

		}
		if (id == 0){
			--iterations;
			

			//print_array(arr, full_dimention);
			// for (int i = id; i < full_dimention/dimention; i+=numprocs){
			// 	cout << "P0 Checking " << i << endl;
			// 	for (int j = 0; j < dimention; ++j){
			// 		cout << "\t -> " << (i*dimention)+j << endl; 
			// 	}
			// }
			int division = full_dimention/numprocs;

			char * local_arr = new char[division];

			int index = 0;
			for (int i = id * division; i < ((id + 1)*division); ++i){
				int neighbours = check_neighbours(arr, dimention, i);
				local_arr[index] = 0;
				if (arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						local_arr[index] = 1;
					}
				}
				else {
					if (neighbours > 2 || neighbours < 1){
						local_arr[index] = 0;
					}
				}
				++index;
			}
			//cout << "Process 0" << endl;
			


			char * received_arr = new char[division];

			int ierr=MPI_Irecv(received_arr,division,MPI_CHAR,1,calculation,MPI_COMM_WORLD,&recv_request);

			ierr=MPI_Wait(&recv_request,&status);

			//cout << 0 << endl;
			//print_division_array(local_arr, division, dimention);
			//cout << 1 << endl;
			//print_division_array(received_arr, division, dimention);

			char * next_arr = new char[full_dimention];
			for (int i = 0; i < full_dimention; ++i){
				next_arr[i] = -1;
			}

			for (int i = 0; i < division; ++i){
				next_arr[i] = local_arr[i];
			}
			for (int i = 5000,j=0; i < division+5000; ++i,++j){
				next_arr[i] = received_arr[j];
			}

			print_array(next_arr, full_dimention);

			arr = array_copy(next_arr, full_dimention);

		}



		//--iterations;
	}

	MPI_Finalize();

	return 0;







 	// MPI INIT
	MPI_Init(&argc,&argv);

	//int numprocs;
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	//int id;
	MPI_Comm_rank(MPI_COMM_WORLD,&id);

	 
	if(id == 0){
		//MPI_Isend(&buffi,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
		//MPI_Isend(&buffi,count,MPI_CHAR,1,tag,MPI_COMM_WORLD,&request);
		//MPI_Isend(&buffi,count,MPI_CHAR,2,tag,MPI_COMM_WORLD,&request);
		//MPI_Isend(&buffi,count,MPI_CHAR,3,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 0){
		//MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 1){
		//MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 2){
		//MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 3){
		//MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}

	//MPI_Wait(&request,&status);
	if(id == 0){
		printf("processor %d sent %d\n",id,(int)buffi[0]);
		printf("processor %d sent %d\n",id,(int)buffi[0]);
		printf("processor %d sent %d\n",id,(int)buffi[0]);
		printf("processor %d sent %d\n",id,(int)buffi[0]);
	}
	if(id == 0){
		for (unsigned int i = 0; i < count; ++i){
			printf("processor %d got %d - %d\n",id,i,(int)buffo[i]);
		}
	}
	if(id == 1){
		for (unsigned int i = 0; i < count; ++i){
			printf("processor %d got %d - %d\n",id,i,(int)buffo[i]);
		}
	}
	if(id == 2){
		for (unsigned int i = 0; i < count; ++i){
			printf("processor %d got %d - %d\n",id,i,(int)buffo[i]);
		}
	}
	if(id == 3){
		for (unsigned int i = 0; i < count; ++i){
			printf("processor %d got %d - %d\n",id,i,(int)buffo[i]);
		}
	}

	MPI_Finalize();

	return 0;
}