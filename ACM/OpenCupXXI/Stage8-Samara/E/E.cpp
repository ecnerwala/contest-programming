#include <bits/stdc++.h>

[[noreturn]] void INFTY() {
	std::cout << "Infinity" << '\n';
	exit(0);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<pair<int64_t, pair<int64_t, int64_t>>> evts; evts.reserve(N);
	for (int i = 0; i < N; i++) {
		int64_t a, b, c; cin >> a >> b >> c;
		evts.push_back({a, {b,c}});
	}
	sort(evts.begin(), evts.end());
	int64_t cur_z = 0;

	set<pair<int64_t, int64_t>> st;
	map<int64_t, int> val_cnt;

	int64_t ans = 0;
	for (auto [z, p] : evts) {
		if (z-1 >= cur_z) {
			cur_z = z;
			if (st.empty()) INFTY();
			if (st.begin()->first != 0) {
				INFTY();
			}
			if (st.rbegin()->second != 0) {
				INFTY();
			}
			if (!val_cnt.empty()) {
				ans = max(ans, cur_z-1 + val_cnt.rbegin()->first);
			}
		}
		// insert p
		auto [x, y] = p;
		auto it = st.lower_bound({x, y});
		if (it != st.begin() && prev(it)->second <= y) {
			// it's not worth inserting
			continue;
		}

		// break at this point
		if (it != st.begin() && it != st.end()) {
			val_cnt[prev(it)->second - 1 + it->first - 1]--;
		}

		while (it != st.end() && y <= it->second) {
			// erase it
			int64_t old_y = it->second;
			it = st.erase(it);
			if (it != st.end()) val_cnt[old_y - 1 + it->first - 1]--;
		}

		if (it != st.end()) {
			val_cnt[y-1 + it->first - 1] ++;
		}
		if (it != st.begin()) {
			val_cnt[prev(it)->second - 1 + x-1] ++;
		}
		st.insert({x, y});

		while (!val_cnt.empty() && val_cnt.rbegin()->second == 0) {
			val_cnt.erase(prev(val_cnt.end()));
		}

		//for (auto it : st) cerr << it.first << ' ' << it.second << '\n';
		//for (auto it : val_cnt) cerr << it.first << 'x' << it.second << '\n';
		//cerr << '\n';
	}

	if (st.empty()) INFTY();
	if (st.begin()->first != 0) {
		INFTY();
	}
	if (st.rbegin()->second != 0) {
		INFTY();
	}
	if (!val_cnt.empty()) {
		INFTY();
	}

	cout << ans << '\n';

	return 0;
}
