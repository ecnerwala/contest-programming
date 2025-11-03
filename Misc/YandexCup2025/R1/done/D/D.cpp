#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> ans(N);
	for (int b = 0; b < 9; b++) {
		std::vector<int> pos;
		for (int i = 0; i < N; i++) {
			if ((i >> b) & 1) pos.push_back(i);
		}
		if (pos.empty()) continue;
		cout << "CHECK" << ' ' << pos.size();
		for (int v : pos) cout << ' ' << v+1;
		cout << '\n' << flush;
		for (int z = 0; z < int(pos.size()); z++) {
			int k; cin >> k; k--;
			ans[k] |= (1 << b);
		}
	}
	cout << "RESULT";
	for (int v : ans) cout << ' ' << v+1;
	cout << '\n' << flush;

	return 0;
}
