#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	for (int i = 0; i+2 < N; i++) {
		if (A[i] != A[i+1]) {
			cout << "no" << '\n';
			exit(0);
		}
	}
	if ((A[N-1] - A[N-2]) % A[N-3]) {
		cout << "no" << '\n';
		exit(0);
	}
	cout << "yes" << '\n';

	return 0;
}
