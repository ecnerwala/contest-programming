#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int P;
ll E[20000];
ll F[20000];

ll tmp[20000];
bool is_good(ll v) {
	if(v == 0) {
		for(int i = 0; i < P; i++) {
			if(F[i] % 2 != 0) return false;
		}
		for(int i = 0; i < P; i++) {
			F[i] /= 2;
		}
		return true;
	} else {
		bool neg = (v < 0);
		v = abs(v);
		memcpy(tmp, F, sizeof(F));
		for(int i = 0, j = 0; i < P; i++) {
			if(!tmp[i]) continue;
			while(j < P && E[j] < E[i] + v) j++;
			if(j == P || E[j] > E[i] + v) return false;
			assert(E[j] == E[i] + v);
			if(tmp[j] < tmp[i]) return false;
			tmp[j] -= tmp[i];
		}

		int ind = 0;
		for(int i = 0; i < P; i++) {
			assert(ind <= i);
			if(!tmp[i]) continue;
			F[ind] = tmp[i];
			E[ind] = E[i];
			if(neg) E[ind] += v;
			ind++;
		}
		P = ind;

		return true;
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cin >> P;
		for(int i = 0; i < P; i++) {
			cin >> E[i];
		}
		for(int i = 0; i < P; i++) {
			cin >> F[i];
		}

		cout << "Case #" << t << ":";
		for(int i = 0; i < P; ) {
			ll v = E[i];
			if(is_good(v)) {
				cout << ' ' << v;
				i = lower_bound(E, E + P, v) - E;
			} else {
				i++;
			}
		}
		cout << '\n';
		assert(P == 1);
		assert(E[0] == 0);
		assert(F[0] == 1);
	}
		
	return 0;
}
