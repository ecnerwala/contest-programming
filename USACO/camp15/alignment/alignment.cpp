#include <bits/stdc++.h>
#include "alignment.h"
using namespace std;

typedef unsigned char num;

const int MAXN = 1500;
const int MAXM = MAXN * 20;
int N;
int M;

int A[MAXN][MAXN];
int B[MAXN][MAXN];

long long prefA[MAXN][MAXN];
long long prefB[MAXN][MAXN];

int main() {
	cin >> N;
	M = N + (N / 2) + (N / 2);

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			cin >> A[i][j];
			A[i][j] -= 128;
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			cin >> B[i][j];
			B[i][j] -= 128;
		}
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			prefA[i + 1][j + 1] = prefA[i][j + 1] + prefA[i + 1][j] - prefA[i][j] + A[i][j] * A[i][j];
			prefB[i + 1][j + 1] = prefB[i][j + 1] + prefB[i + 1][j] - prefB[i][j] + B[i][j] * B[i][j];
		}
	}

	vector<double> a, b;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N / 2; j++) {
			a.push_back(0);
			b.push_back(0);
		}
		for(int j = 0; j < N; j++) {
			a.push_back(A[i][j]);
			b.push_back(B[i][j]);
		}
		for(int j = 0; j < N / 2; j++) {
			a.push_back(0);
			b.push_back(0);
		}
	}
	reverse(b.begin(), b.end());

	vector<double> conv = convolve(a, b);
	
	double res = 1e99;

	int L = M * N;

	assert(int(a.size()) == L);
	assert(int(b.size()) == L);

	for(int x = -(N / 2); x <= N / 2; x++) {
		for(int y = -(N / 2); y <= N / 2; y++) {
			double v = - 2 * conv[L - 1 - (M * x + y)];

			int xmi = max(0, -x);
			int xma = min(0, -x) + N;
			int ymi = max(0, -y);
			int yma = min(0, -y) + N;

			v += prefA[xma][yma] + prefA[xmi][ymi] - prefA[xmi][yma] - prefA[xma][ymi];

			xmi += x;
			xma += x;
			ymi += y;
			yma += y;

			v += prefB[xma][yma] + prefB[xmi][ymi] - prefB[xmi][yma] - prefB[xma][ymi];

			v /= (xma - xmi);
			v /= (yma - ymi);

			res = min(res, v);
		}
	}

	cout << fixed << setprecision(10) << res << '\n';
}
