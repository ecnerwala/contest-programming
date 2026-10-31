#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int S = 1 << 19;
struct seg_node {
	int c[2] = {0,0};
	ll s[2] = {0,0};
} seg[2*S];

seg_node operator + (const seg_node& a, const seg_node& b) {
	seg_node r;
	for (int z = 0; z < 2; z++) {
		r.c[z] = a.c[z] + b.c[z];
		r.s[z] = a.s[z] + b.s[z];
	}
	return r;
}
void updateNode(int i) {
	if (i < S) {
		seg[i] = seg[2*i+0] + seg[2*i+1];
	}
}

void updateAll(int i) {
	for (int a = i; a; a /= 2) updateNode(a);
}


const int MAXN = 5.1e5;
int N, Q;
int A[MAXN][2];
ll pref[MAXN][2];
vector<pair<int, int>> queries[MAXN];

const int MAXQ = 5.1e5;
ll ans[MAXQ];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> Q;
	for (int z = 0; z < 2; z++) {
		pref[0][z] = 0;
		for (int i = 0; i < N; i++) {
			cin >> A[i][z];
			pref[i+1][z] = pref[i][z] + A[i][z];
		}
	}

	for (int q = 0; q < Q; q++) {
		// half-open range on the prefixes
		int l, r; cin >> l >> r;
		queries[l-1].emplace_back(r, q);
	}

	vector<pair<ll, int>> diffs(N+1);
	for (int i = 0; i <= N; i++) {
		diffs[i] = {pref[i][1] - pref[i][0], i};
	}
	sort(diffs.begin(), diffs.end());

	for (int i = 0; i <= N; i++) {
		seg[S+i].c[0] = 0, seg[S+i].s[0] = 0;
		seg[S+i].c[1] = 1, seg[S+i].s[1] = pref[i][1];
	}
	for (int i = S-1; i >= 0; i--) updateNode(i);

	for (auto evt : diffs) {
		int i = evt.second;
		seg[S+i].c[0] = 1, seg[S+i].s[0] = pref[i][0];
		seg[S+i].c[1] = 0, seg[S+i].s[1] = 0;
		updateAll(S+i);

		for (auto query : queries[i]) {
			int r = query.first;
			seg_node val;
			for (int a = S + i + 1, b = S + r + 1; a < b; a /= 2, b /= 2) {
				if (a & 1) val = val + seg[a++];
				if (b & 1) val = val + seg[--b];
			}
			ans[query.second] = 0;
			for (int z = 0; z < 2; z++) {
				ans[query.second] += val.s[z] - val.c[z] * pref[i][z];
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
