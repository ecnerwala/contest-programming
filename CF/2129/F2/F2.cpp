#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> freq(30, 2);
	freq[29] = 0;
	std::set<std::vector<int>> locs;
	std::mt19937 mt(50);
	auto did_find = [&](this auto&& self) -> bool {
		//cerr << "dfs " << int(locs.size()) << '\n';
		if (int(locs.size()) == 18) return true;

		std::vector<int> legal; legal.reserve(30);
		for (int i = 0; i < 30; i++) {
			if (freq[i] > 0) legal.push_back(i);
		}

		assert(!legal.empty());

		std::set<std::array<int, 2>> tried;
		while (int(tried.size()) < (int(legal.size()) - 1) * (int(legal.size()) - 2) / 2) {
			int i = std::uniform_int_distribution<int>(1, int(legal.size())-1)(mt);
			int j = std::uniform_int_distribution<int>(1, int(legal.size())-1)(mt);
			if (i == j) continue;
			if (i > j) std::swap(i, j);
			{
				auto [_, did_insert] = tried.insert({i, j});
				if (!did_insert) continue;
			}

			std::vector<int> cnd({legal[0], legal[i], legal[j]});
			assert(std::is_sorted(cnd.begin(), cnd.end()));
			{
				auto [_, did_insert] = locs.insert(cnd);
				if (!did_insert) continue;
			}
			for (auto v : cnd) freq[v]--;

			if (self()) return true;

			for (auto v : cnd) freq[v]++;
			locs.erase(cnd);
		}

		return false;
	}();
	assert(did_find);

	std::vector<std::vector<int>> loc_list(locs.begin(), locs.end());
	for (int z = 0; z < 2; z++) {
		loc_list.push_back({});
		for (int i = 0; i < 29; i++) {
			loc_list.push_back({i});
			for (int j = i+1; i < 29; i++) {
				loc_list.push_back({i, j});
			}
		}
	}
	std::map<std::vector<int>, std::vector<int>> loc_map;
	for (int i = 0; i < int(loc_list.size()); i++) {
		loc_map[loc_list[i]].push_back(i);
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> results(N);
		for (int q = 0; q < 29; q++) {
			std::vector<int> q_inds; q_inds.reserve(q == 0 ? 300 : 60);
			for (int i = 0; i < N; i++) {
				if (std::find(loc_list[i].begin(), loc_list[i].end(), q) != loc_list[i].end()) {
					q_inds.push_back(i);
				}
			}
			if (q_inds.empty()) {
				continue;
			}
			cout << 2 << ' ' << int(q_inds.size());
			for (auto i : q_inds) {
				cout << ' ' << i+1;
			}
			cout << '\n' << flush;
			assert(int(q_inds.size()) <= 60);
			for (int z = 0; z < int(q_inds.size()); z++) {
				int v; cin >> v; v--;
				results[v].push_back(q);
			}
		}
		std::map<std::vector<int>, std::vector<int>> result_vals;
		for (int i = 0; i < N; i++) {
			result_vals[results[i]].push_back(i);
		}
		std::vector<int> ans(N, -1);
		std::vector<std::array<int, 2>> to_swap(N, {-1, -1});
		std::vector<int> cnds; cnds.reserve(436);
		for (auto [k, v] : result_vals) {
			assert(!v.empty());
			auto answers = loc_map[k];
			assert(int(answers.size()) >= int(v.size()));
			if (int(v.size()) >= 2) {
				assert(int(v.size()) == 2);
				if (std::uniform_int_distribution(0, 1)(mt)) {
					std::swap(v[0], v[1]);
				}
				assert(loc_map[k][0] < loc_map[k][1]);
				ans[v[0]] = loc_map[k][0];
				ans[v[1]] = loc_map[k][1];
				// if loc_map[k][1] shows up in cnds, then we should swap them
				cnds.push_back(v[0]);
				to_swap[loc_map[k][1]] = {v[0], v[1]};
			} else {
				ans[v[0]] = loc_map[k][0];
			}
		}
		if (!cnds.empty()) {
			cout << '3' << ' ' << int(cnds.size());
			for (int i : cnds) { cout << ' ' << i+1; }
			cout << '\n' << flush;
			int resp_len = std::min(int(cnds.size()), 300);
			for (int z = 0; z < resp_len; z++) {
				int v; cin >> v; v--;
				if (to_swap[v][0] != -1) {
					std::swap(ans[to_swap[v][0]], ans[to_swap[v][1]]);
				}
			}
		}
		cout << '!';
		for (int i = 0; i < N; i++) {
			cout << ' ' << ans[i]+1;
		}
		cout << '\n' << flush;
	}

	return 0;
}
