#include<iostream>
#include<set>
#include<algorithm>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<pii, int> ppi;

#define A first
#define B second

const int MAXN = 1e5;

int N;
ppi E[MAXN]; // E is north & west of D
ppi D[MAXN];

ppi evts[MAXN + MAXN];

set<pii> stations; 

int main() {
	cin >> N;
	ll res = 0;
	for(int i = 0; i < N; i++) {
		cin >> E[i].A.A >> E[i].A.B;
		E[i].B = i + 1;
		res -= E[i].A.A;
		res += E[i].A.B;
	}
	for(int i = 0; i < N; i++) {
		cin >> D[i].A.A >> D[i].A.B;
		D[i].B = i + 1;
		res += D[i].A.A;
		res -= D[i].A.B;
	}
	cout << res << '\n';
	for(int i = 0; i < N; i++) {
		evts[i] = E[i];
	}
	for(int i = 0; i < N; i++) {
		evts[i + N] = D[i];
		evts[i + N].B *= -1;
	}
	sort(evts, evts + N + N);

	for(int i = 0; i < N + N; i++) {
		ppi cur = evts[i];
		if(cur.B < 0) {
			cur.B *= -1;
			set<pii>::iterator it = stations.lower_bound(pii(cur.A.B, 0));
			cout << it->B << ' ' << cur.B << '\n';
			stations.erase(it);
		} else {
			stations.insert(pii(cur.A.B, cur.B));
		}
	}
	return 0;
}
