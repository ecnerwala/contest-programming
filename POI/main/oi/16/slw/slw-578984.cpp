#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1.5e5;

int N;
int val[MAXN];

int prv[MAXN];
int nxt[MAXN];
bool has[MAXN];

bool isGood() {
	if(N == 1) return true;
	for(int i = 0; i < N; i++) {
		prv[i] = i - 1;
		nxt[i] = i + 1;
		has[i] = true;
	}
	prv[N] = N - 1;

	if(val[prv[N]] >= 3) val[prv[N]] --;

	priority_queue<pii> pq;

	for(int i = 1; i < N; i++) {
		pq.push(pii(-val[i], -i));
	}

	while(!pq.empty()) {
		int t = -pq.top().A;
		//cerr << "time " << t << '\n';
		int i = -pq.top().B;
		pq.pop();
		if(!has[i]) continue;
		if(t != val[i]) continue;
		int p = prv[i];
		int n = nxt[i];
		//cerr << i << ' ' << p << ' ' << n << '\n';
		if(n == N && val[i] > 0) {
			//remove this
			has[i] = false;
			if(p == -1) return true;
			nxt[p] = N;
			prv[N] = p;
		} else {
			//val[i] = 0
			if(p == -1) {
				continue;
			} else if ((val[p] - t) % 2 == 0) {
				return false;
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
				return false;
			}
		}

		if(n == N) {
			if(prv[prv[N]] == -1) return true;

			if(val[prv[N]] - t >= 3) {
				val[prv[N]] --;
				pq.push(pii(-val[prv[N]], -prv[N]));
			}
		}
	}

	return true;
}

int main() {
	int Z;
	scanf("%d", &Z);
	for(int z = 0; z < Z; z++) {
		scanf("%d", &N);
		for(int i = 0; i < N; i++) {
			scanf("%d", &(val[i]));
		}

		bool res = isGood();
		//cerr << res << '\n';
		if(res) {
			printf("TAK\n");
		} else {
			printf("NIE\n");
		}
	}

	return 0;
}

