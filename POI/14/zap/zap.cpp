#include<iostream>
#include<cassert>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

const int MAXV = 1e5;

const int V = int(5e4);
int mobius[MAXV];
int mobsum[MAXV];
int sieve[MAXV];

int ansV[MAXV];

ll query(int a, int b) {
	if (!a || !b) return 0;
	if(a > b) swap(a, b);
	assert(a <= b);
	if (b == V) return ansV[a];

	//for(int i = 0; i < Na; i++) cerr << endpts[i] << ' '; cerr << '\n';
	//for(int i = Na; i < Nb; i++) cerr << endpts[i] << ' '; cerr << '\n';

	ll res = 0;
	int cur = 0;
	while (cur < a && cur * cur <= b) {
		int v = cur+1;
		res += ll(a / v) * (b / v) * mobius[v];
		cur = v;
	}
	if (cur == a) return res;

	int da = a / (cur+1);
	int db = b / (cur+1);
	int va = a / da;
	int vb = b / db;
	while (true) {
		int v = min(va, vb);
		res += ll(da) * ll(db) * ll(mobsum[v] - mobsum[cur]);
		cur = v;
		if (v == va) {
			da--;
			if (!da) break;
			va = a / da;
		}
		if (v == vb) {
			db--;
			vb = b / db;
		}
	}

	return res;
}

ll query(int a, int b, int d) {
	return query(a / d, b / d);
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
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
	for (int i = 1; i <= V; i++) {
		for (int j = i; j <= V; j += i) {
			ansV[j] += (V/i) * mobius[i];
		}
	}
	for (int i = 1; i <= V; i++) {
		ansV[i] += ansV[i-1];
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
