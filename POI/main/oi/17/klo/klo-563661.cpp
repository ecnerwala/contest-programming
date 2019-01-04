#include<iostream>
#include<algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 20;
int N;
int X[MAXN];

ll val[MAXN];
int pos[MAXN];

const int MAXM = 60;
int M;
int K[MAXM];
int res[MAXM];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < N; i++) cin >> X[i];
	for(int i = 0; i < M; i++) cin >> K[i];

	val[0] = pos[0] = 0;
	for(int m = 0; m < M; m++) {
		int k = K[m];
		res[m] = 0;
		int sz = 1;
		ll v = 0;
		for(int i = 0; i < N; i++) {
			v += X[i];
			v -= k;
			//cerr << k << ' ' << i+1 << ' ' << v << '\n';
			if(v < val[sz - 1]) {
				val[sz] = v;
				pos[sz] = i + 1;
				sz++;
			} else {
				int ma = sz - 1;
				int mi = -1;
				while(ma - mi > 1) {
					int md = (mi + ma) / 2;
					if(v >= val[md]) {
						ma = md;
					} else {
						mi = md;
					}
				}
				//cerr << k << ' ' << i+1 << ' ' << v << ' ' << pos[ma] << ' ' << val[ma] << '\n';
				int cnd = i + 1 - pos[ma];
				if(cnd > res[m]) res[m] = cnd;
			}
		}
	}
	cout << res[0];
	for(int i = 1; i < M; i++) {
		cout << ' ' << res[i];
	}
	cout << '\n';
}
