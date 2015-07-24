#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pii;
typedef pair<pii, ll> ppi;
#define A first
#define B second

inline ll sq(ll a) { return a * a; }

int main() {
	ios_base::sync_with_stdio(0);

	int N; cin >> N;
	vector<pii> P(N);
	for(int i = 0; i < N; i++) {
		cin >> P[i].A >> P[i].B;
	}

	multiset<ppi> mp;

	ll res = 0;
	for(int i = 0; i < N; i++) {
		for(int j = i+1; j < N; j++) {
			ppi v(pii(P[i].A + P[j].A, P[i].B + P[j].B), sq(P[i].A - P[j].A) + sq(P[i].B - P[j].B));
			res += mp.count(v);
			mp.insert(v);
		}
	}

	cout << res << '\n';

	return 0;
}
