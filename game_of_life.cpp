#include <iostream>
#include <vector>

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
	for (int i = 0; i < env.size(); ++i){
		for (int j = 0; j < env.at(i).size(); ++j){
			if (i == j){
				env[i][j] = 1;
			}
		}
	}
	for (int i = 0; i < env.size(); ++i){
		for (int j = 0; j < env.at(i).size(); ++j){
			if (j == (env.size() - i - 1)){
				env[i][j] = 1;
			}
		}
	}
	return env;
}

void print_array(vector <vector <int>> env){
	for (int i = 0; i < env.size(); ++i){
		for (int j = 0; j < env.at(i).size(); ++j){
			cout << env[i][j] << "";
		}
		cout << endl;
	}
}

int count_neighbours(vector <vector <int>> env, int x, int y){
	int count = 0;

	//cout << x << ":" << y << " - ";

	if (x-1 > -1 && y-1 > -1){
		count += env[x - 1][y - 1];
		//cout << env[x - 1][y - 1];
	}
	if (x-1 > -1){
		count += env[x - 1][y];
		//cout << env[x - 1][y];
	}
	if (x - 1 > -1 && y + 1 < env.size()){
		count += env[x - 1][y + 1];
		//cout << env[x - 1][y + 1];
	}
	if (y - 1 > -1){
		count += env[x][y - 1];
		//cout << env[x][y - 1];
	}
	if (y + 1 < env.size()){
		count += env[x][y + 1];
		//cout << env[x][y + 1];
	}
	if (x + 1 < env.size() && y - 1 > -1){
		count += env[x + 1][y - 1];
		//cout << env[x + 1][y - 1];
	}
	if (x + 1 < env.size()){
		count += env[x + 1][y];
		//cout << env[x + 1][y];
	}
	if (x + 1 < env.size() && y + 1 < env.size()){
		count += env[x + 1][y + 1];
		//cout << env[x + 1][y + 1];
	}
	//cout << "\t" << count << endl;
	return count;
}

vector <vector <int>> process_life(vector <vector <int>> env){
	vector <vector <int>> next_env = env;
	//print_array(env);
	for (int i = 0; i < env.size(); ++i){
		for (int j = 0; j < env.at(i).size(); ++j){
			int neighbours = count_neighbours(env, i, j);
			//cout << i << ":" << j << " neighbours count:" << neighbours << endl;
			if (env[i][j] == 0){
				//cout << "\t" << "in zero" << endl;
				if (neighbours == 1 || neighbours == 2){
					//cout << "\t" << next_env[i][j] << " --> ";
					next_env[i][j] = 1;
					//cout << next_env[i][j] << endl;
				}
			}
			else {
				//cout << "\t" << "in one" << endl;
				if (neighbours > 2 || neighbours < 1){
					//cout << "\t" << next_env[i][j] << " --> ";
					next_env[i][j] = 0;
					//cout << next_env[i][j] << endl;
				}
			}
		}
	}
	//print_array(next_env);
	return next_env;
}

int count_living(vector <vector <int>> env){
	int count = 0;
	for (int i = 0; i < env.size(); ++i){
		for (int j = 0; j < env.at(i).size(); ++j){
			count += env[i][j];
		}
	}
	return count;
}

void life(vector <vector <int>> env, int iterations){
	vector <vector <int>> current;
	current = env;
	while (iterations > 0){
		vector <vector <int>> next_env = process_life(current);
		int count = count_living(next_env);
		print_array(next_env);
		cout << count << endl << endl;
		iterations -= 1;
		current = next_env;
		next_env.clear();
	}
}

int main(){

	int dimention = 60;
	int iterations = 60;

	vector <vector <int>> env = create_environment(dimention);
	env = set_diagonal(env);
	//print_array(env);

	life(env, iterations);

	system("PAUSE");
	return 0;
}