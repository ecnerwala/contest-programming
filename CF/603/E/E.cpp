#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1.2e5;
const int MAXM = 5e5;

int N, M;
pii E[MAXM];
int L[MAXM];

const int B = 400;
const int S = 300;

struct dsds {
	int par[MAXN];
	bool size[MAXN];
	int rank[MAXN];
	int odd;

	int getpar(int a) {
		return (par[a] == a ? a : (par[a] = getpar(par[a])));
	}

	bool merge(int a, int b) {
		a = getpar(a), b = getpar(b);
		if(a == b) return false;
		if(rank[a] > rank[b]) swap(a, b);
		else if(rank[a] == rank[b]) rank[b] ++;
		par[a] = b;
		if(size[a] && size[b]) odd--;
		size[b] ^= size[a];
		return true;
	}

	void reset() {
		for(int i = 0; i < N; i++) par[i] = i, size[i] = 1, rank[i] = 0;
		odd = N / 2;
	}

} buckets[B + 20];

vector<pair<int, pii>> edges[B + 20];

int main() {

	vector<int> lens;

	cin >> N >> M;

	if(N % 2) {
		for(int i = 0; i < M; i++) cout << -1 << '\n';
		return 0;
	}

	for(int i = 0; i < M; i++) {
		cin >> E[i].A >> E[i].B >> L[i];
		E[i].A --, E[i].B --;
		lens.push_back(L[i]);
	}

	sort(lens.begin(), lens.end());
	lens.resize(unique(lens.begin(), lens.end()) - lens.begin());

	for(int i = 0; i < M; i++) {
		L[i] = int(lower_bound(lens.begin(), lens.end(), L[i]) - lens.begin());
	}

	for(int i = 0; i <= B; i++) {
		buckets[i].reset();
	}

	for(int i = 0; i < M; i++) {
		assert(L[i] / S < B);
		edges[L[i] / S].emplace_back(L[i], E[i]);
		for(int b = L[i] / S + 1; b <= B; b++) {
			buckets[b].merge(E[i].A, E[i].B);
		}
		if(buckets[B].odd) {
			cout << -1 << '\n';
			continue;
		}
		int b = B;
		while(!buckets[b].odd) b--;
		assert(b >= 0);
		dsds s = buckets[b];
		sort(edges[b].begin(), edges[b].end());
		int res = 0;
		assert(s.odd);
		for(auto it : edges[b]) {
			res = it.A;
			s.merge(it.B.A, it.B.B);
			if(!s.odd) break;
		}
		cout << lens[res] << '\n';
	}

	return 0;
}
