#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int A[MAXN];
int B[MAXN];

bool go() {
	int N;
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	for (int i = N / 2 - 1, j = (N % 2); i >= 0; i--, j += 2) {
		B[j] = A[i];
	}
	for (int i = N / 2, j = 1-(N % 2); i < N; i++, j += 2) {
		B[j] = A[i];
	}
	int l = 1, r = N;
	for (int j = 0; j < N; j++) {
		if (B[j] == l) {
			l++;
		} else if (B[j] == r) {
			r--;
		} else return false;
	}
	return true;
}
int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		bool res = go();
		cout << "Case #" << case_num << ": " << (res ? "YES" : "NO") << '\n';
	}

	return 0;
}
