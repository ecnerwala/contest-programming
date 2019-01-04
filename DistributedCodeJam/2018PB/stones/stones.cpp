#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "stones.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split(ll a, ll i, ll n = NODES) {
	return (a / n) * i + min(a % n, i);
}

const ll N = GetNumberOfStones() + 1;
ll GetJumpLocation(ll location) {
	return min(N, location + GetJumpLength(location));
}

const ll ST = split(N, ID);
const ll EN = split(N, ID+1);

const int MAXSPLIT = int(1e7) + 20;

typedef pair<ll, ll> pll;
pll _dp[MAXSPLIT];
pll* dp = _dp - ST;

int main() {
	for (ll i = ST; i < EN; i++) {
		dp[i] = pll(GetJumpLocation(i), 1);
	}
	ll maxV = 0;
	for (ll i = ST; i < EN; i++) {
		maxV = max(maxV, dp[i].first);
	}
	{
		ll prevMax = 0;
		if (ID-1 >= 0) {
			Receive(ID-1);
			prevMax = GetLL(ID-1);
			maxV = max(maxV, prevMax);
		}
		if (ID+1 < NODES) {
			PutLL(ID+1, maxV);
			Send(ID+1);
		}
		maxV = prevMax;
	}

	for (ll i = ST; i < EN; i++) {
		dp[i].first = maxV = max(maxV, dp[i].first);
	}
	for (ll i = EN-1; i >= ST; i--) {
		if (dp[i].first < EN) {
			ll n = dp[i].first;
			dp[i] = pll(dp[n].first, dp[i].second + dp[n].second);
		}
	}
	pll st(0, 0);
	if (ID-1 >= 0) {
		Receive(ID-1);
		st.first = GetLL(ID-1);
		st.second = GetLL(ID-1);
	}
	assert(st.first >= ST);
	if (st.first < EN) {
		ll n = st.first;
		st = pll(dp[n].first, st.second + dp[n].second);
	}
	assert(st.first >= EN);
	if (ID+1 < NODES) {
		PutLL(ID+1, st.first);
		PutLL(ID+1, st.second);
		Send(ID+1);
	} else {
		cout << st.second << '\n';
	}

	return 0;
}
