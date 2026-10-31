#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int> pivots; pivots.reserve(N);
	int cur_max = A[0];
	for (int i = 0; i < N; i++) {
		while (!pivots.empty() && pivots.back() >= A[i]) {
			pivots.pop_back();
		}
		if (A[i] >= cur_max) {
			cur_max = A[i];
			pivots.push_back(A[i]);
		}
	}

	cout << pivots.size();
	for (int i = 0; i < int(pivots.size()) && i < 100; ++i) {
		cout << ' ' << pivots[i];
	}
	cout << '\n';

	return 0;
}
