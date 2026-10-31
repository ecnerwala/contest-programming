#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
const int S = 1 << 18;
int N;
vector<int> v[2 * S];
void go() {
	cin >> N;

	for (int i = 1; i < 2 * S; i++) v[i] = {};
	for (int i = 0; i < N; i++) {
		int r, s; cin >> r >> s;
		for (int a = max(0, i - r) + S, b = min(N, i + r + 1) + S; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				v[a].push_back(-s);
				a++;
			}
			if (b & 1) {
				--b;
				v[b].push_back(-s);
			}
		}
	}
	int ans = 0;
	for (int i = 1; i < 2 * S; i++) {
		if (i > 1) {
			v[i].insert(v[i].end(), v[i/2].begin(), v[i/2].end());
		}
		if (v[i].size() > 3) {
			nth_element(v[i].begin(), v[i].begin() + 3, v[i].end());
			v[i].resize(3);
		}
	}
	for (int i = S; i < 2 * S; i++) {
		if (v[i].size() == 3) {
			ans = max(ans, -(v[i][0] + v[i][1] + v[i][2]));
		}
	}
	cout << ans << '\n';
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Z; cin >> Z;
	while (Z--) {
		go();
	}

	return 0;
}
