#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<pair<int, int>> A(2*N);
	for (int i = 0; i < 2*N; i++) { cin >> A[i].first; A[i].second = i; }
	sort(A.begin(), A.end());
	int res = 0;
	for (int i = 0; i < N; i++) res += A[i].first != A[i+N].first;
	cout << res << '\n';
	for (int i = 0; i < N; i++) {
		cout << A[i].second+1 << ' ' << A[i+N].second+1 << '\n';
	}

	return 0;
}
