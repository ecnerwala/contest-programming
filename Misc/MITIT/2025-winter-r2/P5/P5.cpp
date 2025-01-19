#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;
		std::vector<std::pair<int, int>> locs(N);
		for (int i = 0; i < N; i++) {
			locs[i] = {C[i], i};
		}
		std::sort(locs.begin(), locs.end());

		struct balance_t {
			int bal;
			int color;
			int en;
		};
		int K = 0;
		std::vector<std::vector<balance_t>> starts(N);
		std::vector<balance_t> cur_balances; cur_balances.reserve(N);
		int num_colors = 0;
		for (int l = 0, r = 0; l < N; l = r) {
			int c = num_colors++;
			while (r < N && locs[r].first == locs[l].first) r++;
			for (int i = l; i < r; i++) C[locs[i].second] = c;
			int cur_x = N;
			int balance = 0;
			int st = N;
			for (int i = r-1; i >= l; i--) {
				int x = locs[i].second;
				if (balance > 0 && balance - (cur_x - (x+1)) <= 0) {
					starts[cur_x - balance].push_back({0, c, st});
				}
				balance -= cur_x - (x+1);
				if (balance <= 0) {
					balance = 0;
					st = x+1;
				}
				balance++;
				K = std::max(K, balance);
				cur_x = x;
			}
			if (balance > 0) {
				cur_balances.push_back({balance, c, st});
			}
		}

		std::map<int, std::vector<int>> alive_stacks;

		std::vector<std::vector<int>> stacks(K);

		for (int k = 0; k < K; k++) {
			alive_stacks[-1].push_back(k);
		}

		for (int i = 0; i < N; i++) {
			{
				int L = 0;
				for (int z = 0; z < int(cur_balances.size()); z++) {
					if (cur_balances[z].en > i) {
						cur_balances[L++] = cur_balances[z];
					} else {
						assert(cur_balances[z].en == i);
						assert(cur_balances[z].bal == 0);
					}
				}
				cur_balances.resize(L);
			}
			cur_balances.insert(cur_balances.end(), starts[i].begin(), starts[i].end());

			int c = C[i];

			auto get_cnt = [&](int color) -> int {
				auto it = alive_stacks.find(color);
				if (it == alive_stacks.end()) return 0;
				else return int(it->second.size());
			};

			// check the current state
			int must_color = -1;
			for (const auto& it : cur_balances) {
				assert(it.en > i);
				int cnt = get_cnt(it.color);
				assert(cnt + it.bal <= K);
				if (it.color == c) {
					continue;
				} else if (cnt + it.bal == K) {
					assert(must_color == -1);
					must_color = it.color;
				} else {
					// we're fine
				}
			}

			if (must_color != -1) {
				auto it = alive_stacks.begin();
				assert(it != alive_stacks.end());
				if (it->first == c) {
					assert(int(it->second.size()) < K);
					++it;
					assert(it != alive_stacks.end());
				}
				assert(it->first != c);
				must_color = it->first;
			}
			{
				auto it = alive_stacks.find(must_color);
				assert(it != alive_stacks.end());
				int k = it->second.back();
				it->second.pop_back();
				if (it->second.empty()) {
					alive_stacks.erase(it);
				}
				alive_stacks[c].push_back(k);
				stacks[k].push_back(c);
			}
			for (auto& it : cur_balances) {
				if (it.color == must_color) {
					assert(it.bal > 0);
					it.bal --;
				} else {
					it.bal ++;
				}
			}
		}

		cout << K << '\n';
		for (int k = 0; k < K; k++) {
			cout << stacks[k].size();
			for (int v : stacks[k]) { cout << ' ' << v+1; }
			cout << '\n';
		}
	}

	return 0;
}
