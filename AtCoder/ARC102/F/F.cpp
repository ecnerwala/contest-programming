#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4e5;
int N;
int P[MAXN];

bool go() {
	//cerr << "Go\n";
	for (int i = 0; i < N; i++) {
		if (P[i] % 2 != i % 2) {
			return false;
		}
	}
	int prefmax = -1;
	int st = -1;
	for (int i = 0; i < N; i++) {
		prefmax = max(prefmax, P[i]);
		if (prefmax == i) {
			int en = i;
			//cerr << st << ' ' << en << '\n';
			if (en - st == 1) {
				// we're fine
			} else {
				bool good = true;
				for (int j = en - 1; j > st; j-=2) {
					if (P[j] != j) {
						good = false;
					}
				}
				if (!good) {
					return false;
				}
				assert((en - st) % 2 == 1);
				assert(P[st+1] != st+1);
				int m1 = -1, m2 = -1;
				for (int j = st + 1; j <= en; j+=2) {
					if (P[j] < m2) {
						return false;
					} else if (P[j] < m1) {
						m2 = P[j];
					} else {
						m1 = P[j];
					}
				}
			}
			st = en;
		}
	}

	return true;
}

void test() {
	using perm = vector<int>;
	vector<perm> allPerms;
	set<perm> allPermsSet;
	map<perm, int> inv;
	map<perm, int> indegs;
	map<perm, vector<perm>> adj;
	for (int a = 0; a < 8; a+=2) {
		for (int b = 1; b < 8; b+=2) {
			for (int c = 0; c < 8; c+=2) {
				if (c == a) continue;
				for (int d = 1; d < 8; d+=2) {
					if (d == b) continue;
					for (int e = 0; e < 8; e+=2) {
						if (e == a || e == c) continue;
						for (int f = 1; f < 8; f+=2) {
							if (f == b || f == d) continue;
							for (int g = 0; g < 8; g+=2) {
								if (g == a || g == c || g == e) continue;
								for (int h = 1; h < 8; h+=2) {
									if (h == b || h == d || h == f) continue;
							perm cur{a,b,c,d,e,f,g,h};
							int nInv = (a > b) + (a > c) + (a > d) + (a > e) + (a > f) + (b > c) + (b > d) + (b > e) + (b > f);
							nInv += (c > d) + (c > e) + (c > f) + (d > e) + (d > f) + (e > f);
							nInv += (a > g) + (b > g) + (c > g) + (d > g) + (e > g) + (f > g);
							nInv += (a > h) + (b > h) + (c > h) + (d > h) + (e > h) + (f > h) + (g > h);
							allPerms.push_back(cur);
							allPermsSet.insert(cur);
							inv[cur] = nInv;
							int indeg = 0;
							if (a > b && b > c) {
								perm nxt{c,b,a,d,e,f,g,h};
								adj[nxt].push_back(cur);
								indeg ++;
							}
							if (b > c && c > d) {
								perm nxt{a,d,c,b,e,f,g,h};
								adj[nxt].push_back(cur);
								indeg ++;
							}
							if (c > d && d > e) {
								perm nxt{a,b,e,d,c,f,g,h};
								adj[nxt].push_back(cur);
								indeg ++;
							}
							if (d > e && e > f) {
								perm nxt{a,b,c,f,e,d,g,h};
								adj[nxt].push_back(cur);
								indeg ++;
							}
							if (e > f && f > g) {
								perm nxt{a,b,c,d,g,f,e,h};
								adj[nxt].push_back(cur);
								indeg ++;
							}
							if (f > g && g > h) {
								perm nxt{a,b,c,d,e,h,g,f};
								adj[nxt].push_back(cur);
								indeg ++;
							}
							indegs[cur] = indeg;
								}
							}
						}
					}
				}
			}
		}
	}
	set<perm> seen;
	perm st{0,1,2,3,4,5,6,7};
	vector<perm> q;
	q.push_back(st);
	seen.insert(st);
	for (size_t i = 0; i < q.size(); i++) {
		const perm& cur = q[i];
		assert(allPermsSet.count(cur));
		for (int j : cur) { cerr << j; } cerr << ' ' << inv[cur] << '\n';
		for (const perm& nxt : adj[cur]) {
			if (seen.count(nxt)) continue;
			seen.insert(nxt);
			q.push_back(nxt);
		}
	}
	cerr << "BADS" << '\n';
	for (const perm& cur : allPerms) {
		if (seen.count(cur)) continue;
		if (inv[cur] % 3 != 0) continue;
		for (int j : cur) { cerr << j; } cerr << ' ' << inv[cur] <<  ' ' << indegs[cur] << '\n';
	}
	for (const perm& cur : allPerms) {
		N = int(cur.size());
		for (int i = 0; i < N; i++) {
			P[i] = cur[i];
		}
		bool res = go();
		if (res != seen.count(cur)) {
			cerr << res << ' ' << seen.count(cur) << '\n';
			for (int j : cur) { cerr << j; } cerr << ' ' << inv[cur] <<  ' ' << indegs[cur] << '\n';
		}
	}
	exit(0);
}

int main() {
	ios_base::sync_with_stdio(0);
	test();
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> P[i];
		P[i] --;
	}
	bool res = go();
	cout << (res ? "Yes" : "No") << '\n';

	return 0;
}
