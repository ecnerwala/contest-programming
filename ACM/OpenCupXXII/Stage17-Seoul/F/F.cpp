#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int> runs;
	for (int i = 0; i < N; i++) {
		if (i == 0 || S[i] != S[i-1]) {
			runs.push_back(A[i]);
		} else {
			runs.back() = std::max(runs.back(), A[i]);
		}
	}
	if (runs.size() <= 1) {
		cout << 0 << '\n';
		exit(0);
	}
	runs.erase(runs.begin());
	runs.pop_back();

	int num_take = int(runs.size() + 1) / 2;
	std::nth_element(runs.begin(), runs.end() - num_take, runs.end());
	cout << std::accumulate(runs.end() - num_take, runs.end(), int64_t(0)) << '\n';

	return 0;
}
