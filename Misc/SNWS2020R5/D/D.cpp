#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

const int MAXN = 1.1e5;
int N;
ll C[MAXN];

const int S = 1 << 17;
struct {
	ll cntMin = 0;
	ll minVal = 0;
	ll minVal2 = 0;
	ll sum = 0;

	ll oldMinVal = 0;
	ll lazyVal = 0;
} seg[2*S];

int node_sz(int i) {
	return 1 << (__builtin_clz(i) - __builtin_clz(S));
}

void update(int i) {
	// must be propogated already
	assert(seg[i].lazyVal == 0);
	assert(seg[i].minVal == seg[i].oldMinVal);
	if (i >= S) {
		seg[i].cntMin = 1;
		seg[i].minVal2 = INF;
		seg[i].sum = seg[i].minVal;
	} else {
		auto& s = seg[i];
		auto& l = seg[2*i];
		auto& r = seg[2*i+1];

		s.minVal = min(l.minVal, r.minVal);
		s.minVal2 = min((s.minVal == l.minVal ? l.minVal2 : l.minVal), (s.minVal == r.minVal ? r.minVal2 : r.minVal));
		assert(s.minVal2 > s.minVal && s.minVal >= 0);
		s.cntMin = (s.minVal == l.minVal ? l.cntMin : 0) + (s.minVal == r.minVal ? r.cntMin : 0);
		s.sum = l.sum + r.sum;

		s.oldMinVal = s.minVal;
		s.lazyVal = 0;
	}
}

void propogate(int i) {
	if (i >= S) {
		// do nothing
	} else {
		for (int z = 0; z < 2; z++) {
			int c = 2 * i + z;
			// modify the minVal first
			if (seg[c].minVal == seg[i].oldMinVal) {
				seg[c].sum -= seg[c].cntMin * seg[c].minVal;
				seg[c].minVal = seg[i].minVal;
				seg[c].sum += seg[c].cntMin * seg[c].minVal;
			} else {
				seg[c].minVal += seg[i].lazyVal;
				seg[c].sum += seg[i].lazyVal * seg[c].cntMin;
			}

			seg[c].minVal2 += seg[i].lazyVal;
			seg[c].lazyVal += seg[i].lazyVal;
			seg[c].sum += seg[i].lazyVal * (node_sz(c) - seg[c].cntMin);
			assert(0 <= seg[c].minVal && seg[c].minVal < seg[c].minVal2);
		}
	}
	seg[i].lazyVal = 0;
	seg[i].oldMinVal = seg[i].minVal;
	// shouldn't need to update
}

void modify(int i, ll d) {
	if (d + seg[i].minVal2 <= 0) {
		assert(i < S);
		propogate(i);
		modify(2*i, d);
		modify(2*i+1, d);
		update(i);
		return;
	}

	seg[i].minVal2 += d;
	seg[i].lazyVal += d;
	seg[i].sum += d * (node_sz(i) - seg[i].cntMin);

	seg[i].sum -= seg[i].cntMin * seg[i].minVal;
	seg[i].minVal = max(seg[i].minVal + d, 0ll);
	seg[i].sum += seg[i].cntMin * seg[i].minVal;

	assert(seg[i].minVal < seg[i].minVal2);
}

void propogateAll(int i) {
	if (!i) return;
	propogateAll(i/2);
	propogate(i);
}

void updateAll(int a) {
	for (; a; a /= 2) update(a);
}

void updateRange(int l, int r, ll d) {
	//cerr << "updateRange " << l << ' ' << r << ' ' << d << '\n';
	assert(l <= r);
	propogateAll(S+l);
	propogateAll(S+r);
	int leftUp = 0;
	int rightUp = 0;
	for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			modify(a, d);
			if (leftUp == 0) leftUp = a/2;
			a++;
		}
		if (b & 1) {
			--b;
			modify(b, d);
			if (rightUp == 0) rightUp = b/2;
		}
	}
	updateAll(leftUp);
	updateAll(rightUp);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> C[i];
	}

	for (int i = 2*S-1; i >= S; i--) update(i);
	for (int i = 0; i < N; i++) {
		modify(S+i, C[i]);
	}
	for (int i = S-1; i; i--) update(i);
	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int t, i, j; ll d; cin >> t >> i >> j >> d; i--, j--;
		if (t == 1) {
			updateRange(i, j, d);
		} else if (t == 2) {
			ll oldSum = seg[1].sum;
			updateRange(i, j, -d);
			ll newSum = seg[1].sum;
			cout << oldSum - newSum << '\n';
		} else assert(false);
	}

	return 0;
}
