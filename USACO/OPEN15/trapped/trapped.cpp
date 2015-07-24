#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;
#define A first
#define B second

const int MAXN = 2e5;

int N;

pii H[MAXN]; // bales, (pos, size)
pii R[MAXN]; // right endpoint, position

pii ranges[MAXN];

int main() {
	if(fopen("trapped.in", "r")) {
		freopen("trapped.in", "r", stdin);
		freopen("trapped.out", "w", stdout);
	}

	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%lld%lld", &H[i].B, &H[i].A);

		R[i] = pii(H[i].A + H[i].B, H[i].A);
	}

	sort(H, H + N);
	sort(R, R + N);

	//for(int i = 0; i < N; i++) cerr << H[i].A - H[i].B << ' ' << H[i].A << ' ' << H[i].A + H[i].B << '\n';

	// we want to query min location that still covers
	// store positions that work
	set<ll> pq;

	for(int i = 0, j = 0; i < N; i++) {

		assert(!pq.count(H[i].A));
		pq.insert(H[i].A);

		while(j < N && R[j].A < H[i].A) {
			assert(j <= i);
			assert(pq.count(R[j].B));
			pq.erase(R[j].B);
			j++;
		}

		ll l = *pq.lower_bound(H[i].A - H[i].B);

		ranges[i] = pii(l, H[i].A);
		//cerr << l << ' ' << H[i].A << '\n';

	}
	
	sort(ranges, ranges + N);

	ll res = 0;
	ll end = 0; // right point
	for(int i = 0; i < N; i++) {
		assert(ranges[i].A > 0);
		if(ranges[i].A > end) {
			res += end;
			res -= ranges[i].A;
		}
		end = max(end, ranges[i].B);
	}

	res += end;

	printf("%lld\n", res);

	return 0;
}
