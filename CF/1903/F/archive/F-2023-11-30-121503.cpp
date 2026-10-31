#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

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

	int atMostOne(int a, int b) {
		int next = addVar();
		either(~a, ~b);
		// next >= a iff next == true or a == false
		either(~a, next);
		either(~b, next);
		return next;
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

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) {
			cin >> u >> v; u--, v--;
		}

		kactl::TwoSat base_prob(N);
		for (auto [u, v] : edges) {
			base_prob.either(u, v);
		}

		auto is_good = [&](int k) -> bool {
			assert(k >= 2);
			// no 2 within k
			kactl::TwoSat cur = base_prob;
			std::vector<int> pref(N);
			std::vector<int> suff(N);
			std::iota(pref.begin(), pref.end(), 0);
			std::iota(suff.begin(), suff.end(), 0);
			for (int i = 0; i < N; i += k-1) {
				int j = std::min(N, (i + k-1)) - 1;
				for (int a = i+1; a <= j; a++) {
					pref[a] = cur.atMostOne(pref[a], pref[a-1]);
				}
				for (int a = j-1; a >= i; a--) {
					suff[a] = cur.atMostOne(suff[a], suff[a+1]);
				}
			}
			for (int i = 0; i + k-1 < N; i++) {
				cur.either(~pref[i], ~suff[i+(k-1)]);
			}
			return cur.solve();
		};

		int mi = 1;
		int ma = N+1;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			assert(md >= 2);
			if (is_good(md)) mi = md;
			else ma = md;
		}
		cout << mi << '\n';
	}

	return 0;
}

// binary search on answer
