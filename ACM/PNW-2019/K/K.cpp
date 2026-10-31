#include<bits/stdc++.h>
using namespace std;

struct op {
	int i = 0; int st = 0; int t = 0;
	friend bool operator == (const op& a, const op& b) {
		return tie(a.i, a.st, a.t) == tie(b.i, b.st, b.t);
	}
};

op merge(op a, op b) {
	if (b == op()) return a;
	else return b;
}

const int S = 1 << 19;
op seg[2*S];

void downdate(int i) {
	if (i < S) {
		seg[2*i] = merge(seg[2*i], seg[i]);
		seg[2*i+1] = merge(seg[2*i+1], seg[i]);
		seg[i] = op();
	}
}

void downdate_all(int i) {
	if (!i) return;
	downdate_all(i/2);
	downdate(i);
}

void rangeUpdate(int l, int r, op o) {
	assert(l < r);
	downdate_all(l + S);
	downdate_all(r + S - 1);

	for (int a = l + S, b = r + S; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			seg[a] = merge(seg[a], o);
			a++;
		}
		if (b & 1) {
			--b;
			seg[b] = merge(seg[b], o);
		}
	}
}

op query(int l) {
	op res;
	for (int a = l+S; a; a /= 2) {
		res = merge(res, seg[a]);
	}
	return res;
}

const int MAXN = 5.1e5;
const int MAXM = 5.1e5;
const int MAXQ = 5.1e5;

int N, M, Q;
vector<int> X[MAXM];

vector<pair<int, pair<int, int>>> evts[MAXM];
int ans[MAXQ];

int bit[MAXM];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> Q;
	for (int i = 1; i <= M; i++) {
		int s; cin >> s;
		X[i].resize(s);
		for (int j = 0; j < s; j++) {
			cin >> X[i][j];
		}
	}

	memset(ans, -1, sizeof(ans));

	for (int q = 0; q < Q; q++) {
		int t; cin >> t;
		if (t == 1) {
			int i, p; cin >> i >> p;
			rangeUpdate(p, p + int(X[i].size()), op{i, p, q});
		} else if (t == 2) {
			int p; cin >> p;
			auto o = query(p);
			if (o.i == 0) {
				ans[q] = 0;
			} else {
				evts[o.i].push_back({o.t, {p - o.st, -1-q}});
			}
		} else if (t == 3) {
			int i, l, r; cin >> i >> l >> r;
			l--;
			evts[i].push_back({q, {l, r}});
		} else assert(false);
	}

	for (int i = 1; i <= M; i++) {
		for (int a = 1; a <= int(X[i].size()); a++) {
			bit[a] = 0;
		}
		sort(evts[i].begin(), evts[i].end());
		for (auto it : evts[i]) {
			if (it.second.second < 0) {
				// query
				int p = it.second.first;
				int q = -1-it.second.second;

				ans[q] = X[i][p];
				for (int a = p+1; a > 0; a -= a & (-a)) {
					ans[q] += bit[a];
				}
				ans[q] &= 255;
			} else {
				int l = it.second.first;
				int r = it.second.second;
				for (int a = l+1; a <= int(X[i].size()); a += a & (-a)) {
					bit[a]++;
				}
				for (int a = r+1; a <= int(X[i].size()); a += a & (-a)) {
					bit[a]--;
				}
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		if (ans[q] == -1) continue;
		cout << ans[q] << '\n';
	}

	return 0;
}
