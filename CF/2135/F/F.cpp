#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> ch(N);
		for (auto& [l, r] : ch) {
			cin >> l >> r; l--, r--;
			assert((l == -1) == (r == -1));
		}
		std::vector<std::vector<int>> by_gen(N);
		int G = 1 + [&](this auto&& self, int cur) -> int {
			int gen;
			if (ch[cur][0] == -1) {
				gen = 0;
			} else {
				int gl = self(ch[cur][0]);
				int gr = self(ch[cur][1]);
				gen = std::max(gl, gr+1);
			}
			by_gen[gen].push_back(cur);
			return gen;
		}(0);
		while (!by_gen.empty() && by_gen.back().empty()) by_gen.pop_back();
		assert(int(by_gen.size()) == G);
	}

	return 0;
}
