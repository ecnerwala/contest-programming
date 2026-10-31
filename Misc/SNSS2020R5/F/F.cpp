#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	vector<int> A(N);
	for (int& a : A) cin >> a;
	sort(A.begin(), A.begin()+M);
	sort(A.begin()+M, A.end());
	int j = M;
	for (int i = 0; i < M; i++) {
		for (int cnt = 0; cnt < 4 && j < N && A[j] <= A[i]; cnt++) {
			j++;
		}
	}
	cout << N-j << '\n';

	return 0;
}
