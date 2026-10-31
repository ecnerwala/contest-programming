#include<bits/stdc++.h>
using namespace std;

const int MAXF = 1.1e5;
int F;
int st[MAXF];
int en[MAXF];
list<int> A[MAXF];

const int MAXV = 3.1e5;
int V;
int par[MAXV];
int realPar[MAXV];

int getPar(int a) {
	return par[a] == -1 ? a : (par[a] = getPar(par[a]));
}
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	par[b] = a;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> F;
	for (int i = 0; i < F; i++) {
		int a; cin >> a;
		st[i] = V;
		A[i] = list<int>(a);
		iota(A[i].begin(), A[i].end(), V);
		V += a;
		en[i] = V;
	}
	memset(par, -1, sizeof(par));
	sort(A, A + F, [&](const list<int>& a, const list<int>& b) { return a.size() < b.size(); });
	for (int i = F-2; i >= 0; i--) {
		if (A[i+1].size() > A[i].size()) {
			swap(A[i+1], A[i]);
		}

		assert(A[i].size() >= A[i+1].size());
		int tgt = (i ? int(A[i-1].size()) : 3);
		if (tgt % 2 != (int(A[i].size()) + int(A[i+1].size())) % 2) {
			tgt++;
		}
		tgt = max(tgt, int(A[i].size()) - int(A[i+1].size()) + 2);
		assert(tgt < int(A[i].size()) + int(A[i+1].size()));
		// how many edges to merge
		int numMerge = (int(A[i].size()) + int(A[i+1].size()) - tgt) / 2;
		auto it = A[i+1].begin();
		for (int z = 0; z < numMerge; z++) {
			merge(A[i].back(), *it);
			A[i].pop_back();
			++it;
		}
		--it;
		A[i].insert(A[i].end(), it, A[i+1].end());
		merge(*A[i].begin(), *A[i].rbegin());
		A[i].erase(A[i].begin());
		assert(int(A[i].size()) == tgt);
	}

	int N = 0;
	for (int i = 0; i < V; i++) {
		if (par[i] == -1) {
			realPar[i] = ++N;
		}
	}
	cout << N << '\n';
	for (int i = 0; i < F; i++) {
		for (int v = st[i]; v < en[i]; v++) {
			cout << realPar[getPar(v)] << ' ';
		}
		cout << '\n';
	}

	return 0;
}
