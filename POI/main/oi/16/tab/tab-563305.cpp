#include <iostream>
#include <algorithm>

using namespace std;

typedef pair<int, int> pii;
typedef pair<int, pii> pip;

const int MAXN = 1020, MAXM = 1020;

int N, M;
int A[MAXN][MAXM], B[MAXN][MAXM];

pip aval[MAXN * MAXM];
pip bval[MAXN * MAXM];

bool go() {
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
		for(int j = 0; j < M; j++) {
			aval[i * M + j] = pip(A[i][j], pii(A[i][0], A[0][j]));
			bval[i * M + j] = pip(B[i][j], pii(B[i][c], B[r][j]));
		}
	}

	sort(aval, aval + N * M);
	sort(bval, bval + N * M);

	for(int i = 0; i < N * M; i++) {
		if(aval[i] != bval[i]) return false;
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
