#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N = 7;

	std::mt19937 mt(48);

	std::vector<std::array<int, 2>> freq(2*N, {0, 0});
	std::vector<int> vals(2*N, 0);
	[&](this auto&& self, int idx) -> bool {
		if (idx == N * N) {
			auto v = vals;
			std::sort(v.begin(), v.end());
			if (std::unique(v.begin(), v.end()) == v.end()) {
				cerr << "done" << '\n';
				for (auto x : vals) cerr << x << '\n';
			}
		}
		int i = idx / N, j = idx % N;
		int val = int(mt() & 1);
		for (int z = 0; z < 2; z++, val = !val) {
			vals[0+i] += (val << j);
			vals[N+j] += (val << i);

			freq[0+i][val]++;
			freq[N+j][val]++;

			if (freq[0+i][val] <= (N+1)/2 && freq[N+j][val] <= (N+1)/2) {
				if (self(idx+1)) {
					return true;
				}
			} 

			freq[0+i][val]--;
			freq[N+j][val]--;

			vals[0+i] -= (val << j);
			vals[N+j] -= (val << i);
		}
		return false;
	}(0);

	return 0;
}
