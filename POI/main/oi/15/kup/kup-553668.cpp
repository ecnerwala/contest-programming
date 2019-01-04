#include<iostream>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pii;
const int MAXN = 4e3;

ll K;
int N;
ll C[MAXN][MAXN];
ll P[MAXN][MAXN];

void precomp() {
	for(int i = 0; i <= N; i++) {
		P[i][0] = P[0][i] = 0;
	}
	for(int i = 1; i <= N; i++) {
		for(int j = 1; j <= N; j++) {
			P[i][j] = P[i - 1][j] + P[i][j - 1] - P[i - 1][j - 1] + C[i][j];
		}
	}
}

inline ll area(int minx, int miny, int maxx, int maxy) {
	minx--, miny--;
	return P[maxx][maxy] - P[minx][maxy] - P[maxx][miny] + P[minx][miny];
}

pair<pii, pii> go() {
	precomp();
	//for(int i = 1; i <= N; i++) { for(int j = 1; j <= N; j++) { cerr << C[i][j] << ' '; } cerr << '\n'; } cerr << '\n';
	//for(int i = 0; i <= N; i++) { for(int j = 0; j <= N; j++) { cerr << P[i][j] << ' '; } cerr << '\n'; } cerr << '\n';

	for(int y1 = 1; y1 <= N; y1++) {
		for(int y2 = y1; y2 <= N; y2++) {
			int lx = 1;
			for(int x = 1; x <= N; x++) {
				ll a = area(x, y1, x, y2);
				if(a < K) {
					if(area(lx, y1, x, y2) >= K) {
						return make_pair(pii(lx, y1), pii(x, y2));
					}
				} else if(a <= 2 * K) {
					return make_pair(pii(x, y1), pii(x, y2));
				} else {
					lx = x + 1;
				}
			}
		}
	}
	return make_pair(pii(-1, -1), pii(-1, -1));
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> K >> N;
	for(int j = 1; j <= N; j++) {
		for(int i = 1; i <= N; i++) {
			cin >> C[i][j];
		}
	}
	pair<pii, pii> res = go();
	if(res.first.first == -1) {
		cout << "NIE" << '\n';
	} else {
		cout << res.first.first << ' ' << res.first.second << ' ' << res.second.first << ' ' << res.second.second << '\n';
	}
	return 0;
}
