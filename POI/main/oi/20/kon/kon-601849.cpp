#include<algorithm>
#include<cassert>
#include<iostream>
#include<cstring>
using namespace std;

const int D = 19;
const int S = 1 << D;
const int MAXS = 2 * S + 20;

const int INF = 1e9;

int N;
int K;

int cnt[MAXS];
int ma[MAXS];
bool full[MAXS];
int lazy[MAXS];

int insert(int a, int b, int v, int i, int l, int r) {
	//cerr << a << ' ' << b << ' ' << v << ' ' << i << ' ' << l << ' ' << r << '\n';
	assert(l <= a && a < b && b <= r);
	if(full[i]) {
		return b - a;
	} else if(r - l == 1) {
		assert(b - a == 1);
		lazy[i] = 0;
		ma[i] += v;
		cnt[i] = full[i] = ma[i] >= K;
		if(full[i]) { ma[i] = -INF; }
		return cnt[i];
	} else {

		//propogate, no harm
		lazy[2 * i] += lazy[i];
		ma[2 * i] += lazy[i];
		lazy[2 * i + 1] += lazy[i];
		ma[2 * i + 1] += lazy[i];
		lazy[i] = 0;
		 
		if(b - a == r - l && v + ma[i] < K) {
			// no changes will occur
			ma[i] += v;
			lazy[i] += v;
			return cnt[i];
		} else {
			int m = (l + r) / 2;

			int res = 0;
			if(a < m) res += insert(a, min(b, m), v, 2 * i + 0, l, m);
			if(m < b) res += insert(max(a, m), b, v, 2 * i + 1, m, r);

			assert(!lazy[i]);
			ma[i] = max(ma[2 * i], ma[2 * i + 1]);
			assert(ma[i] < K);
			cnt[i] = cnt[2 * i] + cnt[2 * i + 1];
			full[i] = full[2 * i] && full[2 * i + 1];
			if(full[i]) { ma[i] = -INF; }
			assert(ma[i] < K);

			return res;
		}
	}
}

void inicjuj(int n, int k, int *H) {
	N = n, K = k;
	memset(cnt, 0, sizeof(cnt));
	memset(ma, 0, sizeof(ma));
	memset(full, 0, sizeof(full));
	memset(lazy, 0, sizeof(lazy));
	for(int i = 0; i < N; i++) {
		insert(i, i + 1, H[i], 1, 0, N);
	}
}

void podlej(int a, int b) {
	b++;
	insert(a, b, 1, 1, 0, N);
}

int dojrzale(int a, int b) {
	b++;
	return insert(a, b, 0, 1, 0, N);
}
