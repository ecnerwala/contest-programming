#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> A(2 * N);
	for (int i = 0; i < 2 * N; i++) {
		cin >> A[i];
	}
	sort(A.begin(), A.end());
	for (int i = 0; i < N; i++) {
		A[i] += A[2 * N - 1 - i];
	}
	A.resize(N);
	sort(A.begin(), A.end());
	cout << A[N-1] - A[0] << '\n';

	return 0;
}
