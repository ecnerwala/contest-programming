#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::tuple<int, bool, int>> evts; evts.reserve(2*N);
		for (int i = 0; i < N; i++) {
			int k, a, b; cin >> k >> a >> b;
			evts.push_back({b, 0, k});
			evts.push_back({a, 1, k});
		}
		std::sort(evts.begin(), evts.end());
		std::map<int, int> alive;
		std::vector<std::array<int, 2>> edges; edges.reserve(M);
		{
			int nxt_idx = 0;
			for (auto [t, ins, k] : evts) {
				if (ins) {
					int cur = nxt_idx++;
					auto [ins_it, did_insert] = alive.insert({k, cur});
					assert(did_insert);
					{
						auto pit = ins_it;
						for (int z = 0; z <= M; z++) {
							if (pit == alive.begin()) break;
							--pit;
							edges.push_back({pit->second, cur});
						}
					}
					{
						auto nit = ins_it;
						for (int z = 0; z <= M; z++) {
							++nit;
							if (nit == alive.end()) break;
							edges.push_back({cur, nit->second});
						}
					}
				} else {
					auto it = alive.find(k);
					assert(it != alive.end());
					auto nit = alive.erase(it);
					if (M == 1 && nit != alive.begin() && nit != alive.end()) {
						auto pit = std::prev(nit);
						if (pit != alive.begin()) {
							edges.push_back({std::prev(pit)->second, nit->second});
						}
						++nit;
						if (nit != alive.end()) {
							edges.push_back({pit->second, nit->second});
						}
					}
					alive.erase(it);
				}
			}
			assert(nxt_idx == N);
		}
	}

	return 0;
}
