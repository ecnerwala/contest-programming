#include<bits/stdc++.h>
using namespace std;

const int MAXX = int(2.5e5);
const int S = 1 << 19;
int seg[S * 2];
int best[S * 2];

void update_one(int a) {
	if (a < S) {
		best[a] = max(best[2 * a], best[2 * a + 1]);
	} else {
		best[a] = 0;
	}
	best[a] += seg[a];
}

// [l,r)
void update(int l, int r, int v) {
	for (int a = l+S, b = r+S; a <= b; a /= 2, b /= 2) {
		if ((a & 1) == 1) {
			seg[a] += v;
			update_one(a);
			a++;
		}
		if ((b & 1) == 0) {
			seg[b] += v;
			update_one(b);
			b--;
		}
	}
	for (int a = l + S, b = r - 1 + S; a && b; a /= 2, b /= 2) {
		assert(bool(a) == bool(b));
		update_one(a);
		update_one(b);
	}
}

int query() {
	return best[1];
}

const int MAXN = int(1e5) + 200;
int N;
int Y1[MAXN], Y2[MAXN];
pair<int, int> X1[MAXN], X2[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d %d %d %d", &X1[i].first, &Y1[i], &X2[i].first, &Y2[i]);
		X1[i].first += MAXX;
		X2[i].first += MAXX;
		Y1[i] += MAXX;
		Y2[i] += MAXX;
		X1[i].second = X2[i].second = i;
	}
	sort(X1, X1 + N);
	sort(X2, X2 + N);
	int res = 0;
	for (int a = 0, b = 0; a < N; ) {
		assert(a < N);
		assert(b <= a);
		assert(b < N);
		if (X1[a].first <= X2[b].first) {
			// do a
			int i = X1[a].second;
			update(Y1[i], Y2[i] + 1, 1);
			res = max(res, query());
			a++;
		} else {
			// do b
			int i = X2[b].second;
			update(Y1[i], Y2[i] + 1, -1);
			res = max(res, query());
			b++;
		}
	}
	printf("%d\n", res);

	return 0;
}
