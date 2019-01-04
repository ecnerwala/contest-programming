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

inline int getTime(int i) {
	return (val[i] != 0 && nxt[i] == N) ? val[i] - 1 : val[i];
}

bool isGood() {
	if(N == 1) return true;
	for(int i = 0; i < N; i++) {
		prv[i] = i - 1;
		nxt[i] = i + 1;
		has[i] = true;
	}
	prv[N] = N - 1;

	priority_queue<pii> pq;
	for(int i = 1; i < N; i++) {
		if(val[i] == 0 || nxt[i] != N) pq.push(pii(-val[i], i));
		else pq.push(pii(-max(1, val[i] - 1), i));
	}

	while(!pq.empty()) {
		int t = -pq.top().A;
		//cerr << "time " << t << '\n';
		bool changed = false;
		while(!pq.empty() && -pq.top().A == t) {
			int i = pq.top().B;
			pq.pop();
			if(!has[i]) continue;
			if(t != max(t, getTime(i))) continue;
			int p = prv[i];
			int n = nxt[i];
			//cerr << i << ' ' << p << ' ' << n << '\n';
			if(n == N && val[i] > 0) {
				//remove this
				has[i] = false;
				if(p == -1) return true;
				nxt[p] = N;
				prv[N] = p;
				changed = true;
			} else {
				//val[i] = 0
				if(p == -1) {
					return true;
					assert(false);
					continue;
				} else if ((val[p] - t) % 2 == 0) {
					return false;
				} else if(prv[p] == -1 && val[p] - t <= 0) {
					//prv is start and equals '1'
					nxt[p] = n;
					val[p] = t + 2;
				} else if(val[p] - t == 1) {
					nxt[p] = n;
					val[p] = t + 2;
					pq.push(pii(-getTime(p), p));
				} else if(val[p] - t == 3) {
					val[p] --;
					val[i] ++;
					pq.push(pii(-getTime(p), p));
					pq.push(pii(-getTime(i), i));
				} else {
					return false;
				}
			}
		}
		// oops that's a lot of extra insertions... lulz
		// we know that val[prv[N]] > t (otherwise we would've killed), 
		if(changed) {
			if(prv[prv[N]] == -1) return true;
			pq.push(pii(-max(t + 1, getTime(prv[N])), prv[N]));
		}
	}
	assert(false);

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
