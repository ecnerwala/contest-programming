#include "message.h"
#include "load_balance.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll N = GetN();

ll W[60];

ll T;

ll B;
ll L;
ll R;

bool is_good() {
	for(ll i = 0; i < B; i++) {
		if(ID & (1ll << i)) {
			T -= W[L + R + i];
		}
	}
	if(T < 0) return false;
	if(T == 0) return true;

	unordered_set<ll> vals(1ll << (L + 1));
	for(ll m = 0; m < (1ll << L); m++) {
		ll v = T;
		for(ll i = 0; i < L; i++) {
			if(m & (1ll << i)) v -= W[R + i];
		}
		vals.insert(v);
	}

	for(ll m = 0; m < (1ll << R); m++) {
		ll v = 0;
		for(ll i = 0; i < R; i++) {
			if(m & (1ll << i)) v += W[i];
		}
		if(vals.count(v)) return true;
	}

	return false;
}

int main() {
	T = 0; // Target
	for(ll i = 0; i < N; i++) {
		W[i] = GetWeight(i);
		T += W[i];
	}

	if(T % 2 != 0) {
		if(ID == 0) {
			printf("IMPOSSIBLE\n");
		}
		return 0;
	}

	assert(T % 2 == 0);
	T /= 2;

	B = 0;
	while((1ll << B) <= NODES) {
		B ++;
	}

	B--;
	assert(B >= 0);

	assert((1ll << B) <= NODES);

	if(ID >= (1ll << B)) {
		return 0;
	}

	L = (N - B) / 2;
	R = N - L - B;

	bool good = is_good();

	if(ID) {
		PutChar(0, good);
		Send(0);
	} else {
		if(good) {
			printf("POSSIBLE\n");
			return 0;
		}
		for(int i = 1; i < (1ll << B); i++) {
			Receive(i);
			if(GetChar(i)) {
				printf("POSSIBLE\n");
				return 0;
			}
		}

		printf("IMPOSSIBLE\n");
		return 0;
	}

	return 0;
}
