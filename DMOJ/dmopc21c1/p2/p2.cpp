#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> W(N); for (auto& w : W) cin >> w;
	std::vector<int> ops; ops.reserve(3*N);

	ops.push_back(N);

	for (int z = 0; z < N; z++) {
		int i = int(std::max_element(W.begin(), W.end()) - W.begin());
		ops.push_back(-(i+1));
		if (i) ops.push_back(i);
		W.erase(W.begin() + i);
	}

	cout << ops.size() << '\n';
	for (int v : ops) {
		cout << v << '\n';
	}

	return 0;
}
