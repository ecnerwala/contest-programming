#include<bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	if (fopen("hats.in", "r")) {
		freopen("hats.in", "r", stdin);
		freopen("hats.out", "w", stdout);
	}

	int N, M; cin >> N >> M;
	vector<int> K(M);
	for (int i = 0; i < M; i++) cin >> K[i];
	sort(K.begin(), K.end());

	map<int, int> canonIds;
	map<int, int> viewIds;
	vector<int> views;

	vector<vector<pair<int, int>>> edges;

	for (int m = 0; m < (1 << (2 * N)); m++) {
		vector<int> cnt(4);
		for (int i = 0; i < N; i++) {
			int v = (m >> (2 * i)) & 3;
			cnt[v]++;
		}
		while (int(cnt.size()) > M) {
			cnt.pop_back();
		}
		sort(cnt.begin(), cnt.end());
		if (cnt != K) continue;

		int canon = m;
		for (int i = 1; i < N; i++) {
			canon = min(canon, (m >> (2 * i)) | ((m & ((1 << (2 * i)) - 1)) << (2 * (N-i))));
		}
		if (!canonIds.count(canon)) {
			canonIds[canon] = int(canonIds.size());
			edges.emplace_back();
		}
		canon = canonIds[canon];
		int view = m >> 2;
		if (!viewIds.count(view)) {
			viewIds[view] = int(viewIds.size());
			views.push_back(view);
		}
		view = viewIds[view];
		int own = m & 3;

		edges[canon].emplace_back(view, own);
	}

	int L = int(canonIds.size());
	int R = int(viewIds.size());

	vector<pair<int, int>> curMatch(R, {-1, 0});
	vector<char> vis(L);

	int ans = 0;
	while (ans < N) {
		for (int i = 0; i < L; i++) {
			if (ans % (N / int(edges[i].size())) != 0) {
				// we don't need to increment
				continue;
			}
			fill(vis.begin(), vis.end(), false);
			if (
				!y_combinator([&](auto self, int cur) -> bool {
					if (cur == -1) return true;
					if (vis[cur]) return false;
					vis[cur] = true;
					for (auto it : edges[cur]) {
						int view = it.first, label = it.second;
						int nxt = curMatch[view].first;
						if (self(nxt)) {
							curMatch[view] = {cur, label};
							return true;
						}
					}
					return false;
				})(i)
			) {
				goto fail;
			}
		}
		ans++;
	}

fail:
	cout << ans << '\n';
	for (int r = 0; r < R; r++) {
		for (int i = 0; i < N-1; i++) {
			cout << ((views[r] >> (2 * i)) & 3) + 1;
		}
		cout << ' ' << curMatch[r].second+1 << '\n';
	}

	return 0;
}
