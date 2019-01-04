#include<iostream>

using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 20;
const ll MAXM = 2e18;
int N, K;
ll M;
ll P[MAXN];

int nxt[MAXN];
int nxt2[MAXN];

int res[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K >> M;

	for(int i = 0; i < N; i++) cin >> P[i];

	int l = 0, r = K + 1;
	for(int i = 0; i < N; i++) {
		while(r < N && P[i] - P[l] > P[r] - P[i]) {
			l++, r++;
		}
		//cerr << i << ' ' << l << ' ' << r - 1 << ' ';
		//cerr << P[i] << ' ' << P[l] << ' ' << P[r - 1] << '\n';
		if(P[i] - P[l] >= P[r - 1] - P[i]) {
			nxt[i] = l;
		} else {
			nxt[i] = r - 1;
		}
	}

	//for(int i = 0; i < N; i++) cerr <<  nxt[i] << ' '; cerr << '\n';

	for(int i = 0; i < N; i++) {
		res[i] = i;
	}

	while(M) {
		if(M % 2) {
			for(int i = 0; i < N; i++) {
				res[i] = nxt[res[i]];
			}
		}

		M /= 2;

		for(int i = 0; i < N; i++) {
			nxt2[i] = nxt[nxt[i]];
		}
		for(int i = 0; i < N; i++) {
			nxt[i] = nxt2[i];
		}
	}

	for(int i = 0; i < N; i++) {
		cout << res[i] + 1 << (i == N - 1 ? '\n' : ' ');
	}

	return 0;
}
