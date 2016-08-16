#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
vector<int> adj[MAXN];
int par[MAXN];

void dfs(int cur, int prv = -1) {
	par[cur] = prv;
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		dfs(nxt, cur);
	}
}

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	int n = atoi(argv[1]);
    int t = atoi(argv[2]);
	int p_no_bridge = atoi(argv[3]); // percent
	int p_wrong = atoi(argv[4]); // percent

    vector<int> p(n);
	for(int i = 0; i < n; i++) {
		if(i > 0) p[i] = rnd.wnext(i, t);
	}

    vector<int> perm(n);
	for(int i = 0; i < n; i++) perm[i] = i;
    shuffle(perm.begin() + 1, perm.end());
    vector<pair<int,int> > edges;

    for (int i = 1; i < n; i++) {
        if (rnd.next(2)) {
            edges.push_back(make_pair(perm[i], perm[p[i]]));
		} else {
            edges.push_back(make_pair(perm[p[i]], perm[i]));
		}
	}

    shuffle(edges.begin(), edges.end());

    printf("%d\n", n);

    vector<int> A(n);
	for(int i = 0; i < n; i++) A[i] = i;
	shuffle(A.begin(), A.end());
	for(int i = 0; i < n; i++) {
		if(i) printf(" ");
		printf("%d", A[i]);
	}
	printf("\n");

	for(pair<int, int> e : edges) {
		adj[e.first].push_back(e.second);
		adj[e.second].push_back(e.first);
	}
	int st = 0; while(A[st]) st++;
	dfs(st);

	if(rnd.next(1, 100) < p_no_bridge) {

	} else {
		int v1 = rnd.next(0, n - 1);
		vector<int> p1;
		int v2 = rnd.next(0, n - 1);
		vector<int> p2;
		for(int c = v1; c != st; c = par[c]) p1.push_back(c);
		for(int c = v2; c != st; c = par[c]) p2.push_back(c);
		while(!p1.empty() && !p2.empty() && p1.back() == p2.back()) {
			p1.pop_back();
			p2.pop_back();
		}
		if(!(p1.empty() && p2.empty())) {
			int sz = int(p1.size() + p2.size());
			reverse(p1.begin(), p1.end());
			vector<int> cyc;
			for(int i : p1) cyc.push_back(i);
			for(int i : p2) cyc.push_back(i);
			vector<int> vals;
			for(int i : cyc) vals.push_back(A[i]);
			int off = rnd.next(0, sz - 1);
			for(int i = 0; i < sz; i++) {
				A[cyc[i]] = vals[(i + off) % sz];
			}
		}
	}

	int en = rnd.next(0, n - 1);
	for(int c = en, v = 0; true; c = par[c]) {
		swap(A[c], v);
		if(c == st) break;
	}

	if(rnd.next(1, 100) < p_wrong) {
		int a = rnd.next(0, n - 1);
		int b = rnd.next(0, n - 1);
		swap(A[a], A[b]);
	}

	for(int i = 0; i < n; i++) {
		if(i) printf(" ");
		printf("%d", A[i]);
	}
	printf("\n");

    for (int i = 0; i + 1 < n; i++) {
        printf("%d %d\n", edges[i].first + 1, edges[i].second + 1);
	}

    return 0;
}
