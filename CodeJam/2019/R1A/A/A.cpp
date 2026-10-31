#include<bits/stdc++.h>
using namespace std;

using pii = pair<int, int>;
#define A first
#define B second

bool pair_good(pii a, pii b) {
	return !(a.A == b.A || a.B == b.B || a.A + a.B == b.A + b.B || a.A - a.B == b.A - b.B);
}

bool chain_good(const vector<pii>& v) {
	for (int i = 0; i+1 < int(v.size()); i++) {
		if (!pair_good(v[i], v[i+1])) return false;
	}
	return true;
}

mt19937 mt(48);
bool dfs(vector<pii>& r, int i) {
	if (i == int(r.size())) {
		return true;
	}
	shuffle(r.begin() + i, r.end(), mt);
	for (int j = i; j < int(r.size()); j++) {
		swap(r[i], r[j]);
		if (i && !pair_good(r[i-1], r[i])) continue;
		if (dfs(r, i+1)) {
			return true;
		}
	}
	return false;
}

vector<pii> go(int R, int C) {
	vector<pii> res;
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			res.emplace_back(i, j);
		}
	}
	if (dfs(res, 0)) {
		return res;
	} else {
		//cerr << "No solution! " << R << " " << C << '\n';
		return {};
	}
}

vector<pii> transpose(const vector<pii>& v) {
	vector<pii> res;
	for (auto it : v) res.emplace_back(it.second, it.first);
	return res;
}

vector<pii> ans[30][30];
void precomp() {
	for (int r = 2; r <= 20; r++) {
		for (int c = 2; c <= 20; c++) {
			if (r == 3 && c == 18) continue;
			if (r == 17 && c == 3) continue;
			ans[r][c] = go(r, c);
		}
	}
	ans[3][18] = transpose(ans[18][3]);
	ans[17][3] = transpose(ans[3][17]);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int r, c; cin >> r >> c;

		cout << "Case #" << case_num << ": ";
		if (ans[r][c].empty()) {
			cout << "IMPOSSIBLE" << '\n';
		} else {
			cout << "POSSIBLE" << '\n';
			for (auto it : ans[r][c]) {
				cout << it.first << ' ' << it.second << '\n';
			}
		}
	}

	return 0;
}
