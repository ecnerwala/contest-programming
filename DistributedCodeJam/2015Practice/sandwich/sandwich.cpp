#include "message.h"
#include "sandwich.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetN();

const ll L = (N + NODES - 1) / NODES;

struct seg {
	ll sum;
	ll msum;
	ll lsum;
	ll rsum;

	seg() : sum(0LL), msum(0LL), lsum(0LL), rsum(0LL) { }

	seg(ll v) : sum(v), msum(min(v, 0LL)), lsum(msum), rsum(msum) { }

	seg(seg a, seg b) : 
		sum(a.sum + b.sum),
		msum(min(min(a.msum, b.msum), a.rsum + b.lsum)),
		lsum(min(a.lsum, a.sum + b.lsum)),
		rsum(min(b.rsum, b.sum + a.rsum)) { }
};

seg GetSeg(int source) {
	seg s;
	s.sum = GetLL(source);
	s.msum = GetLL(source);
	s.lsum = GetLL(source);
	s.rsum = GetLL(source);
	return s;
}

void PutSeg(int target, seg s) {
	PutLL(target, s.sum);
	PutLL(target, s.msum);
	PutLL(target, s.lsum);
	PutLL(target, s.rsum);
}

int main() {
	seg cur;
	for(ll i = L * ID; i < N && i < L * (ID + 1); i++) {
		cur = seg(cur, GetTaste(i));
	}

	for(int i = 1; i <= ID || ID + i < NODES; i <<= 1) {
		if(ID & i) {
			//cerr << "Send to " << (ID - i) << '\n';
			PutSeg(ID - i, cur);
			Send(ID - i);
			break;
		} else if (ID + i < NODES){
			//cerr << "Receive from " << (ID + i) << '\n';
			Receive(ID + i);
			cur = seg(cur, GetSeg(ID + i));
		}
	}

	if(ID == 0) {
		printf("%lld\n", cur.sum - cur.msum);
	}
	return 0;
}
