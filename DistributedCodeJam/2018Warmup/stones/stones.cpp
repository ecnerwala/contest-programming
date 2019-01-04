#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "stones.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll split (ll a, ll i, ll n) {
	return (a / n) * i + min(a % n, i);
}

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll L = GetNumberOfStones()+1;

ll GetNext(ll i) {
	assert(0 <= i && i < L);
	return min(L, i + GetJumpLength(i));
}

const ll MAXSPLIT=1.1e7;
pair<ll, ll> dp[MAXSPLIT];

int main() {
	const ll ST = split(L, ID, NODES);
	const ll EN = split(L, ID+1, NODES);

	ll stMax = 0;
	ll enMax = 0;
	{
		for (ll i = ST; i < EN; i++) {
			enMax = max(enMax, GetNext(i));
		}
		if (ID > 0) {
			Receive(ID-1);
			stMax = GetLL(ID-1);
		} else {
			stMax = 0;
		}
		enMax = max(enMax, stMax);
		if (ID+1 < NODES) {
			PutLL(ID+1, enMax);
			Send(ID+1);
		}
	}
	{
		ll curMax = stMax;
		for (ll i = ST; i < EN; i++) {
			curMax = max(curMax, GetNext(i));
			dp[i-ST].first = curMax;
			dp[i-ST].second = 1;
		}
	}

	{
		for (ll i = EN-1; i >= ST; i--) {
			if (dp[i-ST].first < EN) {
				ll n = dp[i-ST].first;
				dp[i-ST].first = dp[n-ST].first;
				dp[i-ST].second += dp[n-ST].second;
			}
		}
	}

	pair<ll, ll> res(0,0);
	if (ID > 0) {
		Receive(ID-1);
		res.first = GetLL(ID-1);
		res.second = GetLL(ID-1);
	}
	if (res.first < EN) {
		ll n = res.first;
		res.first = dp[n-ST].first;
		res.second += dp[n-ST].second;
	}
	assert(res.first >= EN);

	if (ID+1 < NODES) {
		PutLL(ID+1, res.first);
		PutLL(ID+1, res.second);
		Send(ID+1);
	} else {
		assert(res.first == L);
		cout << res.second << '\n';
	}

	return 0;
}
