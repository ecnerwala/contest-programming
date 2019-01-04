#include<iostream>
#include<cassert>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

const int MAXV = 1e5;

const int V = 5e4;
int mobius[MAXV];
int mobsum[MAXV];
int sieve[MAXV];

int mobs(int l, int r) {
	return mobsum[r] - mobsum[l - 1];
}

int S;
int endpts[MAXV * 2]; // stores superset of right endpts

void findEndPts(int v) {
	int i = 1;
	for(; i * i <= v; i++) {
		endpts[S++] = i;
	}
	i--;
	for(; i > 0; i--) {
		endpts[S++] = v / i;
	}
}

ll query(int a, int b) {
	if(a > b) swap(a, b);
	S = 0;
	findEndPts(a);
	int Na = S;
	findEndPts(b);
	int Nb = S;

	//for(int i = 0; i < Na; i++) cerr << endpts[i] << ' '; cerr << '\n';
	//for(int i = Na; i < Nb; i++) cerr << endpts[i] << ' '; cerr << '\n';

	ll res = 0;
	int cur = 0;
	for(int i = 0, j = Na; i < Na && j < Nb; ) {
		int v;
		if(endpts[i] < endpts[j]) v = endpts[i++];
		else v = endpts[j++];

		//cerr << cur << ' ' << v << '\n';

		res += ll(a / v) * (b / v) * (mobsum[v] - mobsum[cur]);
		cur = v;
	}

	return res;
}

ll query(int a, int b, int d) {
	return query(a / d, b / d);
}

int main() {
	for(int i = 1; i <= V; i++) sieve[i] = i, mobius[i] = 1;
	for(int i = 2; i <= V; i++) {
		if(sieve[i] != 1) {
			//i is prime
			for(int j = i; j <= V; j += i) {
				sieve[j] /= i;
				if(sieve[j] % i == 0) mobius[j] = 0;
				else mobius[j] *= -1;
				while(sieve[j] % i == 0) {
					sieve[j] /= i;
				}
			}
		}
	}
	mobsum[0] = 0;
	for(int i = 1; i <= V; i++) {
		mobsum[i] = mobsum[i - 1] + mobius[i];
	}
	
	int N; cin >> N;
	for(int i = 0; i < N; i++) {
		int a, b, d; cin >> a >> b >> d;
		cout << query(a, b, d) << '\n';
	}
}
