#include<bits/stdc++.h>
using namespace std;

const int INF = 1.1e9;

const int MAXN = 2e6;

int N;
pair<int, int> D[MAXN];
int H[MAXN];

const int S = 1 << 20;

int seg[S * 2];

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d", &D[i].first);
		D[i].second = i;
	}
	sort(D, D + N);

	for(int i = 0; i < N; i++) {
		H[D[i].second] = i;
	}

	int Q; scanf("%d", &Q);

	for(int q = 0; q < Q; q++) {
		int k; scanf("%d", &k);

		for(int i = 1; i < 2 * S; i++) seg[i] = INF;

		for(int i = 0; i < N; i++) {
			int h = H[i];
			int v;
			if(!i) {
				v = 0;
			} else {
				v = seg[1] + 1;
				for(int c = h + S, r = 2 * S - 1; c <= r; c /= 2, r /= 2) {
					if(c % 2 == 1) {
						v = min(v, seg[c++]);
					}
				}
			}

			if(i == N - 1) {
				printf("%d\n", v);
				break;
			}

			if(i >= k) {
				for(int c = H[i - k] + S; c; c /= 2) {
					if(c >= S) {
						seg[c] = INF;
					} else {
						seg[c] = min(seg[2 * c], seg[2 * c + 1]);
					}
				}
			}
			for(int c = h + S; c; c /= 2) {
				if(c >= S) {
					seg[c] = v;
				} else {
					seg[c] = min(seg[2 * c], seg[2 * c + 1]);
				}
			}
		}
	}

	return 0;
}
