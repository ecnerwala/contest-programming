#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> pair(2*N);
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y; x--, y--;
		pair[x] = y;
		pair[y] = x;
	}

	int ans = 2*N;
	std::vector<std::array<int, 2>> edges; edges.reserve(2*N-3);
	/*
	std::set<std::array<int, 2>> edges;
	edges.insert({0, N-1});
	for (int i = 0; i+1 < N; i++) {
		edges.insert({i, i+1});
	}
	auto insert_edge = [&](int x, int y) {
		if (x > y) std::swap(x, y);
		edges.insert({x, y});
	};
	*/

	std::vector<int> prv(2*N);
	std::vector<int> nxt(2*N);
	for (int i = 0; i < 2*N; i++) {
		prv[i] = i == 0 ? 2*N-1 : i-1;
		nxt[i] = i == 2*N-1 ? 0 : i+1;
	}

	std::vector<std::pair<int, std::set<int>>> stk; stk.reserve(N);
	for (int i = 0; i < 2*N; i++) {
		if (pair[i] < i) {
			assert(!stk.empty() && *stk.back().second.begin() == i);
			stk.back().second.erase(stk.back().second.begin());
			if (stk.back().second.empty()) {
				// pop off this whole group
				int st = stk.back().first;
				int en = i;
				stk.pop_back();

				for (int cur = nxt[st]; cur != en; cur = nxt[cur]) {
					if (cur == nxt[st]) continue;
					edges.push_back({st, cur});
				}

				ans--;

				if (nxt[en] == st) {
					// we're done, no more edges
					assert(i == 2*N-1);
					break;
				} else {
					assert(prv[st] != nxt[en]);
					if (nxt[st] != en) {
						edges.push_back({st, en});
					}
					edges.push_back({prv[st], en});
					if (prv[prv[st]] != nxt[en]) {
						edges.push_back({prv[st], nxt[en]});
					}

					// link them together
					nxt[prv[st]] = nxt[en];
					prv[nxt[en]] = prv[st];
				}
			}
		} else {
			int x = i;
			std::set<int> y; y.insert(pair[i]);
			while (!stk.empty() && pair[i] > *stk.back().second.begin()) {
				x = stk.back().first;
				if (y.size() < stk.back().second.size()) {
					std::swap(y, stk.back().second);
				}
				for (int a : stk.back().second) y.insert(a);
				stk.pop_back();
			}
			stk.emplace_back(x, std::move(y));
		}
	}

	assert(stk.empty());
	assert(int(edges.size()) == 2*N-3);

	cout << ans << '\n';
	for (auto [x, y] : edges) {
		//if (y-x == 1 || y-x == N-1) continue;
		cout << x+1 << ' ' << y+1 << '\n';
	}

	return 0;
}
