#include <iostream>
#include <vector>
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

char * array_copy(char arr[], int dimention){
	char * new_arr = new char[dimention * dimention];
	copy(arr, arr + (dimention * dimention), new_arr);
	return new_arr;
}

int count_living(char arr[], int dimention){
	int counter = 0;
	for (int i = 0; i < dimention * dimention; ++i){
		counter += arr[i];
	}
	return counter;
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

char * process_life(char arr[], int dimention){
	char * new_array = array_copy(arr, dimention);

	for (int i = 0; i < dimention * dimention; ++i){
		int neighbours = check_neighbours(arr, dimention, i);

		if (arr[i] == 0){
			if (neighbours == 1 || neighbours == 2){
				new_array[i] = 1;
			}
		}
		else {
			if (neighbours > 2 || neighbours < 1){
				new_array[i] = 0;
			}
		}
	}

	return new_array;
}

void life(char arr[], int dimention, int iterations){
	char * current = new char[dimention * dimention];
	copy(arr, arr + (dimention * dimention), current);
	free(arr);
	int max_iterations = iterations;

	while (iterations > 0){
		cout << "Iterations left: " << iterations << endl;
		char * next_array = process_life(current, dimention);

		// PRINTING ARRAY
		cout << endl;
		for (int i = 0; i < (dimention*dimention); ++i){
			cout << (int)next_array[i];
			if (((i + 1) % dimention) == 0 && i / 1 != 0){
				cout << endl;
			}
		}

		copy(next_array, next_array + (dimention * dimention), current);
		--iterations;
		free(next_array);

		// if (iterations < 6) {
		// 	int count = count_living(current, dimention);
		// 	cout << "[NFO] Living in iteration "<< iterations << "/" << max_iterations <<" : " << count << endl;
		// }
	}
}

int main(){
	
	int dimention = 10;
	int iterations = 10;

	vector< vector<char>> env = create_environment(dimention);
	env = set_diagonal(env);

	char * arr = vector_to_array(env, dimention);
	env.clear();
	
	

	char * r_arr[4];

	for (int i = 0; i < 4; ++i){
		char * tarr = new char[dimention * dimention];
		copy(arr, arr + (dimention*dimention), tarr);
		r_arr[i] = tarr;
	}

	for (int i = 0; i < 4; ++i){
		cout << "r_arr[" << i <<"] " << (int)r_arr[i][0] << endl;
	}


	//system("PAUSE");
	return 0;
}
