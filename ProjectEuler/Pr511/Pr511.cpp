#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MOD = 1e9;

const int BASE = 4321;

ll N = 1234567898765;
int V = 3537;

// N = 1234567898765
// N % BASE
// 3537
// divisors of N
// {1, 5, 41, 205, 25343, 126715, 237631, 1039063, 1188155, 5195315, 9742871, 48714355, 6022282433, 30111412165, 246913579753, 1234567898765}
// divisors of N % BASE
// {1, 5, 41, 205, 3738, 1406, 4297, 2023, 4201, 1473, 3337, 3722, 1029, 824, 3300, 3537}
vector<int> divisors = {1, 5, 41, 205, 3738, 1406, 4297, 2023, 4201, 1473, 3337, 3722, 1029, 824, 3300, 3537};

int mat[BASE];
int trans[BASE];

ll tmp[BASE];

void mult(int a[BASE], int b[BASE]) {
	for(int i = 0; i < BASE; i++) {
		tmp[i] = 0;
	}
	for(int i = 0; i < BASE; i++) {
		for(int j = 0; j < BASE; j++) {
			tmp[(i + j) % BASE] += ll(a[i]) * b[j];
			tmp[(i + j) % BASE] %= MOD;
		}
	}

	for(int i = 0; i < BASE; i++) {
		a[i] = tmp[i];
	}
}

int main() {
	mat[V] = 1;

	for(int d : divisors) {
		trans[d] ++;
	}

	while(N) {

		cerr << N << '\n';

		if(N % 2) {
			mult(mat, trans);
		}

		N /= 2;
		mult(trans, trans);
	}

	cout << mat[0] << '\n';

	return 0;
}
