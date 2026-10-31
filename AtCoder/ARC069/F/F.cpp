#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
struct TwoSat {
	int N;
	vector<vi> gr;
	vi values; // 0 = false, 1 = true

	TwoSat(int n = 0) : N(n), gr(2*n) {}

	int add_var() { // (optional)
		gr.emplace_back();
		gr.emplace_back();
		return N++;
	}

	void either(int f, int j) {
		f = max(2*f, -1-2*f);
		j = max(2*j, -1-2*j);
		gr[f^1].push_back(j);
		gr[j^1].push_back(f);
	}
	void set_value(int x) { either(x, x); }

	void at_most_one(const vi& li) { // (optional)
		if (sz(li) <= 1) return;
		int cur = ~li[0];
		rep(i,2,sz(li)) {
			int next = add_var();
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
		trav(e, gr[i]) if (!comp[e])
			low = min(low, val[e] ?: dfs(e));
		++time;
		if (low == val[i]) do {
			x = z.back(); z.pop_back();
			comp[x] = time;
			if (values[x>>1] == -1)
				values[x>>1] = !(x&1);
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

const int MAXN = 1.1e4;
int N;
pair<int, int> pts[MAXN*2];

bool isGood(int D) {
	TwoSat ts(N * 3);
	const int Pref = N, Suff = 2 * N;
	for (int i = 0; i < N; i++) {
		ts.either(~pts[i].second, i+Pref);
		if (i > 0 && pts[i].first / D == pts[i-1].first / D) {
			ts.either(~pts[i].second, ~(i-1+Pref));
			ts.either(~(i-1+Pref), i+Pref);
		}
	}
	for (int i = N-1; i >= 0; i--) {
		ts.either(~pts[i].second, i+Suff);
		if (i+1 < N && pts[i].first / D == pts[i+1].first / D) {
			ts.either(~pts[i].second, ~(i+1+Suff));
			ts.either(~(i+1+Suff), i+Suff);
		}
	}
	for (int i = 0, j = 0; i < N; i++) {
		while (j < N && pts[j].first - pts[i].first < D) {
			j++;
		}
		if (j > i && pts[j-1].first / D > pts[i].first / D) {
			ts.either(~(i+Suff), ~(j-1+Pref));
		}
	}
	return ts.solve();
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> pts[2*i].first >> pts[2*i+1].first;
		pts[2*i].second = i;
		pts[2*i+1].second = ~i;
	}
	N *= 2;
	sort(pts, pts + N);

	int mi = 0;
	int ma = int(1e9);
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (isGood(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	cout << mi << '\n';

	return 0;
}
