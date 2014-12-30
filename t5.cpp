#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <array>


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

int count_living(char arr[], int dimention){
	int counter = 0;
	for (int i = 0; i < dimention * dimention; ++i){
		counter += (int)arr[i];
	}
	return counter;
}

int main(int argc, char *argv[])
{
	 
	int array_broadcast = 9;
	int calculation = 8;

	// DATA SIZES
	int dimention = 8000;
	int full_dimention = 64000000;

	// CONTROL VARIABLES
	MPI_Request request;
    MPI_Status status;
	request = MPI_REQUEST_NULL;
	MPI_Request	send_request;
	MPI_Request	send_request2;
	MPI_Request	send_request3;
	MPI_Request recv_request;
	MPI_Request recv_request2;
	MPI_Request recv_request3;


	// CREATING ENVIRONMENT
 	vector< vector<char>> env = create_environment(dimention);
	env = set_diagonal(env);

	// VECTOR TO PROPER ARRAY
	char * arr = vector_to_array(env, dimention);

	// VECTOR TERMINATION
	env.clear();

	

	// MPI INIT
	MPI_Init(&argc,&argv);

	int numprocs;
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	int id;
	MPI_Comm_rank(MPI_COMM_WORLD,&id);

	// SET ITERATION COUNT
	int iterations = 400;

	// if (id == 0){
	// 	// PRINT INITIAL
	// 	print_array(arr, full_dimention);
	// }

	// ITERATE
	while (iterations > 0){

		// RECEIVERS RECEIVE
		if (id == 1){
			int ierr=MPI_Irecv(arr,full_dimention,MPI_CHAR,0,array_broadcast,MPI_COMM_WORLD,&recv_request);
			ierr=MPI_Wait(&recv_request,&status);
		}
		if (id == 2){
			int ierr=MPI_Irecv(arr,full_dimention,MPI_CHAR,0,array_broadcast,MPI_COMM_WORLD,&recv_request);
			ierr=MPI_Wait(&recv_request,&status);
		}
		if (id == 3){
			int ierr=MPI_Irecv(arr,full_dimention,MPI_CHAR,0,array_broadcast,MPI_COMM_WORLD,&recv_request);
			ierr=MPI_Wait(&recv_request,&status);
		}

		// SENDER SENDS
		if (id == 0){
			int ierr=MPI_Isend(arr,full_dimention,MPI_CHAR, 1,array_broadcast,MPI_COMM_WORLD,&send_request);
			ierr=MPI_Isend(arr,full_dimention,MPI_CHAR, 2,array_broadcast,MPI_COMM_WORLD,&send_request2);
			ierr=MPI_Isend(arr,full_dimention,MPI_CHAR, 3,array_broadcast,MPI_COMM_WORLD,&send_request3);

			ierr=MPI_Wait(&send_request,&status);
			ierr=MPI_Wait(&send_request2,&status);
			ierr=MPI_Wait(&send_request3,&status);
		}
		
		// RECEIVER ACTION
		if (id == 1){

			// LOCAL VARIABLES
			int index = 0;
			int division = full_dimention/numprocs;

			// CREATING LOCAL ARRAYS
			char * local_arr = new char[division];

			// RECEIVER PART OF JOB
			for (int i = id * division; i < ((id + 1)*division); ++i){

				// CHECKING NEIGHBOURS
				int neighbours = check_neighbours(arr, dimention, i);

				// FILLING ARRAY WITH 0
				// TODO: NECCESSARY?
				local_arr[index] = 0;
				if (arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						local_arr[index] = 1;
					}
					else {
						local_arr[index] = 0;
					}

				}
				else {
					if (neighbours > 2 || neighbours < 1){
						local_arr[index] = 0;
					}
					else {
						local_arr[index] = 1;
					} 
				}
				++index;
			}

			// SENDING EXECUTED JOB
			int ierr=MPI_Isend(local_arr, division, MPI_CHAR, 0, calculation, MPI_COMM_WORLD, &send_request);
			ierr=MPI_Wait(&send_request, &status);

			free(local_arr);
			//free(b_arr);

			// END OF RECEIVER ACTION
			--iterations;
		}

		// RECEIVER ACTION 2
		if (id == 2){

			// LOCAL VARIABLES
			int index = 0;
			int division = full_dimention/numprocs;

			// CREATING LOCAL ARRAYS
			char * local_arr = new char[division];

			// RECEIVER PART OF JOB
			for (int i = id * division; i < ((id + 1)*division); ++i){

				// CHECKING NEIGHBOURS
				int neighbours = check_neighbours(arr, dimention, i);

				// FILLING ARRAY WITH 0
				// TODO: NECCESSARY?
				local_arr[index] = 0;
				if (arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						local_arr[index] = 1;
					}
					else {
						local_arr[index] = 0;
					}

				}
				else {
					if (neighbours > 2 || neighbours < 1){
						local_arr[index] = 0;
					}
					else {
						local_arr[index] = 1;
					} 
				}
				++index;
			}

			// SENDING EXECUTED JOB
			int ierr=MPI_Isend(local_arr, division, MPI_CHAR, 0, calculation, MPI_COMM_WORLD, &send_request);
			ierr=MPI_Wait(&send_request, &status);

			free(local_arr);
			//free(b_arr);

			// END OF RECEIVER ACTION
			--iterations;
		}

		// RECEIVER ACTION 3
		if (id == 3){

			// LOCAL VARIABLES
			int index = 0;
			int division = full_dimention/numprocs;

			// CREATING LOCAL ARRAYS
			char * local_arr = new char[division];

			// RECEIVER PART OF JOB
			for (int i = id * division; i < ((id + 1)*division); ++i){

				// CHECKING NEIGHBOURS
				int neighbours = check_neighbours(arr, dimention, i);

				// FILLING ARRAY WITH 0
				// TODO: NECCESSARY?
				local_arr[index] = 0;
				if (arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						local_arr[index] = 1;
					}
					else {
						local_arr[index] = 0;
					}

				}
				else {
					if (neighbours > 2 || neighbours < 1){
						local_arr[index] = 0;
					}
					else {
						local_arr[index] = 1;
					} 
				}
				++index;
			}

			// SENDING EXECUTED JOB
			int ierr=MPI_Isend(local_arr, division, MPI_CHAR, 0, calculation, MPI_COMM_WORLD, &send_request);
			ierr=MPI_Wait(&send_request, &status);

			free(local_arr);
			//free(b_arr);

			// END OF RECEIVER ACTION
			--iterations;
		}

		// SENDER ACTION
		if (id == 0){
			
			// LOCAL VARIABLES
			int index = 0;
			int division = full_dimention/numprocs;

			// CREATING LOCAL ARRAYS
			char * local_arr = new char[division];
			char * next_arr = new char[full_dimention];
			char * received_arr = new char[division];
			char * received_arr2 = new char[division];
			char * received_arr3 = new char[division];

			// SENDER PART OF JOB
			for (int i = id * division; i < ((id + 1)*division); ++i){

				// CHECKING NEIGHBOURS
				int neighbours = check_neighbours(arr, dimention, i);

				// FILLING ARRAY WITH 0
				// TODO: NECCESSARY?
				local_arr[index] = 0;

				if ((int)arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						local_arr[index] = 1;
					}
					else {
						local_arr[index] = 0;
					}
				}
				else {
					if (neighbours > 2 || neighbours < 1){
						local_arr[index] = 0;
					}
					else {
						local_arr[index] = 1;
					} 
				}
				++index;
			}

			// MPI RECEIVING ACTION
			// TODO: DYNAMIC BASED ON NUMBER OF PROCESSES
			int ierr=MPI_Irecv(received_arr,division,MPI_CHAR,1,calculation,MPI_COMM_WORLD,&recv_request);
			ierr=MPI_Irecv(received_arr2,division,MPI_CHAR,2,calculation,MPI_COMM_WORLD,&recv_request2);
			ierr=MPI_Irecv(received_arr3,division,MPI_CHAR,3,calculation,MPI_COMM_WORLD,&recv_request3);
			ierr=MPI_Wait(&recv_request,&status);
			ierr=MPI_Wait(&recv_request2,&status);
			ierr=MPI_Wait(&recv_request3,&status);

			// FILLING NEW ARRAY WITH -1
			// TODO: REMOVE AS REDUNDANT
			for (int i = 0; i < full_dimention; ++i){
				next_arr[i] = -1;
			}

			// FILLING NEW ARRAY
			// TODO: DYNAMIC BASED ON NUMBER OF PROCESSES
			for (int i = 0 * division; i < (0+1)*division; ++i){
				next_arr[i] = local_arr[i];
			}
			for (int i = 1 * division ,j=0; i < (1+1) * division; ++i,++j){
				next_arr[i] = received_arr[j];
			}
			for (int i = 2 * division ,j=0; i < (2+1) * division; ++i,++j){
				next_arr[i] = received_arr2[j];
			}
			for (int i = 3 * division ,j=0; i < (3+1) * division; ++i,++j){
				next_arr[i] = received_arr3[j];
			}

			// OPTIONAL PRINTING
			//print_array(next_arr, full_dimention);

			// SETTING NEW ARRAY AS CURRENT ONE
			//arr = array_copy(next_arr, full_dimention);
			copy(next_arr, next_arr + (full_dimention), arr);

			// CLEANUP
			free(local_arr);
			free(received_arr);
			free(received_arr2);
			free(received_arr3);
			free(next_arr);

			// END OF SENDER ACTIONS
			--iterations;

			if (iterations < 2){
				//print_array(arr, full_dimention);
				int count = count_living(arr, dimention);
				cout << "Living ones: " << count << endl;
			}
		}
	}

	MPI_Finalize();

	return 0;
}