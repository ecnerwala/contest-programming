#include <bits/stdc++.h>

namespace kactl {

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

/**
 * Author: Emil Lenngren, Simon Lindholm
 * Date: 2011-11-29
 * License: CC0
 * Source: folklore
 * Description: Calculates a valid assignment to boolean variables a, b, c,... to a 2-SAT problem,
 * so that an expression of the type $(a||b)\&\&(!a||c)\&\&(d||!b)\&\&...$
 * becomes true, or reports that it is unsatisfiable.
 * Negated variables are represented by bit-inversions (\texttt{\tilde{}x}).
 * Usage:
 *  TwoSat ts(number of boolean variables);
 *  ts.either(0, \tilde3); // Var 0 is true or var 3 is false
 *  ts.setValue(2); // Var 2 is true
 *  ts.atMostOne({0,\tilde1,2}); // <= 1 of vars 0, \tilde1 and 2 are true
 *  ts.solve(); // Returns true iff it is solvable
 *  ts.values[0..N-1] holds the assigned values to the vars
 * Time: O(N+E), where N is the number of boolean variables, and E is the number of clauses.
 * Status: stress-tested
 */

struct TwoSat {
	int N;
	vector<vi> gr;
	vi values; // 0 = false, 1 = true

	TwoSat(int n = 0) : N(n), gr(2*n) {}

	int addVar() { // (optional)
		gr.emplace_back();
		gr.emplace_back();
		return N++;
	}

	void either(int f, int j) {
		f = max(2*f, -1-2*f);
		j = max(2*j, -1-2*j);
		gr[f].push_back(j^1);
		gr[j].push_back(f^1);
	}
	void setValue(int x) { either(x, x); }

	void atMostOne(const vi& li) { // (optional)
		if (sz(li) <= 1) return;
		int cur = ~li[0];
		rep(i,2,sz(li)) {
			int next = addVar();
			either(cur, ~li[i]);
			either(cur, next);
			either(~li[i], next);
			cur = ~next;
		}
		either(cur, ~li[1]);
	}

	vi val, comp, z; int time = 0;
	int dfs(int i) {
		int low = val[i] = ++time, x; z.push_back(i);
		for(int e : gr[i]) if (!comp[e])
			low = min(low, val[e] ?: dfs(e));
		if (low == val[i]) do {
			x = z.back(); z.pop_back();
			comp[x] = low;
			if (values[x>>1] == -1)
				values[x>>1] = x&1;
		} while (x != i);
		return val[i] = low;
	}

	bool solve() {
		values.assign(N, -1);
		val.assign(2*N, 0); comp = val;
		rep(i,0,2*N) if (!comp[i]) dfs(i);
		rep(i,0,N) if (comp[2*i] == comp[2*i+1]) return 0;
		return 1;
	}
};

}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	std::vector<std::vector<int>> filled(N, std::vector<int>(M, K));
	for (int i = 0; i < K; i++) {
		int x, y; cin >> x >> y; x--, y--;
		assert(filled[x][y] == K);
		filled[x][y] = ~i;
	}
	std::vector<bool> must(K);
	for (int i = 0; i < K; i++) {
		int l; cin >> l;
		if (l == 0) must[i] = true;
		for (int z = 0; z < l; z++) {
			int x, y; cin >> x >> y; x--, y--;
			assert(filled[x][y] == K);
			filled[x][y] = i;
		}
	}
	int mi = -1;
	int ma = M+1;
	std::vector<int> ma_val(K, false);
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		kactl::TwoSat ts(K + N * (M+1));
		for (int k = 0; k < K; k++) if (must[k]) ts.either(~k, ~k);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int cur = K + i * (M+1) + j;
				// If cur, then cur+1
				ts.either(~cur, cur+1);
				// If filled[i][j], then cur+1
				if (filled[i][j] != K) ts.either(~filled[i][j], cur+1);
				if (j+md < M && filled[i][j+md] != K) {
					// if cur+1, then
					ts.either(~(cur+1), ~filled[i][j+md]);
				}
			}
		}
		if (ts.solve()) {
			ma = md;
			for (int i = 0; i < K; i++) ma_val[i] = ts.values[i];
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';
	std::vector<int> ans; ans.reserve(K);
	for (int i = 0; i < K; i++) if (ma_val[i]) ans.push_back(i);
	cout << ans.size();
	for (int v : ans) cout << ' ' << v+1;
	cout << '\n';

	return 0;
}
