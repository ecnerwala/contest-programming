#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6;
const int MAXM = 1e6;

int N, M;
int P[MAXN];

vector<int> times[MAXN];

int getnum(const vector<int> &v, int a, int b) {
	return lower_bound(v.begin(), v.end(), b) - lower_bound(v.begin(), v.end(), a);
}

int query(int a, int b) {
	b++;
	int l = b - a;

	for(int i = 0; i <= 40; i++) {
		int v = P[a + rand() % l];
		int n = getnum(times[v], a, b);
		if(n * 2 > l) return v;
	}
	return -1;
}

int main() {
	srand(10);

	scanf("%d%d", &N, &M);
	for(int i = 0; i < N; i++) {
		scanf("%d", &P[i]);
		P[i] --;
		times[P[i]].push_back(i);
	}

	for(int i = 0; i < M; i++) {
		int a, b; scanf("%d%d", &a, &b);
		a--, b--;
		printf("%d\n", query(a, b) + 1);
	}
	return 0;
}
