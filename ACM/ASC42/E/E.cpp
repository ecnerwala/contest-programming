#include<bits/stdc++.h>
using namespace std;

const int MAXV = 18;

using poly = vector<int>;
bool polyCmp(const poly& a, const poly& b) {
	if (a.size() != b.size()) return a.size() < b.size();
	for (int i = int(a.size()) - 1; i >= 0; i--) {
		if (a[i] != b[i]) return a[i] < b[i];
	}
	return false;
}

map<poly, vector<vector<const poly*>>> trees[19];
vector<const poly*> polies[19];
map<poly, vector<vector<const poly*>>> allTrees;

const poly* curTrees[20];
void dfs(int totLen, int lenLeft, int lastLen, int lastInd, int nChildren) {
	if (lenLeft == 0) {
		vector<const poly*> curPolies(curTrees, curTrees + nChildren);
		sort(curPolies.begin(), curPolies.end(), [&](const poly* a, const poly* b) { return polyCmp(*a, *b); });
		vector<int> nPoly(20);
		nPoly[0]++;
		for (int i = 0; i < nChildren; i++) {
			for (int j = 0; j < int(curPolies[i]->size()); j++) {
				nPoly[1+i+j] += (*curPolies[i])[j];
			}
		}
		while (nPoly.back() == 0) nPoly.pop_back();

		//for (int v : nPoly) { cerr << v << ' '; } cerr << '\n';
		//cerr << nString << '\n';

		trees[totLen][nPoly].push_back(std::move(curPolies));
		return;
	}

	if (lenLeft < lastLen) {
		lastLen = lenLeft;
		lastInd = 0;
	}
	assert(lastLen <= lenLeft);
	for (; lastLen > 0; lastLen --, lastInd = 0) {
		for (; lastInd < int(polies[lastLen].size()); lastInd++) {
			curTrees[nChildren] = polies[lastLen][lastInd];
			dfs(totLen, lenLeft - lastLen, lastLen, lastInd, nChildren + 1);
		}
	}
}

map<poly, vector<string>> mem;
const vector<string>& getTrees(const poly& p) {
	if (mem.count(p)) return mem[p];
	vector<string> res;
	for (const auto& children : allTrees[p]) {
		vector<vector<string>> poss(1);
		for (auto np : children) {
			vector<vector<string>> nposs;
			for (string s : getTrees(*np)) {
				for (vector<string> a : poss) {
					a.push_back(s);
					nposs.emplace_back(std::move(a));
				}
			}
			poss = std::move(nposs);
		}
		for (vector<string>& v : poss) {
			sort(v.begin(), v.end());
			string a = "(";
			for (const string& s : v) {
				a += s;
			}
			a += ')';
			res.emplace_back(std::move(a));
		}
	}
	sort(res.begin(), res.end());
	res.resize(unique(res.begin(), res.end()) - res.begin());
	return mem[p] = res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	if (fopen("equal.in", "r")) {
		freopen("equal.in", "r", stdin);
		freopen("equal.out", "w", stdout);
	}
	for (int l = 1; l <= 18; l++) {
		dfs(l, l-1, l-1, 0, 0);
		for (const auto& it : trees[l]) {
			polies[l].push_back(&(it.first));
			allTrees.insert(it);
		}
		cerr << l << ' ' << trees[l].size() << '\n';
	}
	while (true) {
		int N; cin >> N;
		if (N == 0) exit(0);
		int d; cin >> d;
		vector<int> p(d+1);
		for (int i = d; i >= 0; i--) cin >> p[i], N -= p[i];
		if (N != 0) cout << 0 << '\n';
		else {
			const auto& res = getTrees(p);
			cout << res.size() << '\n';
			for (const auto& s : res) { cout << s << '\n'; }
		}
	}

	return 0;
}
