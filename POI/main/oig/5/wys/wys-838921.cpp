#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6 + 200;
int N, M, D;
int S[MAXN];
int V[MAXN];

double go() {
	if (M == D) {
		double res = 0;
		for (int i = 0; i < N; i++) {
			res += double(S[i+1] - S[i]) / V[i];
		}
		return res;
	}

	assert(M < D);

	double val = 0;
	int l = 0, r = 0;
	int s = 0, t = M;
	while (S[r+1] <= t) {
		val += double(S[r+1] - S[r]) / V[r];
		r++;
	}
	assert(S[r] <= t);
	assert(S[r+1] > t);
	val += double(t - S[r]) / V[r];

	double res = val;

	while (r < N) {
		assert(S[l+1] > s);
		assert(S[r+1] > t);
		int dist = min(S[r+1] - t, S[l+1] - s);
		val += double(dist) / V[r];
		val -= double(dist) / V[l];
		res = min(res, val);

		t += dist;
		if (S[r+1] == t) r++;

		s += dist;
		if (S[l+1] == s) l++;
	}

	return res;
}

int main() {
	scanf("%d %d %d", &N, &M, &D);
	for (int i = 0; i < N; i++) {
		scanf("%d %d", &S[i], &V[i]);
	}

	S[N] = D;
	S[N+1] = D + M + 1;

	double res = go();

	printf("%.3f\n", res);

	return 0;
}
