#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<int> factors;
	for (int i = 1; i * i <= N; i++) {
		if (N % i == 0) {
			factors.push_back(i);
			if (i < N / i) {
				factors.push_back(N / i);
			}
		}
	}
	sort(factors.begin(), factors.end());
	assert(factors[0] == 1);
	vector<set<int>> poss(factors.size());

	poss[0].insert(0);
	for (int z = 1; z < int(factors.size()); z++) {
		int f = factors[z];
		for (int x = 0; x < z; x++) {
			if (f % factors[x] == 0) {
				int d = f / factors[x] - 1;
				for (int o : poss[x]) poss[z].insert(o + d);
			}
		}
	}

	vector<int> ans(poss.back().begin(), poss.back().end());
	cout << ans.size() << '\n';
	for (int i = 0; i < int(ans.size()); i++) {
		cout << ans[i] << " \n"[i+1==int(ans.size())];
	}

	return 0;
}
