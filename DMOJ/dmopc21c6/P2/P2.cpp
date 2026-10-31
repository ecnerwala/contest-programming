#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	string S; cin >> S;
	std::vector<int> runs; runs.reserve(N);
	int cur = 0;
	for (char c : S) {
		if (c == '1') runs.push_back(std::exchange(cur, 0));
		else cur++;
	}
	runs.push_back(cur);
	K = std::min(K, int(runs.size()));
	std::nth_element(runs.begin(), runs.end() - K, runs.end());
	cout << std::accumulate(runs.end() - K, runs.end(), 0) << '\n';

	return 0;
}
