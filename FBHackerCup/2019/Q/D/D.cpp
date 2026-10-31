#include<bits/stdc++.h>
using namespace std;

struct union_find {
private:
	unordered_map<int, int> pars;
public:
	void reset() { pars = {}; }
	int getpar(int a) {
		return pars.count(a) ? (pars[a] = getpar(pars[a])) : a;
	}
	bool merge(int a, int b) {
		a = getpar(a), b = getpar(b);
		if (a == b) {
			return false;
		}
		pars[a] = b;
		return true;
	}
};

mt19937 mt(48);

bool go(vector<int> inds, vector<pair<pair<int, int>, int>> lcas, int root_par, map<int, int>& pars) {
	//shuffle(inds.begin(), inds.end(), mt);
	for (int root : inds) {
		union_find dsu;
		for (auto it : lcas) {
			int a = it.first.first, b = it.first.second, c = it.second;
			if (c == root) {
				// leave it
			} else {
				if (a == root || b == root) {
					goto not_good;
				}
				dsu.merge(a, c);
				dsu.merge(b, c);
			}
		}
		for (auto it : lcas) {
			int a = it.first.first, b = it.first.second, c = it.second;
			if (c == root) {
				// two sides must be different
				if (a == root || b == root) {
					// leave it
				} else {
					if (dsu.getpar(a) == dsu.getpar(b)) {
						goto not_good;
					}
				}
			} else {
				// no constraints
			}
		}

		{ // it is good
			map<int, pair<vector<int>, vector<pair<pair<int, int>, int>>>> subproblems;
			for (auto i : inds) {
				if (i == root) continue;

				int p = dsu.getpar(i);
				subproblems[p].first.push_back(i);
			}
			for (auto it : lcas) {
				if (it.second == root) continue;
				int p = dsu.getpar(it.second);
				subproblems[p].second.push_back(it);
			}

			for (auto& it : subproblems) {
				if (!go(std::move(it.second.first), std::move(it.second.second), root, pars)) {
					//goto not_good;
					return false;
				}
			}
			pars[root] = root_par;
			return true;
		}

not_good:
		continue;
	}
	return false;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num++) {
		int N, M; cin >> N >> M;
		vector<int> inds;
		for (int i = 1; i <= N; i++) inds.push_back(i);
		vector<pair<pair<int, int>, int>> lcas;
		for (int j = 0; j < M; j++) {
			int a, b, c; cin >> a >> b >> c;
			lcas.emplace_back(pair<int, int>(a, b), c);
		}
		map<int, int> pars;
		bool ans = go(inds, lcas, 0, pars);
		cout << "Case #" << case_num << ": ";
		if (ans) {
			for (int i = 1; i <= N; i++) {
				cout << pars[i] << " \n"[i==N];
			}
		} else {
			cout << "Impossible" << '\n';
		}
	}

	return 0;
}
