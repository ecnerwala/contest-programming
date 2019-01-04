#include <iostream>
#include <algorithm>

using namespace std;

typedef pair<int, int> pii;
typedef pair<int, pii> pip;

const int MAXN = 1020, MAXM = 1020;
const int MINV = -1e6 - 10;
const int MAXV = 1e6 + 10;

int N, M;
int A[MAXN][MAXM], B[MAXN][MAXM];

int rowNum[MAXV - MINV + 1];
int colNum[MAXV - MINV + 1];

bool go() {
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			A[i][j] -= MINV;
			B[i][j] -= MINV;
		}
	}

	int v = A[0][0];
	int r = -1, c = -1;

	for(int i = 0; r == -1 && i < N; i++) {
		for(int j = 0; r == -1 && j < M; j++) {
			if(B[i][j] == v) {
				r = i, c = j;
			}
		}
	}
	if(r == -1) return false;

	for(int i = 0; i < N; i++) {
		rowNum[A[i][0]] = i;
	}
	for(int j = 0; j < M; j++) {
		colNum[A[0][j]] = j;
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			if(B[i][j] != A[rowNum[B[i][c]]][colNum[B[r][j]]]) return false;
		}
	}

	return true;
}

int main() {
	int T; cin >> T;
	for(int t = 0; t < T; t++) {
		cin >> N >> M;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < M; j++) {
				cin >> A[i][j];
			}
		}
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < M; j++) {
				cin >> B[i][j];
			}
		}

		bool res = go();
		cout << (res ? "TAK" : "NIE") << '\n';
	}
}
