#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 300;
int N;
int V[MAXN];

int A, B; // number of passages, length

unordered_map<ll, int> dist;
queue<ll> q;

ll canonize(int n, int m1, int m2) {
	while(n < N && !((m1 & 1) || (m2 & 1))) {
		if(V[n] == 1) {
			m1 += (1 << B);
		} else if(V[n] == 2) {
			m2 += (1 << B);
		}
		n++;
		m1 >>= 1;
		m2 >>= 1;
	}
	assert(!(m1 & m2));
	return (ll(n) << B | m1) << B | m2;
}

void insert(int n, int m1, int m2, int d) {
	assert(!(m1 & m2));
	ll s = canonize(n, m1, m2);
	if(dist.count(s)) return;
	dist[s] = d;
	q.push(s);
}

int suff[1 << 20][25];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> A >> B;
	for(int s = 0; s < 2; s++) {
		for(int i = 0; i < N; i++) {
			int v; cin >> v; V[i] += v;
		}
	}

	for(int m = 0; m < (1 << B); m++) {
		for(int i = 0, v = m; v; i++, v &= (v - 1)) {
			suff[m][i] = v;
		}
	}

	N += B;

	insert(0, 0, 0, 0);

	while(true) {
		ll v = q.front();
		int d = dist[v];

		int m2 = v & ((1 << B) - 1);
		v >>= B;
		int m1 = v & ((1 << B) - 1);
		v >>= B;
		int n = v;

		//cerr << n << ' ' << m1 << ' ' << m2 << ' ' << d << '\n';

		if(n == N) {
			break;
		}

		q.pop();

		d++;

		assert(!(m1 & m2));
		assert(1 & (m1 | m2));

		for(int i = 0; i <= A; i++) {
			int nm2 = suff[m2][A - i];
			int nm1 = suff[m1][i] | (m2 ^ nm2);
			insert(n, nm1, nm2, d);
		}
	}

	cout << dist[q.front()] << '\n';

	return 0;
}
