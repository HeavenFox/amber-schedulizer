#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

const int TOTAL_DANCES = 14;

int cardinal[TOTAL_DANCES][TOTAL_DANCES];

int positional[TOTAL_DANCES][TOTAL_DANCES][TOTAL_DANCES];

int current_state[TOTAL_DANCES];

int current_score = 0;

int best_state[TOTAL_DANCES];

int best_score = -10000;

int PROB_1 = 1000;

const int INITIAL_STATE[] = {10, 0, 13, 1, 8, 2, 7, 6, 3, 9, 4, 11, 5, 12};

const int REPORTING_INTERVAL = 20;

const double K_B = 10;
const double INIT_T = 50000.0;
const double T_STEP = -0.005;

double current_temp = INIT_T;

double get_temperature() {
	current_temp += T_STEP;
	return current_temp;
}

int get_prob(int s1, int s2, double t) {
	if (s2 > s1) {
		return PROB_1;
	} else {
		return PROB_1 * exp((s1-s2) / (K_B * t));
	}
}

void swap(int& a, int& b) {
	int c;
	c = a;
	a = b;
	b = c;
}


int evaluate(int* state) {
	int score = 0;
	for (int i=0; i < TOTAL_DANCES; i++) {
		score += cardinal[state[i]][i];
	}
	for (int i=0; i< TOTAL_DANCES; i++) {
		for (int j=i+1;j < TOTAL_DANCES; j++) {
			score += positional[state[i]][state[j]][j-i];
		}
	}
	return score;
}

void import() {
	ifstream fin_cardinal("cardinal.txt");
	int a,b,c,d;
	while (fin_cardinal>>a>>b>>c) {
		cardinal[a][b] = c;
	}

	ifstream fin_positional("positional.txt");
	while (fin_positional>>a>>b>>c>>d) {
		positional[a][b][c] = d;
	}
}

void init_state() {
	memcpy(current_state, INITIAL_STATE, sizeof(INITIAL_STATE));
	current_score = evaluate(current_state);
}

void sa() {
	double t;
	int ticks = 0;
	while ((t = get_temperature()) > 0.0) {
		int i = rand() % TOTAL_DANCES;
		int j;
		while ((j = rand() % TOTAL_DANCES) == i) {}

		swap(current_state[i], current_state[j]);
		int score = evaluate(current_state);
		if (rand() % PROB_1 < get_prob(current_score, score, t)) {
			current_score = score;
		} else {
			// swap back
			swap(current_state[i], current_state[j]);
		}
		if (current_score > best_score) {
			for (int i=0;i<TOTAL_DANCES;i++) {
				best_state[i] = current_state[i];
			}
			best_score = current_score;
		}
		if (++ticks >= REPORTING_INTERVAL) {
			ticks = 0;
			cout<<"\rTEMP: "<<t<<"  CURRENT SCORE: "<<current_score<<"  BEST SCORE: "<<best_score<<"    ";
		}
	}
}

void report() {
	cout<<endl<<"BEST SCORE: "<<best_score<<endl;
	cout<<"BEST SOLUTION"<<endl;
	for (int i=0;i<TOTAL_DANCES;i++) {
		cout<<best_state[i]<<' ';
	}
	cout<<endl;
}

int main() {
	srand(time(NULL));
	import();
	init_state();
	sa();
	report();
}