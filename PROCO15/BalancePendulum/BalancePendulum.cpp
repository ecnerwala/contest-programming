#include<bits/stdc++.h>
using namespace std;

void run_simulation() {
	system("java -jar pendulum.jar -voff");
}

const double EPS = 1e-9;

const int INF = 20000;

bool r = false;
int t1 = INF, t2 = INF, t3 = INF, t4 = INF;

bool goright(int ang, int vel) {
	if(vel == 0 && ang == 0) return r;
	else if(abs(vel) == 0) {
		return ang > 0;
	} if(abs(vel) == 1) {
		if(abs(ang) >= t1) {
			return ang > 0;
		} else {
			return vel > 0;
		}
	} else if(abs(vel) == 2) {
		if(abs(ang) >= t2) {
			return ang > 0;
		} else {
			return vel > 0;
		}
	} else if(abs(vel) == 3) {
		if(abs(ang) >= t3) {
			return ang > 0;
		} else {
			return vel > 0;
		}
	} else if(abs(vel) == 4) {
		if(abs(ang) >= t4) {
			return ang > 0;
		} else {
			return vel > 0;
		}
	} else {
		return vel > 0;
	}
}

void generate_actions(string file = "actions.txt") {
	ofstream actions(file);

	for(int ang = -90; ang <= 90; ang++) {
		for(int vel = -30; vel <= 30; vel++) {
			actions << (ang + 90) << ' ' << vel << ' ' << (goright(ang, vel) ? 'R' : 'L') << '\n';
		}
	}
	actions.close();
}

int get_score() {
	generate_actions();
	system("java -jar pendulum.jar 2000");
	ifstream score("score.txt");
	int val;
	score >> val;
	score.close();
	return val;
}

void test() {
	int v = 0;
	for(int d = 0; d < 2; d++) {
		for(int i = 9; i <= 13; i++) {
			for(int j = i; j <= 13; j++) {
				for(int k = j; k <= 13; k++) {
					for(int l = k; l <= 13; l++) {
						r = d, t1 = i, t2 = j, t3 = k, t4 = l;
						//string f = "actions" + to_string(d) + "-" + to_string(t1) + "-" + to_string(t2) + "-" + to_string(t3) + "-" + to_string(t4) + ".txt";
						string f = to_string(v) + ".txt";
						v++;
						generate_actions(f);
					}
				}
			}
		}
	}
}

void go() {
	t1 = 11, t2 = 11, t3 = 12, t4 = 13;
	generate_actions();
}

int main() {
	ios_base::sync_with_stdio(0);

	test();

	//go();

	return 0;
}
