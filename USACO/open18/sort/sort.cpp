#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N;
int A[MAXN];
pair<int, int> vals[MAXN];
int B[MAXN]; // inverse permutation

int T[MAXN-1];

int main() {
	if(fopen("sort.in", "r")) {
		freopen("sort.in", "r", stdin);
		freopen("sort.out", "w", stdout);
	}

	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		vals[i] = pair<int, int>(A[i], i);
	}
	sort(vals, vals + N);
	for (int i = 0; i < N; i++) {
		A[i] = int(lower_bound(vals, vals + N, pair<int, int>(A[i], i)) - vals);
	}

	for (int i = 0; i < N; i++) {
		B[A[i]] = i;
	}

	T[0] = 0;
	for (int i = 0; i < N; i++) {
		T[i+1] = max(T[i] - 1, B[i] - i);
		assert(T[i+1] >= 0);
	}
	assert(T[N] == 0);

	long long res = 0;
	for (int i = 0; i < N; i++) {
		res += max(1, max(T[i], T[i+1]));
	}
	printf("%lld\n", res);

	return 0;
}
