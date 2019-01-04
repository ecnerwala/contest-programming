#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXT = 1.1e6;
int T;
int F[MAXT];

const int S = 1 << 20;
struct data {
	ll sum;
	ll best;
} seg[S*2];
void update(int i) {
	assert(i >= 1);
	assert(i < S);
	data& l = seg[2*i];
	data& r = seg[2*i+1];
	seg[i].sum = l.sum + r.sum;
	seg[i].best = max(l.best, l.sum + r.best);
}

void setval(int a, ll v) {
	a += S;
	seg[a].sum = seg[a].best = v;
	for (a /= 2; a; a /= 2) update(a);
}
ll getbest() {
	return seg[1].best;
}

const int MAXM = MAXT;
int M;
int W[MAXM];
vector<int> days[MAXM];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> T >> M;
	for (int i = 0; i < T; i++) {
		cin >> F[i]; F[i] --;
		days[F[i]].push_back(i);
	}
	for (int m = 0; m < M; m++) {
		cin >> W[m];
	}
	for (int m = 0; m < M; m++) {
		reverse(days[m].begin(), days[m].end());
		int sz = int(days[m].size());
		if (sz >= 1) {
			setval(days[m][sz-1], W[m]);
		}
		if (sz >= 2) {
			setval(days[m][sz-2], -W[m]);
		}
	}
	ll res = 0;
	for (int i = 0; i < T; i++) {
		res = max(res, getbest());

		int m = F[i];
		assert(days[m].back() == i);
		// remove my current one
		setval(i, 0);
		days[m].pop_back();

		// update the current day
		int sz = int(days[m].size());
		if (sz >= 1) {
			setval(days[m][sz-1], W[m]);
		}
		if (sz >= 2) {
			setval(days[m][sz-2], -W[m]);
		}
	}
	cout << res << '\n';
	return 0;
}
