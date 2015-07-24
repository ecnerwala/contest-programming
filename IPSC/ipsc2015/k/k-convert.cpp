#include<bits/stdc++.h>
using namespace std;

const double LEN = 1.5;
const int N = 192 / 3 * 2;

const int MAXN = N * 2;

double pitch[MAXN];
double volume[MAXN][2];

int main() {
	ifstream dat("k2.dat");
	while(true) {
		double t, l, r;
		if(!(dat >> t >> l >> r)) break;
		int n = t / LEN;
		assert(0 <= n && n < N);
		volume[n][0] = max(volume[n][0], abs(l));
		volume[n][1] = max(volume[n][1], abs(r));
	}
	ifstream aub("k2.pitch");
	while(true) {
		double t, p;
		if(!(aub >> t >> p)) break;
		int n = t / LEN;
		assert(0 <= n && n < N);
		if(p < 1e-9) {
			continue;
		}
		pitch[n] = p;
	}

	ofstream infile("k2.in");
	infile << N << '\n';
	for(int i = 0; i < N; i++) {
		infile << pitch[i] << ' ' << volume[i][0] << ' ' << volume[i][1] << '\n';
	}
	return 0;
}
