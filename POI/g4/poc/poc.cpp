#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

const int MAXN = int(1e6) + 200;
int N;
int W[MAXN];
int M[MAXN];

int loc[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> W[i];
	}
	for (int i = 0; i < N; i++) {
		cin >> M[i];
		loc[M[i]] = i;
	}

	long long res = 0;
	int cur = 0;
	for (int i = N - 1; i >= 0; i--) {
		int v = W[i];
		int nxt = loc[v];
		res += (nxt - cur + N) % N;
		cur = nxt;
	}
	cout << (res + N - 1) / N << '\n';

	return 0;
}
