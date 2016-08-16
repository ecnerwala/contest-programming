#include<bits/stdc++.h>
using namespace std;

typedef long double ld;
const ld EPS = 1e-7;

const int MAXN = 6;

ld W, H, D;
int N;
int C[MAXN];
ld R[MAXN];

bool vis[200][200][200];

void prt(int t, ld a, ld b, ld c) {
	cout << t + 1 << ' ';
	cout << fixed << setprecision(9) << a << ' ';
	cout << fixed << setprecision(9) << b << ' ';
	cout << fixed << setprecision(9) << c << '\n';
}

void go1() {
	cin >> W >> H >> D;
	cin >> N;
	for(int i = 0; i < N; i++) cin >> C[i] >> R[i];
	
	assert(N == 2);

	ld u = R[1] * 2;
	int f = -1;
	if(abs(R[0] - R[1] * 2) <= EPS) {
		f = 2;
	} else if(abs(R[0] - R[1] * 3) <= EPS) {
		f = 3;
	} else assert(false);

	int w = floor(W / u);
	int h = floor(H / u);
	int d = floor(D / u);

	memset(vis, 0, sizeof(vis));

	for(int l = 0; l < C[0]; l++) {
		int i = l % (w / f), j = l / (w / f) % (h / f), k = l / (w / f) / (h / f) % (d / f);
		assert(l / (w / f) / (h / f) / (d / f) == 0);
		i *= f, j *= f, k *= f;
		prt(0, (u * i) + R[0], (u * j) + R[0], (u * k) + R[0]);
		for(int x = 0; x < f; x++) {
			for(int y = 0; y < f; y++) {
				for(int z = 0; z < f; z++) {
					vis[i + x][j + y][k + z] = true;
				}
			}
		}
	}

	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++) {
			for(int k = 0; k < d; k++) {
				if(vis[i][j][k]) continue;
				if(!C[1]) continue;
				C[1] --;
				prt(1, u * i + R[1], u * j + R[1], u * k + R[1]);
			}
		}
	}
	cout << '\n';
}

int main() {
	int T; cin >> T;
	while(T--) {
		go1();
	}
	return 0;
}
