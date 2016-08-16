#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 2e9;

ll A, M, P, Q, R, S;
vector<ll> res; // positive is A, negative is M

void go() {
	for(int m = 0; (!m || M > 1) && S - R >= Q - P && S >= Q; m++, Q *= M, P *= M) {

		assert(m <= 35);

		ll val = m;

		ll d = max(R - P, 0ll);
		d += A - 1;
		d /= A;
		ll md = S - Q;
		md /= A;

		if(d > md) {
			continue;
		}

		vector<ll> nres;
		for(int i = 0; i < m; i++, d /= M, md /= M) {
			if(d % M && d / M < md / M) {
				d = ((d / M) + 1) * M;
			}
			if(d % M) {
				val += d % M;
				nres.push_back((d % M) * 2);
				nres.push_back(1);
			} else if(nres.empty()) {
				nres.push_back(1);
			} else {
				assert(nres.back() % 2);
				nres.back() += 2;
			}
		}
		if(d) {
			val += d;
			nres.push_back(d);
		}
		nres.push_back(val);
		reverse(nres.begin(), nres.end());

		if(nres < res) {
			res = nres;
		}
	}
}

int main() {
	for(int t = 1; true; t++) {
		scanf("%lld %lld %lld %lld %lld %lld", &A, &M, &P, &Q, &R, &S);
		if(A == 0) break;
		best = INF;
		res.clear();
		go();
		printf("Case %d:", t);
		if(best == INF) {
			printf(" impossible\n");
		} else {
			ll p = 0;
			for(ll i : res) {
				if(!i) continue;
				if(p * i < 0) {
					printf(" %lld%c", abs(p), (p > 0) ? 'A' : 'M');
					p = 0;
				}
				p += i;
			}
			if(p) printf(" %lld%c\n", abs(p), (p > 0) ? 'A' : 'M');
			else printf(" empty\n");
		}
	}

	return 0;
}
