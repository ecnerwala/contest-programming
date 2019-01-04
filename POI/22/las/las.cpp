#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
int C[MAXN];
bool R[MAXN];

int prv(int i) {
	return i == 0 ? N-1 : i-1;
}
int nxt(int i) {
	return i == N-1 ? 0 : i+1;
}

void check(int i) {
	if (R[i]) return;
	int lval = C[i] / (R[prv(i)] + 1);
	int rval = C[nxt(i)] / (!R[nxt(i)] + 1);
	if (rval > lval) {
		// we should swap
		R[i] = true;
		check(prv(i));
		check(nxt(i));
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> C[i];
		C[i] *= 2;
	}
	for (int i = 0; i < N; i++) {
		R[i] = false;
	}

	for (int i = 0; i < N; i++) {
		check(i);
	}

	for (int i = 0; i < N; i++) {
		cout << (R[i] ? nxt(i) : i) + 1 << " \n"[i==N-1];
	}

	return 0;
}
