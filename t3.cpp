#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <math.h>
 
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

char * array_copy(char arr[], int full_dimention){
	char * new_arr = new char[full_dimention];
	copy(arr, arr + (full_dimention), new_arr);
	return new_arr;
}

int main(int argc, char *argv[])
{
	 
	int tag = 7;
	int count = 10;
	int dimention = 10;
	int full_dimention = 100;

	MPI_Status status;
	MPI_Request request;
	request = MPI_REQUEST_NULL;

	const int size = 10;

 	char buffi[100] = {};
 	char buffo[100] = {};

 	vector< vector<char>> env = create_environment(dimention);
	env = set_diagonal(env);

	char * arr = vector_to_array(env, dimention);

	print_array(arr, full_dimention);

	char * n_arr = array_copy(arr, full_dimention);

	print_array(arr, full_dimention);

	return 0;
 	// MPI INIT
	MPI_Init(&argc,&argv);

	int numprocs;
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	int id;
	MPI_Comm_rank(MPI_COMM_WORLD,&id);

	 
	if(id == 0){
		MPI_Isend(&buffi,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
		MPI_Isend(&buffi,count,MPI_CHAR,1,tag,MPI_COMM_WORLD,&request);
		MPI_Isend(&buffi,count,MPI_CHAR,2,tag,MPI_COMM_WORLD,&request);
		MPI_Isend(&buffi,count,MPI_CHAR,3,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 0){
		MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 1){
		MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 2){
		MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}
	if(id == 3){
		MPI_Irecv(&buffo,count,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request);
	}

	MPI_Wait(&request,&status);
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