#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> freq(30, 60);
	freq[29] = 0;
	std::set<std::vector<int>> locs;
	locs.insert({});
	for (int i = 0; i < 30; i++) {
		locs.insert({i});
		freq[i]--;
		for (int j = i+1; j < 30; j++) {
			locs.insert({i, j});
			freq[i]--;
			freq[j]--;
		}
	}
	std::mt19937 mt(50);
	auto did_find = [&](this auto&& self) -> bool {
		//cerr << "dfs " << int(locs.size()) << '\n';
		if (int(locs.size()) == 845) return true;

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
	std::map<std::vector<int>, int> loc_map;
	for (int i = 0; i < int(loc_list.size()); i++) {
		loc_map.insert({loc_list[i], i});
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> results(N);
		for (int q = 0; q < 30; q++) {
			std::vector<int> q_inds; q_inds.reserve(q == 0 ? 300 : 60);
			for (int i = 0; i < N; i++) {
				if (std::find(loc_list[i].begin(), loc_list[i].end(), q) != loc_list[i].end()) {
					q_inds.push_back(i);
				}
			}
			if (q_inds.empty()) {
				continue;
			}
			cout << (q == 0 ? 4 : 2) << ' ' << int(q_inds.size());
			for (auto i : q_inds) {
				cout << ' ' << i+1;
			}
			cout << '\n' << flush;
			assert(int(q_inds.size()) <= (q == 0 ? 300 : 60));
			for (int z = 0; z < int(q_inds.size()); z++) {
				int v; cin >> v; v--;
				results[v].push_back(q);
			}
		}
		cout << '!';
		for (int i = 0; i < N; i++) {
			auto it = loc_map.find(results[i]);
			assert(it != loc_map.end());
			cout << ' ' << it->second+1;
		}
		cout << '\n' << flush;
	}

	return 0;
}
