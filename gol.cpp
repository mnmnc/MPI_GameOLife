#include <iostream>
#include <vector>
#include <array>

using namespace std;

vector <vector <int>> create_environment(int dimention){
	vector <vector <int>> env;

	for (int i = 0; i < dimention; ++i){
		vector <int> row;
		for (int j = 0; j < dimention; ++j){
			row.push_back(0);
		}
		env.push_back(row);
	}

	return env;
}

vector <vector <int>> set_diagonal(vector <vector <int>> env){
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

int * vector_to_array(vector<vector<int>> env, int dimention){
	int * arr = new int[dimention * dimention];
	int index = 0;
	for (unsigned int i = 0; i < env.size(); ++i){
		for (unsigned int j = 0; j < env[0].size(); ++j){
			arr[index] = env[i][j];
			++index;
		}
	}
	return arr;
}

int * array_copy(int arr[], int dimention){
	//cout << endl;
	//for (int i = 0; i < (dimention*dimention); ++i){
	//	cout << arr[i];
	//	if (((i + 1) % dimention) == 0 && i / 1 != 0){
	//		cout << endl;
	//	}
	//}

	int * new_arr = new int[dimention * dimention];

	copy(arr, arr + (dimention * dimention), new_arr);

	//for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i){
	//	new_arr[i] = arr[i];
	//}

	//cout << endl;
	//for (int i = 0; i < (dimention*dimention); ++i){
	//	cout << new_arr[i];
	//	if (((i + 1) % dimention) == 0 && i / 1 != 0){
	//		cout << endl;
	//	}
	//}

	return new_arr;
}

int check_neighbours(int arr[], int dimention, int x){
	int count = 0;
	int a, b, c, d, e, f, g, h = 0;
	a = b = c = d = e = f = g = h;
	//x - d - 1 
	//x - d
	//x - d + 1
	//x - 1
	//x + 1
	//x + d - 1
	//x + d + 1

	if ((x - dimention - 1) > -1 && ((int)((x - dimention - 1) / dimention)) == ((int)((x - dimention) / dimention))){
		//cout << "a" << endl;
		a = arr[x - dimention - 1];
	}

	if (x - dimention > -1){
		//cout << "b" << endl;
		b = arr[x - dimention];
	}

	if ((x - dimention + 1 > 0) && ((int)((x - dimention + 1) / dimention)) == ((int)((x - dimention) / dimention))){
		//cout << "c" << endl;
		c = arr[x - dimention + 1];
	}

	if (x - 1 > -1 && ((int)((x - 1) / dimention)) == ((int)(x / dimention))){
		//cout << "d" << endl;
		d = arr[x - 1];
	}

	if (x + 1 < dimention*dimention && ((int)((x + 1) / dimention)) == ((int)(x / dimention))){
		//cout << "e" << endl;
		e = arr[x + 1];
	}

	if ((x + dimention - 1) < dimention*dimention && ((int)((x + dimention - 1) / dimention)) == ((int)((x + dimention) / dimention))){
		//cout << "f" << endl;
		f = arr[x + dimention - 1];
	}

	if ((x + dimention) < dimention*dimention ){
		//cout << "g" << endl;
		g = arr[x + dimention];
	}

	if ((x + dimention + 1) < dimention*dimention && ((int)((x + dimention + 1) / dimention)) == ((int)((x + dimention) / dimention))){
		//cout << "h" << endl;
		h = arr[x + dimention + 1];
	}

	//cout << "a:" << a << endl;
	//cout << "b:" << b << endl;
	//cout << "c:" << c << endl;
	//cout << "d:" << d << endl;
	//cout << "e:" << e << endl;
	//cout << "f:" << f << endl;
	//cout << "g:" << g << endl;
	//cout << "h:" << h << endl;

	count = a + b + c + d + e + f + g + h;

	return count;
}

int * process_life(int arr[], int dimention){
	int * new_array = array_copy(arr, dimention);

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

void life(int arr[], int dimention, int iterations){
	int * current = new int[dimention * dimention];
	copy(arr, arr + (dimention * dimention), current);

	while (iterations > 0){
		int * next_array = process_life(current, dimention);

		// PRINTING ARRAY
		cout << endl;
		for (int i = 0; i < (dimention*dimention); ++i){
			cout << next_array[i];
			if (((i + 1) % dimention) == 0 && i / 1 != 0){
				cout << endl;
			}
		}

		copy(next_array, next_array + (dimention * dimention), current);
		--iterations;
	}

}

int main(){
	
	int dimention = 60;
	int iterations = 50;

	vector< vector<int>> env = create_environment(dimention);
	env = set_diagonal(env);

	int * arr = vector_to_array(env, dimention);

	// PRINTING ARRAY
	
	for (int i = 0; i < (dimention*dimention); ++i){
		cout << arr[i];

		if (((i + 1) % dimention) == 0 && i / 1 != 0){
			cout << endl;
		}
	}

	//int c = check_neighbours(arr, dimention, 24);
	//cout << c << endl;



	
	//int * n_arr = process_life(arr, dimention);

	//// PRINTING ARRAY
	//for (int i = 0; i < (dimention*dimention); ++i){
	//	cout << n_arr[i];
	//	if (((i + 1) % dimention) == 0 && i / 1 != 0){
	//		cout << endl;
	//	}
	//}
	

	life(arr, dimention, iterations);

	system("PAUSE");
	return 0;
}