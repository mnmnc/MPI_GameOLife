#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <array>
#include <time.h>
#include <ctime>


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


int check_neighbours(char arr[], int dimention, int x){
	int count = 0;

	if ((x - dimention - 1) > -1 && ((int)((x - dimention - 1) / dimention)) == ((int)((x - dimention) / dimention))){
		count += arr[x - dimention - 1];
	}

	if (x - dimention > -1){
		count += arr[x - dimention];
	}

	if ((x - dimention + 1 > 0) && ((int)((x - dimention + 1) / dimention)) == ((int)((x - dimention) / dimention))){
		count += arr[x - dimention + 1];
	}

	if (count > 2){
		return 8;
	}

	if (x - 1 > -1 && ((int)((x - 1) / dimention)) == ((int)(x / dimention))){
		count += arr[x - 1];
	}

	if (count > 2){
		return 8;
	}

	if (x + 1 < dimention*dimention && ((int)((x + 1) / dimention)) == ((int)(x / dimention))){
		count += arr[x + 1];
	}

	if (count > 2){
		return 8;
	}

	if ((x + dimention - 1) < dimention*dimention && ((int)((x + dimention - 1) / dimention)) == ((int)((x + dimention) / dimention))){
		count += arr[x + dimention - 1];
	}

	if (count > 2){
		return 8;
	}

	if ((x + dimention) < dimention*dimention ){
		count += arr[x + dimention];
	}

	if (count > 2){
		return 8;
	}

	if ((x + dimention + 1) < dimention*dimention && ((int)((x + dimention + 1) / dimention)) == ((int)((x + dimention) / dimention))){
		count += arr[x + dimention + 1];
	}

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
	int full_dimention = dimention*dimention;

	// CONTROL VARIABLES
	MPI_Request request;
    MPI_Status status;
	request = MPI_REQUEST_NULL;
	MPI_Request	send_request;
	MPI_Request recv_request;

	// CREATING ENVIRONMENT
 	vector< vector<char>> env = create_environment(dimention);
	env = set_diagonal(env);

	// VECTOR TO PROPER ARRAY
	char * arr = vector_to_array(env, dimention);

	// VECTOR TERMINATION
	env.clear();

	// CAPTURE TIME
	clock_t begin_pt = clock();

	// MPI INIT
	MPI_Init(&argc,&argv);

	int numprocs;
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	int id;
	MPI_Comm_rank(MPI_COMM_WORLD,&id);

	// DYNAMIC REQUESTS
	int * req_arr[numprocs];
	for (int i = 0; i < numprocs; ++i){
		MPI_Request recv_request;
		req_arr[i] = &recv_request;
	}

	MPI_Request new_request[numprocs];
	MPI_Request new_send_requests[numprocs];

	// SET ITERATION COUNT
	int iterations = 400;

	if (id == 0){
		// VERIFY PARAMETERS CORRECTNESS
		if ( ((dimention*dimention)/numprocs)%dimention != 0){
			cout << "[ERROR] ((dimention*dimention)/numprocs)\% dimention be must equal to 0.";
			cout << "\n\tFor example:\n\t\tdimention=8000\n\t\tnumprocs=4\n";
		}
	}

	// ITERATE
	while (iterations > 0){

		// RECEIVERS RECEIVE
		if (id != 0){
			int ierr=MPI_Irecv(arr,full_dimention,MPI_CHAR,0,array_broadcast,MPI_COMM_WORLD,&recv_request);
			ierr=MPI_Wait(&recv_request,&status);
		}

		// SENDER SENDS
		if (id == 0){
			int ierr;
			for (int i = 1; i < numprocs; i++){
				ierr=MPI_Isend(arr,full_dimention,MPI_CHAR, i,array_broadcast,MPI_COMM_WORLD,&new_send_requests[i]);
			}
			for (int i = 1; i < numprocs; i++){
				ierr=MPI_Wait(&new_send_requests[i],&status);
			}
		}
		
		// RECEIVER ACTION
		if (id != 0){

			// LOCAL VARIABLES
			int index = 0;
			int division = full_dimention/numprocs;

			// CREATING LOCAL ARRAYS
			char * local_arr = new char[division];


			// RECEIVER PART OF JOB
			for (int i = id * division; i < ((id + 1)*division); ++i){

				// CHECKING NEIGHBOURS
				int neighbours = check_neighbours(arr, dimention, i);

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

			// END OF RECEIVER ACTION
			--iterations;

			
		}

		// SENDER ACTION
		if (id == 0){
			
			// LOCAL VARIABLES
			int index = 0;
			int division = full_dimention/numprocs;

			// CREATING LOCAL ARRAYS
			char * next_arr = new char[full_dimention];

			char * r_arr[numprocs];
			for (int i = 0; i < numprocs; ++i){
				char * tarr = new char[division];
				copy(arr, arr + (division), tarr);
				r_arr[i] = tarr;
			}

			// SENDER PART OF JOB
			for (int i = id * division; i < ((id + 1)*division); ++i){

				// CHECKING NEIGHBOURS
				int neighbours = check_neighbours(arr, dimention, i);

				if ((int)arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						r_arr[id][index] = 1;
					}
					else {
						r_arr[id][index] = 0;
					}
				}
				else {
					if (neighbours > 2 || neighbours < 1){
						r_arr[id][index] = 0;
					}
					else {
						r_arr[id][index] = 1;
					} 
				}
				++index;
			}

			

			// MPI RECEIVING ACTION
			int ierr;
			for (int i = 1; i < numprocs; i++){
				ierr=MPI_Irecv(r_arr[i],division,MPI_CHAR,i,calculation,MPI_COMM_WORLD,&new_request[i]);
			}

			for (int i = 1; i < numprocs; i++){
				ierr=MPI_Wait(&new_request[i],&status);
			}

			

			// FILLING NEW ARRAY WITH -1
			// TODO: REMOVE AS REDUNDANT
			// for (int i = 0; i < full_dimention; ++i){
			// 	next_arr[i] = -1;
			// }

			// FILLING NEW ARRAY
			index = 0;
			for (int i = 0; i < numprocs; ++i){
				int local_index = 0;
				for (int j = i * division; j < (i+1)*division; ++j){
					next_arr[index] = r_arr[i][local_index];
					++index;
					++local_index;
				}
			}


			// OPTIONAL PRINTING
			//print_array(next_arr, full_dimention);

			// SETTING NEW ARRAY AS CURRENT ONE
			copy(next_arr, next_arr + (full_dimention), arr);

			// CLEANUP
			free(next_arr);
			for (int i = 0; i < numprocs; ++i){
				free(r_arr[i]);
			}

			// END OF SENDER ACTIONS
			--iterations;

			if (iterations < 2){
				//print_array(arr, full_dimention);
				int count = count_living(arr, dimention);
				cout << "Living ones: " << count << endl;
			}
		}
	}

	if (id == 0){
		cout << " Time spent:\t\t" << double(clock() - begin_pt) / CLOCKS_PER_SEC << endl;
	}

	MPI_Finalize();

	return 0;
}