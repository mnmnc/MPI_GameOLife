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
	int count = 10;
	int dimention = 10;
	int full_dimention = 100;

	//MPI_Status status;
	//MPI_Request request;
	MPI_Request request[4];
    MPI_Status status[4];
	request[0] = MPI_REQUEST_NULL;
	request[1] = MPI_REQUEST_NULL;
	request[2] = MPI_REQUEST_NULL;
	request[3] = MPI_REQUEST_NULL;

	const int size = 10;

 	char buffi[100] = {};
 	char buffo[100] = {};

 	for (int i = 0; i < 100; ++i){
 		buffi[i] = i;
 		buffo[i] = i;
 	}

 	//print_array(buffo, 100);

 	vector< vector<char>> env = create_environment(dimention);
	env = set_diagonal(env);

	char * arr = vector_to_array(env, dimention);
	char * b_arr = array_copy(arr, full_dimention);

	//print_array(arr, full_dimention);

	int test = 0;
	if (test == 1){

		cout << "ITERATIONS TEST" << endl;

		int iterations = 13;

		char * b_arr = array_copy(arr, full_dimention);

		while (iterations > 0){

			char * c_arr = array_copy(b_arr, full_dimention);

			for (int i = 0; i < full_dimention; ++i){

				int neighbours = check_neighbours(b_arr, dimention, i);

				if (b_arr[i] == 0){
					if (neighbours == 1 || neighbours == 2){
						c_arr[i] = 1;
					}
				}
				else {
					if (neighbours > 2 || neighbours < 1){
						c_arr[i] = 0;
					}
				}
			}
			cout << iterations << endl;
		 	print_array(c_arr, full_dimention);

		 	copy(c_arr, c_arr + (full_dimention), b_arr);
		 	--iterations;
		 	free(c_arr);

		}
	}

	// CORRECT ABOVE

	// MPI INIT
	MPI_Init(&argc,&argv);

	int numprocs;
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	int id;
	MPI_Comm_rank(MPI_COMM_WORLD,&id);

	int iterations = 2;

	//while (iterations > 0){

		if (id == 0){
			MPI_Isend(&arr,full_dimention,MPI_CHAR,1,tag,MPI_COMM_WORLD,&request[0]);
			for (int i = 0; i < 100; ++i){
				cout << i << ": " << (int) arr[i] << endl;
			}
		}
		if (id == 1){
			MPI_Irecv(&buffo,full_dimention,MPI_CHAR,0,tag,MPI_COMM_WORLD,&request[1]);
		}
		MPI_Wait(&request[0],&status[0]);
		MPI_Wait(&request[1],&status[1]);
		if (id == 1){
			--iterations;
			//cout << " Array "<< iterations <<" has been received." << endl;
			//print_array(buffo, 100);

			for (int i = 0; i < 100; ++i){
				cout << i << ": " << (int) buffo[i] << endl;
			}
		}

		--iterations;
	//}

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