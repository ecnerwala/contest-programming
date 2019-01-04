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

const ll S = GetNumberOfStacks();
const ll H = GetNumberOfTowelsInStack();

const ll T = S * H;

const ll K = GetNumberOfGymUsersBeforeYou();

ll split(ll a, ll i, ll n) {
	return (a / n) * i + min(a % n, i);
}

const ll MAXSPLIT = 3e7;
int N;
pair<ll, ll> top[MAXSPLIT];

ll num_smaller(ll q) {
	auto it = lower_bound(top, top + N, pair<ll, ll>(q, -1));
	if (it == top) {
		return 0;
	} else {
		it--;
		return it->second;
	}
}

void go() {
	{
		ll st = split(S, ID, NODES), en = split(S, ID+1, NODES);
		assert((en - st) * H < MAXSPLIT);
		N = 0;
		for (ll s = st; s < en; s++) {
			ll p = H-1;
			while (p >= 0) {
				ll v = GetTowelCleanlinessRank(s, p);
				top[N].first = v;
				top[N].second = 0;
				while (p >= 0 && GetTowelCleanlinessRank(s, p) <= v) {
					top[N].second ++;
					p--;
				}
				N++;
			}
		}
		sort(top, top + N);

		top[N].second = 0;
		for (int i = 0; i < N; i++) {
			top[i+1].second += top[i].second;
		}
		assert(top[N].second == (en - st) * H);
	}
	/*
	cerr << N << '\n';
	for (int i = 0; i < N; i++) {
		cerr << top[i].first << ' ' << top[i].second << '\n';
	}
	*/

	{
		// collaborative binary search
		ll mi = 0, ma = T+1;
		for (int round = 0; ma - mi > 1; round ++) {
			const int leader = round % NODES;
			//if (ID == leader) cerr << mi << ' ' << ma << ' ' << leader << '\n';

			const ll PIECES = 1000;
			for (int i = 1; i < PIECES; i++) {
				ll q = mi + split(ma - mi, i, PIECES);
				PutLL(leader, num_smaller(q));
			}
			Send(leader);

			if (ID == leader) {
				vector<ll> cnts(PIECES);
				for (int i = 1; i < PIECES; i++) {
					cnts[i] = 0;
				}
				for (int o = 0; o < NODES; o++) {
					Receive(o);
					for (int i = 1; i < PIECES; i++) {
						cnts[i] += GetLL(o);
					}
				}
				ll nmi = mi, nma = ma;
				for (int i = 1; i < PIECES; i++) {
					ll q = mi + split(ma - mi, i, PIECES);
					//if (i == 1 || q != mi + split(ma-mi, i-1, PIECES)) cerr << q << ' ' << cnts[i] << '\n';
					if (cnts[i] <= K) {
						nmi = max(nmi, q);
					} else {
						nma = min(nma, q);
					}
				}
				for (int i = 0; i < NODES; i++) {
					PutLL(i, nmi);
					PutLL(i, nma);
					Send(i);
				}
			}
			Receive(leader);
			mi = GetLL(leader);
			ma = GetLL(leader);
		}

		{
			// mi is the start of the result, we just have to find the offset
			ll v = num_smaller(mi);
			for (int i = 0; i < NODES; i++) {
				PutLL(i, v);
				Send(i);
			}
		}

		ll cnt_small;
		{
			cnt_small = 0;
			for (int i = 0; i < NODES; i++) {
				Receive(i);
				cnt_small += GetLL(i);
			}
		}

		//cerr << mi << ' ' << ma << '\n';
		//cerr << cnt_small << '\n';

		assert(cnt_small <= K);
		ll offset = K - cnt_small;

		ll st = split(S, ID, NODES), en = split(S, ID+1, NODES);
		for (ll s = st; s < en; s++) {
			for (ll p = H-1; p >= 0; p--) {
				if (GetTowelCleanlinessRank(s, p) == mi) {
					assert(offset <= p);
					cout << GetTowelCleanlinessRank(s, p - offset) << '\n';
					return;
				}
			}
		}
	}
}

int main() {
	go();

	return 0;
}
