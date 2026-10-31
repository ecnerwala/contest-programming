#include<bits/stdc++.h>
using namespace std;

const int MAXV = 1.1e5;

int pfac[MAXV];
vector<int> primes;

void precomp() {
	primes.reserve(MAXV);
	for (int v = 2; v < MAXV; v++) {
		if (pfac[v] == 0) {
			pfac[v] = v;
			primes.push_back(v);
		}
		for (int j = 0; j < int(primes.size()) && primes[j] <= pfac[v] && v * primes[j] < MAXV; j++) {
			pfac[v * primes[j]] = primes[j];
		}
	}
}

const int MAXN = 1.1e5;
int N, K;

const int S = 1 << 17;
struct seg_node {
	int msk;
	int cnt;
} seg[2*S];

int getAns() {
	return seg[1].cnt;
}

void updateNode(int i) {
	assert(seg[i].msk >= 0);
	if (seg[i].msk) {
		seg[i].cnt = 1<<(__builtin_clz(i) - __builtin_clz(S));
	} else {
		if (i >= S) {
			seg[i].cnt = 0;
		} else {
			seg[i].cnt = seg[2*i].cnt + seg[2*i+1].cnt;
		}
	}
}

void updateBad(int l, int r, int d) {
	///cerr << "updateBad " << l << ' ' << r << ' ' << d << '\n';
	assert(l < r);
	if (r - l >= K) return;
	int lo = max(0, r - K + 1);
	int hi = min(l, N-K);
	assert(lo <= hi);

	for (int a = S+lo, b = S+hi+1; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			seg[a].msk += d;
			updateNode(a);
			a++;
		}
		if (b & 1) {
			--b;
			seg[b].msk += d;
			updateNode(b);
		}
	}
	for (int a = S+lo; a; a /= 2) updateNode(a);
	for (int b = S+hi; b; b /= 2) updateNode(b);
}

set<int> alive[MAXV];
void changePrime(int i, int p, bool ins) {
	//cerr << "changePrime " << i << ' ' << p << ' ' << ins << '\n';
	set<int>::iterator it = alive[p].lower_bound(i);
	if (!ins) {
		it = alive[p].erase(it);
	}

	int dir = ins ? 1 : -1;

	int nxt = -1;
	if (it != alive[p].end()) {
		nxt = *it;
		updateBad(i, nxt, dir);
	}
	int prv = -1;
	if (it != alive[p].begin()) {
		--it;
		prv = *it;
		updateBad(prv, i, dir);
	}

	if (nxt != -1 && prv != -1) {
		updateBad(prv, nxt, -dir);
	}

	if (ins) {
		alive[p].insert(i);
	}
}

void change(int i, int v, bool ins) {
	//cerr << "change " << i << ' ' << v << ' ' << ins << '\n';
	while (v != 1) {
		int p = pfac[v];
		assert(v % p == 0);
		while (v % p == 0) v /= p;

		changePrime(i, p, ins);
	}
}

int V[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();
	int Q;
	cin >> N >> K >> Q;
	for (int i = 0; i < N; i++) {
		cin >> V[i];
	}

	for (int i = 0; i < N; i++) {
		change(i, V[i], true);
	}
	cout << getAns() << '\n';
	while (Q--) {
		int i, v; cin >> i >> v; i--;
		change(i, V[i], false);
		V[i] = v;
		change(i, V[i], true);
		cout << getAns() << '\n';
	}

	long long sum = 0;
	for (int i = 0; i < N; i++) sum += V[i];
	cout << sum << '\n';

	return 0;
}
