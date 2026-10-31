#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<array<int, 2>> intervals(N);
	for (auto& it : intervals) {
		cin >> it[0] >> it[1];
	}

	vector<pair<int, int>> lefts(N);
	vector<pair<int, int>> rights(N);
	for (int i = 0; i < N; i++) {
		lefts[i] = {intervals[i][0], i};
		rights[i] = {intervals[i][1], i};
	}
	sort(lefts.begin(), lefts.end());
	sort(rights.begin(), rights.end());

	vector<int> prv(N, -1);
	vector<int> nxt(N, -1);
	int Nchains = 0;
	{ // Phase 1: build up chains of projects
		set<pair<int, int>> alive;
		for (auto [r, i] : rights) {
			auto it = alive.lower_bound({intervals[i][0], -1});
			if (it != alive.begin()) {
				--it;
				assert(it->first < intervals[i][0]);
				nxt[it->second] = i;
				prv[i] = it->second;
				alive.erase(it);
			}
			alive.insert({r, i});
		}
		Nchains = int(alive.size());
	}

	// Identify the middle point
	int X = -1;
	for (int a = 0, b = 0; a < N; a++) {
		int cnd = lefts[a].first;
		while (b < N && rights[b].first < cnd) b++;
		if (a + 1 - b == Nchains) {
			X = cnd;
			break;
		}
	}
	assert(X != -1);

	// Compute a max matching of the middle ones
	vector<int> mid_matching(N, -1);
	{
		set<pair<int, int>> right_side;
		set<pair<int, int>> left_side;
		for (int i = 0; i < N; i++) {
			auto [l, r] = intervals[i];
			if (r < X) left_side.insert({r, i});
			else if (l > X) right_side.insert({l, i});
		}
		for (auto [l, i] : lefts) {
			if (l > X) break;
			int r = intervals[i][1];
			if (r < X) continue;

			auto rit = right_side.lower_bound({r+1, -1});
			if (rit != right_side.end()) {
				mid_matching[i] = rit->second;
				right_side.erase(rit);
				continue;
			}

			if (!left_side.empty() && left_side.begin()->first < l) {
				auto lit = left_side.begin();
				mid_matching[i] = lit->second;
				left_side.erase(lit);
				continue;
			}

			// this guy is bad
		}
	}

	// Now fix things up
	vector<int> q; q.reserve(Nchains);
	for (int i = 0; i < N; i++) {
		if (prv[i] == -1 && nxt[i] == -1) {
			assert(intervals[i][0] <= X && X <= intervals[i][1]);
			q.push_back(i);
		}
	}
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		assert(prv[cur] == -1 && nxt[cur] == -1);
		if (mid_matching[cur] == -1) continue;
		int o = mid_matching[cur];
		if (prv[o] != -1) {
			nxt[prv[o]] = nxt[o];
			if (prv[prv[o]] == -1 && nxt[prv[o]] == -1) {
				q.push_back(prv[o]);
			}
		}
		if (nxt[o] != -1) {
			prv[nxt[o]] = prv[o];
			if (prv[nxt[o]] == -1 && nxt[nxt[o]] == -1) {
				q.push_back(nxt[o]);
			}
		}
		prv[o] = nxt[o] = -1;
		if (intervals[o][1] < intervals[cur][0]) {
			nxt[o] = cur;
			prv[cur] = o;
		} else if (intervals[cur][1] < intervals[o][0]) {
			nxt[cur] = o;
			prv[o] = cur;
		} else assert(false);
	}

	int max_matching = N;
	for (int i = 0; i < N; i++) {
		if (prv[i] == -1 && nxt[i] == -1) {
			assert(mid_matching[i] == -1);
			max_matching --;
		}
	}

	vector<pair<int, int>> matches; matches.reserve(max_matching / 2);
	std::optional<std::array<int, 3>> extra_chain;
	for (auto [_, i] : lefts) {
		assert(prv[i] == -1);
		if (nxt[i] == -1) continue;
		else if (nxt[nxt[i]] == -1) {
			matches.push_back({i, nxt[i]});
			prv[nxt[i]] = -1;
			nxt[i] = -1;
		} else if (nxt[nxt[nxt[i]]] == -1) {
			// length 3
			if (extra_chain) {
				array<int, 3> a = {i, nxt[i], nxt[nxt[i]]};
				array<int, 3> b = *extra_chain;
				if (intervals[a[0]][1] < intervals[b[2]][0]) {
					matches.push_back({a[0], b[2]});
					matches.push_back({a[1], a[2]});
					matches.push_back({b[0], b[1]});
				} else if (intervals[b[0]][1] < intervals[a[2]][0]) {
					matches.push_back({b[0], a[2]});
					matches.push_back({b[1], b[2]});
					matches.push_back({a[0], a[1]});
				} else assert(false);
				extra_chain = std::nullopt;
			} else {
				extra_chain = {i, nxt[i], nxt[nxt[i]]};
			}
			prv[nxt[nxt[i]]] = -1;
			nxt[nxt[i]] = -1;
			prv[nxt[i]] = -1;
			nxt[i] = -1;
		} else {
			// length >= 4, just cut it off
			matches.push_back({i, nxt[i]});
			prv[nxt[nxt[i]]] = -1;
			nxt[nxt[i]] = -1;
			prv[nxt[i]] = -1;
			nxt[i] = -1;
		}
	}
	if (extra_chain) {
		matches.push_back({(*extra_chain)[0], (*extra_chain)[1]});
	}
	assert(int(matches.size()) == max_matching / 2);

	vector<int> ans(N, -1);
	for (auto [i, j] : matches) {
		ans[i] = ans[j] = --M;
		if (M == 0) break;
	}
	for (int i = 0; M > 0 && i < N; i++) {
		if (ans[i] == -1) ans[i] = --M;
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
