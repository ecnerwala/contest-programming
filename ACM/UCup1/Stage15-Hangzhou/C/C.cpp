#include <bits/stdc++.h>

[[noreturn]] void FAIL() {
	std::cout << "NO" << '\n';
	exit(0);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> depth(N);
	depth[0] = 0;
	std::vector<int> P(N);
	P[0] = -1;

	std::vector<std::vector<std::pair<int, int>>> chs_eq(N);
	std::vector<std::vector<std::pair<int, int>>> chs_long(N);
	std::vector<std::vector<std::pair<int, int>>> chs_short(N);

	for (int i = 1; i < N; i++) {
		int v, p; string s; cin >> v >> p >> s;
		v--, p--;
		assert(v == i);
		P[i] = p;
		depth[i] = depth[p] + 1;
		if (s == "-") {
		} else if (s == "Duan") {
			chs_short[i].push_back({depth[i], i});
		} else if (s == "Tong") {
			chs_eq[i].push_back({depth[i], i});
		} else if (s == "Chang") {
			chs_long[i].push_back({depth[i], i});
		} else assert(false);
	}

	std::vector<std::array<int, 2>> pairs; pairs.reserve(N);

	std::vector<std::pair<int, int>> buf; buf.reserve(N);

	for (int v = N-1; v >= 0; v--) {
		int p = P[v];
		auto& ch_eq = chs_eq[v];
		auto& ch_long = chs_long[v];
		auto& ch_short = chs_short[v];

		bool has_extra = (v == 0) ? true : false;

		std::sort(ch_eq.begin(), ch_eq.end());
		for (int i = 0; i < int(ch_eq.size()); ) {
			if (i+1 < int(ch_eq.size()) && ch_eq[i+1].first == ch_eq[i].first) {
				pairs.push_back({ch_eq[i].second, ch_eq[i+1].second});
				i += 2;
			} else {
				if (has_extra) FAIL();
				has_extra = true;
				chs_eq[p].push_back(ch_eq[i]);
				i++;
			}
		}

		std::sort(ch_long.begin(), ch_long.end());
		std::sort(ch_short.begin(), ch_short.end());

		buf.clear();
		for (int i = 0, j = 0; i < int(ch_short.size()) || j < int(ch_long.size()); ) {
			if (i < int(ch_short.size()) && (j == int(ch_long.size()) || ch_short[i].first < ch_long[j].first)) {
				buf.push_back(ch_short[i]);
				i++;
			} else {
				if (buf.empty()) {
					if (has_extra) FAIL();
					has_extra = true;
					chs_long[p].push_back(ch_long[j]);
				} else {
					assert(buf.back().first < ch_long[j].first);
					pairs.push_back({buf.back().second, ch_long[j].second});
					buf.pop_back();
				}
				j++;
			}
		}
		while (!buf.empty()) {
			if (has_extra) FAIL();
			has_extra = true;
			chs_short[p].push_back(buf.back());
			buf.pop_back();
		}
	}

	cout << "YES" << '\n';
	for (auto [u, v] : pairs) {
		cout << u+1 << ' ' << v+1 << '\n';
	}

	return 0;
}
