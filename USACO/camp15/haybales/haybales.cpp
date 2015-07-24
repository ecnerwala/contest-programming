#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	int N; cin >> N;

	map<ll, ll> dslope;
	ll yint = 0;

	for(int i = 0; i < N; i++) {
		ll v; cin >> v;

		dslope[v] += 2;

		yint += v;

		map<ll, ll>::iterator it = dslope.begin();
		yint -= it->first;
		if(it->second == 1) {
			dslope.erase(it);
		} else {
			it->second --;
		}
	}

	cout << yint << '\n';
	return 0;
}
