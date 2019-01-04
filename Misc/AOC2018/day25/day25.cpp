#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2000;
int N;
struct pt {
	int x,y,z,w;
	friend int dist(const pt& a, const pt& b) {
		return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z) + abs(a.w - b.w);
	}
	friend bool close(const pt& a, const pt& b) {
		return dist(a,b) <= 3;
	}
} pts[MAXN];

bool vis[MAXN];
void flood(int i) {
	if (vis[i]) return;
	vis[i] = true;
	for (int j = 0; j < N; j++) {
		if (close(pts[i], pts[j])) {
			flood(j);
		}
	}
}

int main() {
	while (true) {
		assert(N < MAXN);
		int v = scanf("%d,%d,%d,%d", &pts[N].x, &pts[N].y, &pts[N].z, &pts[N].w);
		if (v == 4) {
			N++;
		} else {
			break;
		}
	}
	int res = 0;
	for (int i = 0; i < N; i++) {
		if (vis[i]) continue;
		res++;
		flood(i);
	}
	cerr << res << '\n';
	cout << res << '\n';

	return 0;
}
