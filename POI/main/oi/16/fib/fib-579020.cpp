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
	if(val[0] == 0) return -1;
	for(int i = 0; i < N; i++) {
		prv[i] = i - 1;
		nxt[i] = i + 1;
		has[i] = true;
	}
	prv[N] = N - 1;

	int lastMod = -1;
	int extra = 0;

	priority_queue<pii> pq;

	for(int i = 0; i < N; i++) {
		pq.push(pii(-val[i], -i));
	}

	if(val[prv[N]] >= 3) {
		pq.push(pii(-(val[prv[N]] - 3), -prv[N]));
	} else if(val[prv[N]] >= 1) {
		pq.push(pii(-(val[prv[N]] - 1), -prv[N]));
	}

	while(!pq.empty()) {
		int t = -pq.top().A;
		int i = -pq.top().B;
		pq.pop();
		//cerr << "time " << t << ' ' << i << '\n';
		if(!has[i]) continue;

		int p = prv[i];
		int n = nxt[i];
		int v = val[i] - t;
		//cerr << i << ' ' << p << ' ' << n << ' ' << v << '\n';
		
		//for(int k = prv[N]; k >= 0; k = prv[k]) cerr << k << ' ' << val[k] << '\n'; cerr << '\n';

		if(n == N && (v == 3 || v == 1)) {

			if(extra) {
				extra -= t - lastMod;
				lastMod = t + 1;
				if(extra < 1) extra = 1;
			}
			extra ++;
			if(extra > 3) extra = 3;

			if(v == 1) {
				//remove this
				has[i] = false;
				nxt[p] = N;
				prv[N] = p;

				if(p == -1) {
					return t + extra;
				}

			} else {
				val[i] = t + 2;
				pq.push(pii(-(val[i] - 1), -i));
			}

		} else if(v == 0) {
			//val[i] = 0
			if(p == -1) {
				continue;
			} else if ((val[p] - t) % 2 == 0) {
				return -1;
			} else if((prv[p] == -1 && val[p] - t <= 0) || val[p] - t == 1) {
				nxt[p] = n;
				prv[n] = p;
				val[p] = t + 2;
				pq.push(pii(-val[p], -p));
			} else if(val[p] - t == 3) {
				val[p] = t + 2;
				val[i] = t + 2;
				pq.push(pii(-val[p], -p));
				pq.push(pii(-val[i], -i));
			} else {
				return -1;
			}
		}

		//for(int k = prv[N]; k >= 0; k = prv[k]) cerr << k << ' ' << val[k] << '\n'; cerr << '\n';

		if(n == N && prv[N] != -1) {
			if(prv[prv[N]] == -1) {
				if(val[prv[N]] - t <= 1) {
					val[prv[N]] = t + (t - val[prv[N]] + 2) % 2 + 2;
				}
			}
			if(val[prv[N]] - t > 3) {
				pq.push(pii(-(val[prv[N]] - 3), -prv[N]));
			} else if(val[prv[N]] - t > 1) {
				pq.push(pii(-(val[prv[N]] - 1), -prv[N]));
			}
		}

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

