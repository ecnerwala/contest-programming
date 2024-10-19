#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	int t = 0;
	while (t+K < N && A[t+K] <= A[t]) {
		t++;
	}
	A.erase(A.begin() + t, A.begin() + t + K);
	for (int i = 0; i < N-K; i++) {
		cout << A[i] << " \n"[i+1==N-K];
	}

	return 0;
}
