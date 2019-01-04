#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N;
int T[MAXN];
int P[MAXN][21];
int depth[MAXN];
int s_depth[MAXN];

int lca(int a, int b) {
	if (depth[a] < depth[b]) {
		swap(a, b);
	}
	assert(depth[a] >= depth[b]);
	for (int v = 0; depth[a] != depth[b]; v++) {
		if ((depth[a] - depth[b]) & (1 << v)) {
			a = P[a][v];
		}
	}
	if (a == b) return a;

	int v = 0;
	while (P[a][v] != P[b][v]) v++;
	assert(v >= 0);
	for (; v >= 0; v--) {
		assert(P[a][v+1] == P[b][v+1]);
		if (P[a][v] != P[b][v]) {
			a = P[a][v], b = P[b][v];
		}
		assert(P[a][v] == P[b][v]);
	}
	assert(P[a][0] == P[b][0]);
	return P[a][0];
}

bool is_case(int a, int b) {
	if (a == b) return true;
	return T[b] == 0 && s_depth[b] >= (depth[b] - depth[a]);
}
bool is_part(int a, int b) {
	if (a == b) return true;
	return T[b] == 1 && s_depth[b] >= (depth[b] - depth[a]);
}
bool query(int t, int a, int b) {
	if (a == b) return false;
	int c = lca(a, b);
	if (c == 0) return false;
	if (t == 1) {
		return a == c && is_case(a, b);
	} else if (t == 2) {
		return is_case(c, a) && is_part(c, b);
	} else assert(false);
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	memset(P, 0, sizeof(P));
	for (int i = 1; i <= N; i++) {
		cin >> P[i][0] >> T[i];
		if (P[i][0] == -1) {
			P[i][0] = 0;
			depth[i] = 0;
			assert(T[i] == -1);
			s_depth[i] = 0;
		} else {
			depth[i] = depth[P[i][0]] + 1;
			assert(T[i] != -1);
			if (T[P[i][0]] == T[i]) {
				s_depth[i] = s_depth[P[i][0]] + 1;
			} else {
				s_depth[i] = 1;
			}
		}
		for (int v = 0; P[i][v]; v++) {
			P[i][v+1] = P[P[i][v]][v];
		}
	}
	int Q; cin >> Q;
	for (int i = 0; i < Q; i++) {
		int t, a, b; cin >> t >> a >> b;
		cout << (query(t, a, b) ? "YES" : "NO") << '\n';
	}


	return 0;
}
