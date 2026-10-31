#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	auto query_lt = [&](int i, int j) -> bool {
		cout << "? " << i << ' ' << j << '\n' << flush;
		string s; cin >> s;
		return s == "<";
	};
	// 1-indexed sum to K+1 or sum to K
	std::vector<std::pair<int, int>> cnds; cnds.reserve(2*(N+2));
	for (int i = 0; i <= N+1; i++) {
		cnds.push_back({i, K+1-i});
		cnds.push_back({i, K-i});
	}
	int mi = 0, ma = int(cnds.size()) - 1;
	auto is_valid = [&](int a) -> bool {
		return 1 <= cnds[a].first && cnds[a].first <= N && 1 <= cnds[a].second && cnds[a].second <= N;
	};
	while (!is_valid(mi)) mi++;
	mi--;
	while (!is_valid(ma)) ma--;
	ma++;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (query_lt(cnds[md].first, cnds[md].second)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	assert(ma - mi == 1);
	if (cnds[mi].first < cnds[ma].first) {
		cout << '!' << ' ' << 2 << ' ' << cnds[ma].second << '\n' << flush;
	} else {
		cout << '!' << ' ' << 1 << ' ' << cnds[ma].first << '\n' << flush;
	}

	return 0;
}
