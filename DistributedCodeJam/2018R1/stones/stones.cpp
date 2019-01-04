#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "stones.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXNODES = 200;
const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll S = GetNumberOfStones() + 1;

ll GetDest(ll loc) {
	return min(S, loc + GetJumpLength(loc));
}

ll maxdest[MAXNODES];
ll minpt[MAXNODES];

const int MAXSPLIT = 1.1e7;
bool maximal[MAXSPLIT];
pair<ll, ll> res[MAXSPLIT];

ll split(ll a, ll i) {
	return (a / NODES) * i + min(a % NODES, i);
}


void go() {
	ll st = split(S, ID);
	ll en = split(S, ID+1);
	assert(en - st < MAXSPLIT);

	{ // phase 1 compute/send
		ll best = -1;
		for (ll i = st; i < en; i++) {
			best = max(best, GetDest(i));
		}
		for (int i = 0; i < NODES; i++) {
			PutLL(i, best);
			Send(i);
		}
	}
	{ // phase 1 receive
		maxdest[0] = -1;
		for (int i = 0; i < NODES; i++) {
			Receive(i);
			maxdest[i+1] = GetLL(i);
		}
		for (int i = 0; i < NODES; i++) {
			maxdest[i+1] = max(maxdest[i+1], maxdest[i]);
		}
	}

	{ // phase 2 compute
		ll best = maxdest[ID];
		for (ll i = st; i < en; i++) {
			ll v = GetDest(i);
			if (v > best) {
				best = v;
				maximal[i-st] = true;
			} else {
				maximal[i-st] = false;
			}
		}
		assert(best == maxdest[ID+1]);
		ll first = S;
		for (ll i = st; i < en; i++) {
			if (maximal[i-st]) {
				first = i;
				break;
			}
		}
		for (int i = 0; i < NODES; i++) {
			PutLL(i, first);
			Send(i);
		}
	}

	{ // phase 2 receive
		for (int i = 0; i < NODES; i++) {
			Receive(i);
			minpt[i] = GetLL(i);
		}
		minpt[NODES] = S;
		for (int i = NODES-1; i >= 0; i--) {
			minpt[i] = min(minpt[i], minpt[i+1]);
		}
	}

	{ // phase 3: compute all things
		for (ll i = en-1; i >= st; i--) {
			if (!maximal[i-st]) continue;
			ll v = GetDest(i);
			if (v >= minpt[ID+1]) {
				res[i-st].first = v;
				res[i-st].second = 1;
			} else {
				v = min(v, en-1);
				assert(v > i);
				res[i-st].first = res[v-st].first;
				res[i-st].second = res[v-st].second + 1;
			}
			for (ll j = i+1; j < en && !maximal[j-st]; j++) {
				res[j-st] = res[i-st];
			}
		}
	}
	
	{ // do the message passing
		ll start, val;
		if (ID == 0) {
			start = 0, val = 0;
		} else {
			Receive(ID-1);
			start = GetLL(ID-1);
			val = GetLL(ID-1);
		}
		assert(start >= minpt[ID]);
		if (start >= minpt[ID+1]) {
			// do nothing, just skip
		} else {
			start = min(start, en-1);
			assert(start >= st);
			val += res[start-st].second;
			start = res[start-st].first;
		}
		assert(start >= minpt[ID+1]);

		if (ID + 1 < NODES) {
			PutLL(ID+1, start);
			PutLL(ID+1, val);
			Send(ID+1);
		} else {
			cout << val << '\n';
		}
	}
}

ll go1() {
	stack<ll> stk;
	stk.push(S);
	for (ll i = S-1; i >= 0; i--) {
		ll v = GetDest(i);
		ll dst = -1;
		assert(!stk.empty() && v >= stk.top());
		while (!stk.empty() && v >= stk.top()) {
			dst = stk.top();
			stk.pop();
		}
		stk.push(dst);
		stk.push(i);
	}
	return int(stk.size())-1;
}

int main() {
	//if (ID == 0) { cout << go1() << '\n'; }
	go();

	return 0;
}
