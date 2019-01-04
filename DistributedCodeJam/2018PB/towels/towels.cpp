#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "towels.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split(ll a, ll i, ll n = NODES) {
	assert(0 <= i && i <= n);
	return (a / n) * i + min(a % n, i);
}

const ll P = GetNumberOfStacks();
const ll K = GetNumberOfTowelsInStack();
const ll S = GetNumberOfGymUsersBeforeYou();

const ll ST = split(P, ID);
const ll EN = split(P, ID+1);
const ll N = (EN - ST) * K;
const ll MAXN = 3e7;
ll vals[MAXN];

void get_vals() {
	assert(N < MAXN);
	ll n = 0;
	for (ll i = ST; i < EN; i++) {
		for (ll j = 0, v = 0; j < K; j++) {
			v = max(v, GetTowelCleanlinessRank(i, K-1-j));
			vals[n++] = v;
		}
	}
	assert(n == N);
	sort(vals, vals + N);
}

constexpr ll B = 1000;
ll mds[B+20];
ll cnts[B+20];

void go() {
	get_vals();
	ll cmi = 0;
	ll mi = 0;
	ll ma = P*K+1;
	while (ma - mi > 1) {
		for (int i = 1; i < B; i++) {
			ll md = split(ma-mi, i, B) + mi;
			mds[i] = md;
			cnts[i] = lower_bound(vals, vals + N, md) - vals;
		}
		for (int z = 1; z < NODES; z++) {
			int n = (ID + z) % NODES;
			for (int i = 1; i < B; i++) {
				PutLL(n, cnts[i]);
			}
			Send(n);
		}
		for (int z = 1; z < NODES; z++) {
			int n = (ID + NODES - z) % NODES;
			Receive(n);
			for (int i = 1; i < B; i++) {
				cnts[i] += GetLL(n);
			}
		}
		for (ll i = 1; i < B; i++) {
			if (cnts[i] > S) {
				ma = mds[i];
				break;
			} else {
				mi = mds[i];
				cmi = cnts[i];
			}
		}
	}
	// mi is the right point
	assert(cmi <= S);
	for (ll i = ST; i < EN; i++) {
		for (ll j = 0; j < K; j++) {
			if (GetTowelCleanlinessRank(i, K-1-j) == mi) {
				ll nj = (S - cmi) + j;
				assert(0 <= nj);
				assert(nj < K);
				cout << GetTowelCleanlinessRank(i, K-1-nj) << '\n';
				return;
			}
		}
	}
}

int main() {
	go();

	return 0;
}
