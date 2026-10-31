#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> freq(30, 60);
	freq[0] = 300;
	std::set<std::vector<int>> locs;
	for (int i = 0; i < 30; i++) {
		locs.insert({i});
		freq[i]--;
		for (int j = i+1; j < 30; j++) {
			locs.insert({i, j});
			freq[i]--;
			freq[j]--;
		}
	}
	std::mt19937 mt(48);
	auto did_find = [&](this auto&& self) -> bool {
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

			if (self()) return true;

			locs.erase(cnd);
		}

		return false;
	}();
	assert(did_find);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
	}

	return 0;
}
