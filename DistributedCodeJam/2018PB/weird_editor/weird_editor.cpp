#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "weird_editor.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9) + 7;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split(ll a, ll i, ll n = NODES) {
	return (a / n) * i + min(a % n, i);
}

const ll L = GetNumberLength();

const ll st = split(L, ID);
const ll en = split(L, ID + 1);

void phase1() {
	ll maxCur = 0;
	for (ll i = en-1; i >= st; i--) {
		maxCur = max(maxCur, GetDigit(i));
	}
	ll maxRight = 0;
	if (ID+1 < NODES) {
		Receive(ID+1);
		maxRight = GetLL(ID+1);
		maxCur = max(maxCur, maxRight);
	}
	if (ID-1 >= 0) {
		PutLL(ID-1, maxCur);
		Send(ID-1);
	}

	maxCur = maxRight;
	ll val = 0;
	ll fac = 1;
	ll base = 1;
	for (ll i = en-1; i >= st; i--) {
		ll d = GetDigit(i);
		if (d >= maxCur) {
			val += d * base;
			val %= MOD;
			base *= 10;
			base %= MOD;
			maxCur = max(maxCur, d);
		} else {
			fac *= 10;
			fac %= MOD;
		}
	}
	if (ID + 1 < NODES) {
		Receive(ID + 1);
		ll rVal = GetLL(ID+1);
		ll rBase = GetLL(ID+1);
		ll rFac = GetLL(ID+1);
		val = (val * rBase + rVal) % MOD;
		base *= rBase;
		base %= MOD;
		fac *= rFac;
		fac %= MOD;
	}

	if (ID - 1 >= 0) {
		PutLL(ID-1, val);
		PutLL(ID-1, base);
		PutLL(ID-1, fac);
		Send(ID-1);
	} else {
		val *= fac;
		val %= MOD;
		cout << val << '\n';
	}
}

int main() {
	phase1();

	return 0;
}
