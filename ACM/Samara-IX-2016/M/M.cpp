#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXN = 2.1e5;
const int S = 1 << 18;

int seg[2 * S]; // min-segtree
int K;
int N;
char V[MAXN];
int prv[MAXN][27];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> K >> V;
	N = int(strlen(V));


	for (int v = 0; v <= 26; v++) {
		prv[0][v] = -1;
	}
	for (int i = 1; i <= N; i++) {
		for (int v = 0; v <= 26; v++) {
			prv[i][v] = prv[i-1][v];
		}
		prv[i][V[i-1] - 'a'] = i-1;
	}

	for (int i = 2 * S - 1; i > 0; i--) {
		seg[i] = INF;
	}
	seg[S+0] = 0;
	for (int a = (S+0) / 2; a; a /= 2) {
		seg[a] = min(seg[2*a], seg[2*a+1]);
	}

	for (int i = 1; i <= N; i++) {
		sort(prv[i], prv[i] + 27);
		reverse(prv[i], prv[i] + 27);
		int l = prv[i][K], r = prv[i][K-1];
		int v = INF;
		for (int a = S+l+1, b = S+r+1; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				v = min(v, seg[a]+1);
				a++;
			}
			if (b & 1) {
				--b;
				v = min(v, seg[b]+1);
			}
		}
		cout << (v == INF ? -1 : v) << " \n"[i==N];
		seg[S+i] = v;
		for (int a = (S+i) / 2; a; a /= 2) {
			seg[a] = min(seg[2*a], seg[2*a+1]);
		}
	}

	return 0;
}
