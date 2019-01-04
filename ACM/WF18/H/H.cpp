#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.5e6;
int N;

int W, H;
int P;

int to_line(int x, int y) {
	if (y == 0) {
		return x;
	} else if (x == W) {
		return W + y;
	} else if (y == H) {
		return W + H + (W - x);
	} else if (x == 0) {
		return W + H + W + (H - y);
	} else assert(false);
	return 0;
}

pair<int, int> L[MAXN];

void print(int i) {
	assert(0 < i && i <= P);
	if (0 < i && i <= W) {
		printf("%d.5 %d", i-1, 0);
	} else if (W < i && i <= W + H) {
		printf("%d %d.5", W, i-W-1);
	} else if (W + H < i && i <= W + H + W) {
		printf("%d.5 %d", W-(i-W-H), H);
	} else if (W + H + W < i && i <= W + H + W + H) {
		printf("%d %d.5", 0, H-(i-W-H-W));
	} else assert(false);
}

void print(int a, int b) {
	assert(a <= b);
	assert(0 < a);
	assert(b <= P);
	// we want to print a-0.5, b-0.5
	if (0 < a && a <= W && 0 < b && b <= W) {
		b = W + 1;
	}
	if (W < a && a <= W + H && W < b && b <= W + H) {
		b = W + H + 1;
	}
	if (W + H < a && a <= W + H + W && W + H < b && b <= W + H + W) {
		b = W + H + W + 1;
	}
	if (W + H + W < a && a <= W + H + W + H && W + H + W < b && b <= W + H + W + H) {
		a = W + H + W;
	}
	print(a);
	printf(" ");
	print(b);
	printf("\n");
}

int locs[2 * MAXN];
int pref[2 * MAXN];

int main() {
	scanf("%d %d %d", &N, &W, &H);
	P = 2 * W + 2 * H;

	for (int i = 0; i < N; i++) {
		int x1, y1, x2, y2; scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
		int l1 = to_line(x1, y1), l2 = to_line(x2, y2);
		assert(l1);
		assert(l2);
		if (l1 > l2) {
			swap(l1, l2);
		}
		assert(0 < l1 && l1 < l2 && l2 < P);
		L[i] = {l1, l2};
		locs[2*i] = l1;
		locs[2*i+1] = l2;
	}
	sort(locs, locs + 2 * N);
	assert(unique(locs, locs + 2 * N) == locs + 2 * N);
	for (int i = 0; i < N; i++) {
		int l1 = int(lower_bound(locs, locs + 2 * N, L[i].first) - locs);
		int l2 = int(lower_bound(locs, locs + 2 * N, L[i].second) - locs);
		assert(L[i].first == locs[l1] && L[i].second == locs[l2]);
		if (l2 - l1 <= N) {
			pref[l1] ++;
			pref[l2] --;
		} else {
			pref[0] ++;
			pref[l1] --;
			pref[l2] ++;
			pref[2 * N] --;
		}
	}
	for (int i = 0; i < 2*N; i++) {
		pref[i+1] += pref[i];
	}
	assert(pref[2*N] == 0);

	for (int i = 0; i < N; i++) {
		if (pref[i] + pref[i + N] == N) {
			printf("1\n");
			print(locs[i]+1, locs[i+N]+1);
			return 0;
		}
	}

	printf("2\n");
	print(1, W + H + 1);
	print(W + 1, W + H + W + 1);
	return 0;
}
