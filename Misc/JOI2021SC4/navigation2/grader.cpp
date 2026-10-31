#include "Anna.h"
#include "Bruno.h"
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <vector>
#include <iostream>

namespace {

using std::exit;
using std::fprintf;
using std::printf;
using std::scanf;
using std::vector;

const int L_MAX = 1'000'000'000;
const int N_MAX = 100;

// Input
int N;
int K, Q;
int a, b;
int E[N_MAX][N_MAX];

// Other Variables
int setflag_count = 0;
bool answered[N_MAX][N_MAX];

void WrongAnswer(int code) {
	printf("Wrong Answer [%d]\n", code);
	exit(0);
}

bool check(int sr, int sc, int tr, int tc, int z) {
	bool isvalid[5] = {false, false, false, false, false};
	if (sc < tc) isvalid[0] = true;
	if (sc > tc) isvalid[1] = true;
	if (sr < tr) isvalid[2] = true;
	if (sr > tr) isvalid[3] = true;
	if (sr == tr && sc == tc) isvalid[4] = true;
	return isvalid[z];
}

} // namespace

void SetFlag(int r, int c, int value) {
	setflag_count += 1;
	if (r < 0 || c < 0 || r >= N || c >= N) WrongAnswer(1);
	if (value < 1 || value > L_MAX) WrongAnswer(2);
	if (answered[r][c] == true) WrongAnswer(3);
	E[r][c] = value;
	answered[r][c] = true;
}

int main() {
	// Step #1. Input
	if (scanf("%d", &Q) != 1) {
		fprintf(stderr, "Fatal Error: Error while reading input.\n");
		exit(1);
	}

	// Step #2. Queries
	int max_L = 0;
	for (int query = 0; query < Q; query++) {
		if (scanf("%d%d", &N, &K) != 2) {
			fprintf(stderr, "Fatal Error: Error while reading input.\n");
			exit(1);
		}
		vector<int> R(K), C(K);
		for (int i = 0; i < K; i++) {
			if (scanf("%d%d", &R[i], &C[i]) != 2) {
				fprintf(stderr, "Fatal Error: Error while reading input.\n");
				exit(1);
			}
		}
		if (scanf("%d%d", &a, &b) != 2) {
			fprintf(stderr, "Fatal Error: Error while reading input.\n");
			exit(1);
		}

		// Anna
		setflag_count = 0;
		for (int r = 0; r < N; r++) {
			for (int c = 0; c < N; c++) answered[r][c] = false;
		}
		for (int r = 0; r < N; r++) {
			for (int c = 0; c < N; c++) E[r][c] = 0;
		}
		Anna(N, K, R, C);
		if (setflag_count != N * N) WrongAnswer(4);

		for (int r = 0; r < N; r++) {
			for (int c = 0; c < N; c++) {
				std::cerr << E[r][c] << ' ';
			}
			std::cerr << '\n';
		}

		// Bruno
		for (int a = 1; a < N-1; a++) {
			for (int b = 1; b < N-1; b++) {
				std::cerr << "Testing (" << a << ',' << b << ")" << '\n';
				vector<int> value = { E[a - 1][b - 1], E[a - 1][b], E[a - 1][b + 1], E[a][b - 1], E[a][b], E[a][b + 1], E[a + 1][b - 1], E[a + 1][b], E[a + 1][b + 1] };
				vector<int> z = Bruno(K, value);
				if ((int)z.size() != K) WrongAnswer(5);
				for (int i = 0; i < K; i++) {
					if (z[i] < 0 || z[i] > 4) WrongAnswer(6);
				}
				for (int i = 0; i < K; i++) {
					if (check(a, b, R[i], C[i], z[i]) == false) {
						std::cerr << "Wrong answer: " << "(" << a << ',' << b << ") to flag " << i << " (" << R[i] << ' ' << C[i] << ")" << ": " << z[i] << '\n';
						WrongAnswer(7);
					}
				}

			}
		}

		// Calculate Maximum Value
		for (int r = 0; r < N; r++) {
			for (int c = 0; c < N; c++) {
				if (max_L < E[r][c]) max_L = E[r][c];
			}
		}
	}

	// Step #3. Output
	printf("Accepted: Maximum value = %d\n", max_L);
	return 0;
}
