#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "todd_and_steven.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9) + 7;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split(ll a, ll i, ll n = NODES) {
	assert(0 <= i && i <= n);
	return (a / n) * i + min(a % n, i);
}

const ll N[2] = {GetStevenLength()+1, GetToddLength()+1};

ll INF = ll(1e18);

inline ll GetValue(int z, ll i) {
	assert(z == 0 || z == 1);
	assert(0 <= i && i <= N[z]);
	if (i == 0) return -INF + z;
	if (i == N[z]) return INF + z;
	else if (z == 0) return GetStevenValue(i-1);
	else if (z == 1) return GetToddValue(i-1);
	else assert(false);
}

ll solveRange(ll a, ll b) {
	ll res = 0;

	ll s0 = split(N[0], a), e0 = split(N[0], a+1);
	ll s1 = split(N[1], b), e1 = split(N[1], b+1);
	ll lo = max(GetValue(0, s0), GetValue(1, s1));
	ll hi = min(GetValue(0, e0), GetValue(1, e1));
	for (ll i0 = s0, i1 = s1; i0 < e0 || i1 < e1; ) {
		ll v0 = GetValue(0, i0);
		ll v1 = GetValue(1, i1);
		assert(v0 != v1);
		ll v = min(v0, v1);
		if (v >= hi) break;
		if (v >= lo) {
			ll i = i0 + i1 - 2;
			if (i >= 0) {
				res += (v ^ i);
				res %= MOD;
			}
		}

		if (v0 < v1) {
			i0++;
		} else {
			i1++;
		}
	}

	return res;
}

enum class Opcode {
	KILL = 0,
	RANGE_SOLVE
};

const int MASTER = 0;

void worker() {
	assert(ID != MASTER);
	while (true) {
		Receive(MASTER);
		Opcode op = Opcode(GetLL(MASTER));
		switch(op) {
			case Opcode::KILL:
				return;
			case Opcode::RANGE_SOLVE:
				{
					ll i = GetLL(MASTER);
					ll j = GetLL(MASTER);
					ll res = solveRange(i, j);
					PutLL(MASTER, res);
					Send(MASTER);
				} break;
			default: assert(false);
		}
	}
}

void master() {
	assert(ID == MASTER);
	queue<int> nodes;
	for (int i = 0; i < NODES; i++) {
		if (i == ID) continue;
		nodes.push(i);
	}
	ll res = 0;
	for (int i = 0; i < NODES; i++) {
		for (int j = 0; j < NODES; j++) {
			ll s0 = GetValue(0, split(N[0], i)), e0 = GetValue(0, split(N[0], i+1));
			ll s1 = GetValue(1, split(N[1], j)), e1 = GetValue(1, split(N[1], j+1));
			assert(s0 % 2 == 0 && e0 % 2 == 0);
			assert(s1 % 2 != 0 && e1 % 2 != 0);
			if (e0 < s1) continue;
			if (e1 < s0) continue;
			if (nodes.empty()) {
				int n = Receive(-1);
				res += GetLL(n);
				res %= MOD;
				nodes.push(n);
			}
			int n = nodes.front();
			nodes.pop();
			PutLL(n, ll(Opcode::RANGE_SOLVE));
			PutLL(n, i);
			PutLL(n, j);
			Send(n);
		}
	}
	while (int(nodes.size()) < NODES-1) {
		int n = Receive(-1);
		res += GetLL(n);
		res %= MOD;
		nodes.push(n);
	}

	while (!nodes.empty()) {
		int n = nodes.front(); nodes.pop();
		PutLL(n, ll(Opcode::KILL));
		Send(n);
	}

	cout << res << '\n';
}


int main() {
	if (ID == MASTER) {
		master();
	} else {
		worker();
	}
	return 0;
}
