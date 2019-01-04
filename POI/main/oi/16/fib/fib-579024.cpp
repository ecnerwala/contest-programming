#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1e6 + 20;

int N;
int val[MAXN];

int prv[MAXN];
int nxt[MAXN];
bool has[MAXN];

int isGood() {
	if(N == 1) return val[0];

	for(int i = 0; i < N; i++) {
		prv[i] = i - 1;
		nxt[i] = i + 1;
		has[i] = true;
	}
	prv[N] = N - 1;

	int lastMod = -1;
	int extra = 0;

	/*
	priority_queue<pii> pq;

	for(int i = 0; i < N; i++) {
		pq.push(pii(-val[i], -i));
	}

	if(val[prv[N]] >= 3) {
		pq.push(pii(-(val[prv[N]] - 3), -prv[N]));
	} else if(val[prv[N]] >= 1) {
		pq.push(pii(-(val[prv[N]] - 1), -prv[N]));
	}
	*/

	for(int i = prv[N], t = 0; true; ) {

		//cerr << "time " << t << ' ' << i << '\n';

		int p = prv[i];
		int n = nxt[i];
		//cerr << i << ' ' << p << ' ' << n << ' ' << v << '\n';
		
		//for(int k = prv[N]; k >= 0; k = prv[k]) cerr << k << ' ' << val[k] << '\n'; cerr << '\n';

		if(p != -1 && val[i] <= t) {
			if(prv[p] != -1 && val[p] <= val[i]) {
				//we need to compute the previous first
				i = p;
				continue;
			} else if ((val[p] - val[i]) % 2 == 0) {
				//oops, 0 0
				return -1;
			} else if((prv[p] == -1 && val[p] - val[i] <= 0) || val[p] - val[i] == 1) {
				nxt[p] = n;
				prv[n] = p;
				val[p] = val[i] + 2;
			} else if(val[p] - val[i] == 3) {
				val[p] = val[i] + 2;
				val[i] = val[i] + 2;
			} else {
				//can only merge with 1 and 101
				return -1;
			}

			i = n;
			continue;
		} else if(n == N) {
			if(p == -1) {
				if(val[i] <= t) {
					val[i] = t + (t - val[i] + 1) % 2 + 1; // at least t + 1
				}
			}

			if(val[i] - 3 >= t) t = val[i] - 3;
			else if(val[i] - 1 >= t) t = val[i] - 1;
			else assert(false);

			if(extra) {
				extra -= t - lastMod;
				lastMod = t + 1;
				if(extra < 1) extra = 1;
			}
			extra ++;
			if(extra > 3) extra = 3;

			if(val[i] - t == 1) {
				//remove this
				has[i] = false;
				nxt[p] = N;
				prv[N] = p;

				if(p == -1) {
					return t + extra;
				}

				i = p;
			} else {
				val[i] = t + 2;
			}

			t++;
		}

		//for(int k = prv[N]; k >= 0; k = prv[k]) cerr << k << ' ' << val[k] << '\n'; cerr << '\n';

		//for(int k = prv[N]; k >= 0; k = prv[k]) cerr << k << ' ' << val[k] << '\n'; cerr << '\n';
	}


	assert(false);
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d", &(val[i]));
	}

	int res = isGood();
	//cerr << res << '\n';
	if(res >= 0) {
		printf("%d\n", res);
	} else {
		printf("NIE\n");
	}

	return 0;
}

