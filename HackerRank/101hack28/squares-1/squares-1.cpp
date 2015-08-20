#include<bits/stdc++.h>
using namespace std;

const int INF = 2e9 + 10;

vector<int> coords;

inline int lookup(int a) {
	return int(lower_bound(coords.begin(), coords.end(), a) - coords.begin());
}

typedef pair<int, int> pii;
#define A first
#define B second

const int S = 1 << 19;
int seg[2 * S];

//0-indexed, [a,b)
void update(int a, int b, int v) {
	assert(a < S && b < S);
	for(a |= S, b |= S; a < b; a >>= 1, b >>= 1) {
		if(a % 2 == 1) {
			seg[a] += v;
			a++;
		}
		if(b % 2 == 1) {
			b--;
			seg[b] += v;
		}
	}
}

int query(int a) {
	int res = 0;
	for(a |= S; a; a >>= 1) {
		res += seg[a];
	}
	return res;
}

const int MAXN = 2e5, MAXM = 2e5;

int N, M;
pii P[MAXN];
pii Q[MAXM];

int L;

vector<pair<int, pii>> evts;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> L;
	L++;
	for(int i = 0; i < N; i++) cin >> P[i].A >> P[i].B;
	for(int i = 0; i < M; i++) cin >> Q[i].A >> Q[i].B;
	for(int i = 0; i < N; i++) {
		coords.push_back(P[i].B);
		coords.push_back(P[i].B + L);
	}
	for(int i = 0; i < M; i++) {
		coords.push_back(Q[i].B);
	}
	assert(coords.size() < S);
	sort(coords.begin(), coords.end());
	coords.resize(unique(coords.begin(), coords.end()) - coords.begin());
	for(int i = 0; i < N; i++) {
		int l = lookup(P[i].B), r = lookup(P[i].B + L);
		assert(l < r);
		evts.emplace_back(P[i].A, pii(l, r));
		evts.emplace_back(P[i].A + L, pii(-l, -r));
	}
	for(int i = 0; i < M; i++) {
		int v = lookup(Q[i].B);
		evts.emplace_back(Q[i].A, pii(INF, v));
	}
	sort(evts.begin(), evts.end());

	int res = 0;
	for(auto it : evts) {
		pii p = it.B;
		if(p.A == INF) {
			// query
			res = max(res, query(p.B));
		} else {
			if(p.A < p.B) {
				// insert
				update(p.A, p.B, 1);
			} else {
				update(-p.A, -p.B, -1);
			}
		}
	}
	cout << res << '\n';
	return 0;
}
