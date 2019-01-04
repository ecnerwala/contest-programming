#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 100;
int N;
pair<ll, ll> pts[MAXN];

bool go(ll x) {
	ll res = 0;
	priority_queue<ll> pq;
	vector<ll> to_add;
	ll last = -1;
	for (int i = 0; i < N; i++) {
		if (pts[i].first != last) {
			for (ll v : to_add) {
				pq.push(v);
			}
			to_add.clear();
			last = pts[i].first;
		}
		assert(last == pts[i].first);
		ll v = pts[i].first * 1000 - pts[i].second * x;
		res += v;
		cerr << pts[i].first << ' ' << v << '\n';
		if (v <= 0) {
			if (!pq.empty()) {
				// we do a pop
				res -= pq.top();
				pq.pop();
			}
		} else {
			assert(v > 0);
			to_add.push_back(v);
		}
	}
	return res <= 0;
}

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		int a; scanf("%d", &a);
		pts[i].first = a;
	}
	for (int i = 0; i < N; i++) {
		int b; scanf("%d", &b);
		pts[i].second = b;
	}
	sort(pts, pts + N);
	go(1200);
	ll mi = 0;
	ll ma = 1e12;
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (go(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';
	return 0;
}
