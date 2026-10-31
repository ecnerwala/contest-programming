#include <bits/stdc++.h>

int powmod(int a, int e, int m) {
	int r = 1;
	while (e) {
		if (e & 1) r = int(int64_t(r) * int64_t(a) % m);
		a = int(int64_t(a) * int64_t(a) % m);
		e >>= 1;
	}
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int P; cin >> P;
	int A, B; cin >> A >> B;

	vector<int> ord_facs;
	for (int v = P-1, q = 2; v > 1; q++) {
		if (v % q == 0) {
			ord_facs.push_back((P-1)/q);
			while (v % q == 0) v /= q;
		}
	}

	auto is_prim_root = [&](int g) -> bool {
		for (int o : ord_facs) if (powmod(g, o, P) == 1) return false;
		return true;
	};

	int g = 1; while (!is_prim_root(g)) g++;

	vector<int> powG(P);
	powG[0] = 1;
	for (int i = 1; i <= P-1; i++) powG[i] = int(int64_t(powG[i-1]) * int64_t(g) % P);

	A = int(find(powG.begin(), powG.end(), A) - powG.begin());
	B = int(find(powG.begin(), powG.end(), B) - powG.begin());

	int gA = std::gcd(A, P-1);
	int gB = std::gcd(B, P-1);

	if (std::gcd(gA, gB) != 1) {
		cout << "No" << '\n';
		exit(0);
	}

	if (gA > gB) {
		swap(A, B);
		swap(gA, gB);
	}

	vector<multiset<int>> adj(P-1);
	for (int i = 0; i < P-1; i++) {
		adj[i].insert((i + A) % (P-1));
		adj[i].insert((i - A + (P-1)) % (P-1));
	}

	for (int i = 0, cur = 0; i+1 < gA; i++, cur += (A + B), cur %= (P-1)) {
		int cur2 = (cur + A) % (P-1);
		int nxt = (cur + B) % (P-1);
		int nxt2 = (nxt + A) % (P-1);
		adj[cur].erase(adj[cur].find(cur2));
		adj[cur2].erase(adj[cur2].find(cur));
		adj[nxt].erase(adj[nxt].find(nxt2));
		adj[nxt2].erase(adj[nxt2].find(nxt));
		// B * i + A * i and B * i + A * (i+1)
		// B * (i+1) + A * i and B * (i+1) + A * (i+1)
		adj[cur].insert(nxt);
		adj[nxt].insert(cur);
		adj[cur2].insert(nxt2);
		adj[nxt2].insert(cur2);
	}

	vector<int> res; res.reserve(P-1);

	for (int cur = 0, prv = *adj[cur].begin(); int(res.size()) < P-1; ) {
		res.push_back(cur);

		prv = std::accumulate(adj[cur].begin(), adj[cur].end(), 0) - prv;
		std::swap(cur, prv);
	}

	cout << "Yes" << '\n';
	for (int i = 0; i < P-1; i++) {
		cout << powG[res[i]] << ' ';
	}
	cout << powG[res[0]] << '\n';

	return 0;
}

// say gcd(A, P-1) = gA; then, there are gA rings of length (P-1)/gA
//  +a +a +a +a...
//
// -   - - - -
//  | |
// -     - - -
//    | |
// - -     - -
//      | |
// - - -   - -
