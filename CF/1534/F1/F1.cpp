#include <bits/stdc++.h>

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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int H, N; cin >> H >> N;
	vector<vector<int8_t>> G(N, vector<int8_t>(H));
	for (int i = 0; i < H; i++) {
		string r; cin >> r;
		for (int j = 0; j < N; j++) {
			G[j][H-1-i] = (r[j] == '#');
		}
	}
	vector<int> A(N);
	for (auto& a : A) cin >> a;

	const int V = N * H;
	vector<vector<int>> adj(V);
	auto vert = [&](int i, int h) -> int { assert(0 <= i && i < N && 0 <= h && h < H); return i * H + h; };
	vector<bool> alive(V);
	for (int i = 0; i < N; i++) {
		int prvleft = -1;
		int prv = -1;
		int prvright = -1;
		for (int j = 0; j < H; j++) {
			if (i-1 >= 0 && G[i-1][j]) prvleft = j;
			if (i+1 < N && G[i+1][j]) prvright = j;
			if (G[i][j]) {
				alive[vert(i, j)] = true;
				if (prv != -1) adj[vert(i, j)].push_back(vert(i, prv));
				if (prvleft != -1) adj[vert(i, j)].push_back(vert(i-1, prvleft));
				if (prvright != -1) adj[vert(i, j)].push_back(vert(i+1, prvright));
				if (j+1 < H && G[i][j+1]) {
					adj[vert(i, j)].push_back(vert(i, j+1));
				}
				prv = j;
			}
		}
	}
	vector<int> scc_num(V, -1);
	{
		int nxt_idx = 0;
		vector<int> index(V, -1);
		vector<int> lowval(V, -1);
		vector<int> stk; stk.reserve(V);
		vector<bool> in_stk(V, false);
		auto dfs = std::y_combinator([&](auto self, int cur) -> void {
			assert(alive[cur]);
			assert(index[cur] == -1);
			index[cur] = nxt_idx++;
			lowval[cur] = index[cur];
			stk.push_back(cur);
			in_stk[cur] = true;
			for (int nxt : adj[cur]) {
				if (index[nxt] == -1) {
					self(nxt);
					lowval[cur] = min(lowval[cur], lowval[nxt]);
				} else if (in_stk[nxt]) {
					lowval[cur] = min(lowval[cur], index[nxt]);
				}
			}
			if (lowval[cur] == index[cur]) {
				while (in_stk[cur]) {
					int v = stk.back(); stk.pop_back();
					in_stk[v] = false;
					scc_num[v] = cur;
				}
				assert(scc_num[cur] == cur);
			}
		});
		for (int v = 0; v < V; v++) {
			if (alive[v]) {
				if (index[v] == -1) {
					dfs(v);
					assert(stk.empty());
				}
			}
		}
	}
	vector<int> inDeg(V);
	for (int u = 0; u < V; u++) {
		if (!alive[u]) continue;
		for (int v : adj[u]) {
			if (scc_num[v] != scc_num[u]) inDeg[scc_num[v]]++;
		}
	}

	int ans = 0;
	for (int i = 0; i < V; i++) {
		assert((scc_num[i] != -1) == alive[i]);
		if (scc_num[i] == i) {
			ans += inDeg[i] == 0;
		}
	}
	cout << ans << '\n';

	/*
	vector<int> heights(N);
	for (int i = 0; i < N; i++) {
		int h = -1;
		int a = A[i];
		while (a > 0) {
			h++;
			if (G[i][h]) a--;
		}
		assert(h == -1 || G[i][h]);
		heights[i] = h;
	}

	int ans = 0;
	for (int h = H-1; h >= 0; h--) {
		for (int i = 0; i < N; i++) {
			if (heights[i] == h) {
				ans++;
				{
					int cur_height = h;
					for (int j = i+1; j < N; j++) {
						if (heights[j] == -1 || heights[j] > cur_height) break;
						cur_height = heights[j];
						heights[j] = -1;
					}
				}
				{
					int cur_height = h;
					for (int j = i-1; j >= 0; j--) {
						if (heights[j] == -1 || heights[j] > cur_height) break;
						cur_height = heights[j];
						heights[j] = -1;
					}
				}
				heights[i] = -1;
			}
		}
	}
	cout << ans << '\n';
	*/

	return 0;
}
