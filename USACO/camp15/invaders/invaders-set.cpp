#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

typedef pair<ll, ll> pii;
#define A first
#define B second

typedef set<pii> st;
typedef st::iterator it;

typedef priority_queue<pii, vector<pii>, greater<pii>> pq;

st pos; // ranges, stored by left & right - time (< left)

const int MAXN = 1e5;
int N;
pii att[MAXN]; // attacks, stored by time, coord

ll W;

ll T;

bool is_good(ll w) {
	W = w;

	pos.clear();
	pos.insert(pii(0, 0));
	
	pq merges;

	for(int i = 0; i < N; i++) {
		while(!merges.empty() && merges.top().A <= att[i].A) {
			T = merges.top().A;
			ll x = merges.top().B;
			merges.pop();

			it r = pos.lower_bound(pii(x, -INF));
			if(r == pos.end()) continue;
			assert(*r >= pii(x, -INF));
			assert(r->A >= x);
			if(r->A > x) continue;

			it l = r;
			if(l == pos.begin()) continue;
			l--;

			assert(l->B + T <= r->A);

			if(l->B + T < x) continue;

			pii nxt = pii(l->A, r->B);

			pos.erase(l);
			pos.erase(r);

			pos.insert(nxt);
		}

		assert(!pos.empty());

		// process the attack
		T = att[i].A;
		ll a = att[i].B - W, b = att[i].B + W;

		it l, r; // first and last to be removed

		// find whatever stradles a and split it
		l = pos.upper_bound(pii(a, INF));
		if(l != pos.begin()) {
			l--;

			// last with l->A <= a
			assert(l->A <= a);
			assert(l->A <= l->B + T);

			if(l->B + T > a) {
				pii n1(l->A, a - T);
				pii n2(a + 1, l->B);
				assert(n2.A <= n2.B + T);
				pos.erase(l);
				pos.insert(n1);
				pos.insert(n2);
			}
		}

		// find whatever stradles b and split it
		r = pos.lower_bound(pii(b, -INF));
		if(r != pos.begin()) {
			r--;

			// last with r->A < b
			assert(r->A < b);
			assert(r->A <= r->B + T);

			if(r->B + T >= b) {
				pii n1(r->A, b - 1 - T);
				pii n2(b, r->B);

				assert(n1.A <= n1.B + T);
				pos.erase(r);
				pos.insert(n1);
				pos.insert(n2);
			}
		}

		l = pos.upper_bound(pii(a, INF));
		r = pos.lower_bound(pii(b, -INF));
		pos.erase(l, r);

		if(pos.empty()) return false;

		// generate the new merge
		if(r == pos.end()) continue;
		l = r;
		if(l == pos.begin()) continue;
		l--;

		ll nt = r->A - l->B;
		assert(l->B + nt == r->A);

		merges.push(pii(nt, r->A));
	}

	return !pos.empty();
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> att[i].A >> att[i].B;
		att[i].B += att[i].A;
		att[i].A *= 4;
		att[i].B *= 2;
	}

	sort(att, att + N);

	ll mi = 0;
	ll ma = 1e10; // we can't move far enough to dodge the first attack
	while(ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if(is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}

	cout << mi << '\n';
}
