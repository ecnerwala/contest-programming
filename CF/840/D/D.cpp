#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

struct data {
	pii vals[5]; // cnt, val
	data() {
		memset(vals, 0, sizeof(vals));
	}
	data(int i) {
		memset(vals, 0, sizeof(vals));
		vals[0] = pii(1, i);
	}
	void append(const pii &nxt) {
		if (!nxt.first) return;
		vals[4].second = nxt.second;
		for (int i = 0; i <= 4; i++) {
			if (vals[i].second == nxt.second) {
				vals[i].first += nxt.first;
				break;
			}
		}
		for (int i = 4; i; i--) {
			if (vals[i] > vals[i-1]) {
				swap(vals[i], vals[i-1]);
			}
		}
		for (int i = 0; i <= 4; i++) {
			vals[i].first -= vals[4].first;
			assert(vals[i].first >= vals[4].first);
		}
		assert(vals[4].first == 0);
	}
	void append(const data & d) {
		for (int i = 0; i < 4; i++) {
			append(d.vals[i]);
		}
	}
	void print() {
		for (int i = 0; i < 4; i++) {
			cerr << vals[i].first << ' ' << vals[i].second << '\n';
		}
		cerr << '\n';
	}
};

struct seg_tree {
	static const int L = 19;
	static const int S = 1 << L;
	data s[2 * S];
	void init(int* a, int n) {
		for (int i = 2 * S - 1; i; i--) {
			if (i >= S) {
				if (i-S < n) {
					s[i] = data(a[i-S]);
				} else {
					s[i] = data();
				}
			} else {
				s[i] = s[2 * i];
				s[i].append(s[2 * i + 1]);
			}
		}
	}
	// [l,r)
	data query(int l, int r) {
		data res;
		for (int a = l+S, b = r+S; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				res.append(s[a]);
				a++;
			}
			if (b & 1) {
				b--;
				res.append(s[b]);
			}
		}
		return res;
	}
} seg;

const int MAXN = 4e5;
int N;
int A[MAXN];
pair<int, int> vals[MAXN];


int get_cnt(int v, int l, int r) {
	return int(lower_bound(vals, vals + N, pii(v, r)) - lower_bound(vals, vals + N, pii(v, l)));
}

int query(int l, int r, int k) {
	data d = seg.query(l, r);
	int res = -1;
	for (int i = 0; i < 4; i++) {
		int cur = d.vals[i].second;
		if (get_cnt(cur, l, r) * k > (r - l)) {
			if (res == -1 || cur < res) res = cur;
		}
	}
	return res;
}

int main() {
	int Q;
	scanf("%d %d", &N, &Q);
	for (int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		vals[i] = pii(A[i], i);
	}
	seg.init(A, N);
	sort(vals, vals + N);

	for(int i = 0; i < Q; i++) {
		int l, r, k; scanf("%d %d %d", &l, &r, &k);
		l--; // 0-index and [l,r)
		printf("%d\n", query(l, r, k));
	}
	return 0;
}
