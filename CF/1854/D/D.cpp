#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	auto query = [&](int u, int k, const std::vector<int>& s) -> bool {
		assert(!s.empty());
		cout << '?' << ' ' << u+1 << ' ' << k << ' ' << int(s.size());
		for (int v : s) cout << ' ' << v+1;
		cout << '\n' << flush;
		int res; cin >> res;
		if (res == 0) {
			return false;
		} else if (res == 1) {
			return true;
		} else assert(false);
	};

	std::vector<int> buf; buf.reserve(N);
	auto get_succ = [&](int u, int k) -> int {
		int mi = 0, ma = N;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			// query [mi, md)
			buf.resize(md - mi);
			std::iota(buf.begin(), buf.end(), mi);
			if (query(u, k, buf)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return mi;
	};

	std::vector<bool> in_cycle(N);
	int cur = get_succ(0, N);
	for (int z = 0; !in_cycle[cur]; z++) {
		in_cycle[cur] = true;
		if (z <= 2) cur = get_succ(cur, 1);
		else cur = get_succ(cur, 3);
	}

	std::vector<int> goods; goods.reserve(N);
	std::vector<int> bads; bads.reserve(N);
	for (int i = 0; i < N; i++) {
		if (in_cycle[i]) goods.push_back(i);
		else bads.push_back(i);
	}

	std::mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	std::shuffle(bads.begin(), bads.end(), rng);
	for (int z = 0; z < 2; z++) {
		for (int v : bads) {
			if (in_cycle[v]) continue;
			if (query(v, 1, goods)) {
				in_cycle[v] = true;
				goods.push_back(v);
			}
		}
	}

	for (int v : bads) {
		if (in_cycle[v]) continue;
		if (query(v, N, goods)) {
			in_cycle[v] = true;
			goods.push_back(v);
		}
	}
	cout << '!' << ' ' << goods.size();
	for (int v : goods) cout << ' ' << v+1;
	cout << '\n' << flush;

	return 0;
}
