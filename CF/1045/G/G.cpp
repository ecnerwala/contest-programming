#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
struct bit {
	int V[MAXN];
	void reset() {
		memset(V, 0, sizeof(V));
	}
	void update(int a) {
		// update the ith guy, 0-indexed
		a++;
		for (; a <= N; a += a & (-a)) {
			V[a]++;
		}
	}
	int query(int a) {
		int res = 0;
		for(; a; a -= a & (-a)) {
			res += V[a];
		}
		return res;
	}
};

int Xs[MAXN];
int Qs[MAXN];

pair<int, pair<int, int>> robots[MAXN];

int K;
bit bits[MAXN][4];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		int x, r, q; cin >> x >> r >> q;
		Xs[i] = x;
		Qs[i] = q;
		robots[i].first = r;
		robots[i].second = pair<int, int>(x, q);
	}
	sort(Xs, Xs + N);
	sort(Qs, Qs + N);

	sort(robots, robots + N);
	long long res = 0;
	for (int i = N-1; i >= 0; i--) {
		int r = robots[i].first;
		int x = robots[i].second.first;
		int q = robots[i].second.second;
		int xst = int(lower_bound(Xs, Xs + N, x-r) - Xs);
		int xen = int(upper_bound(Xs, Xs + N, x+r) - Xs);
		int qst = int(lower_bound(Qs, Qs + N, q-K) - Qs);
		int qen = int(upper_bound(Qs, Qs + N, q+K) - Qs);
	}

	return 0;
}
