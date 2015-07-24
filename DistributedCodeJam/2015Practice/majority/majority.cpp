#include "message.h"
#include "majority.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetN();

const ll L = (N + NODES - 1) / NODES;

struct votes {
	int winner;
	int count; 

	votes() : winner(0), count(0) {}

	votes(int person) : winner(person), count(1) {}
};

votes merge(votes a, votes b) {
	if(a.count < b.count) swap(a, b);
	assert(a.count >= b.count);
	if(a.winner == b.winner) {
		a.count += b.count;
	} else {
		a.count -= b.count;
	}
	assert(a.count >= 0);
	return a;
}

void PutVotes(int target, votes v) {
	PutInt(target, v.winner);
	PutInt(target, v.count);
}
votes GetVotes(int source) {
	votes v;
	v.winner = GetInt(source);
	v.count = GetInt(source);
	return v;
}

int main() {
	votes cur;
	for(ll i = L * ID; i < L * (ID + 1) && i < N; i++) {
		cur = merge(cur, int(GetVote(i)));
	}

	for(int i = 1; i <= ID || ID + i < NODES; i <<= 1) {
		if(ID & i) {
			PutVotes(ID - i, cur);
			Send(ID - i);
			break;
		} else if(ID + i < NODES) {
			Receive(ID + i);
			cur = merge(cur, GetVotes(ID + i));
		}
	}
	
	int cnd;
	if(ID == 0) {
		cnd = cur.winner;
		for(int i = 1; i < NODES; i++) {
			PutInt(i, cnd);
			Send(i);
		}
	} else {
		Receive(0);
		cnd = GetInt(0);
	}

	int cnt = 0;

	for(ll i = L * ID; i < L * (ID + 1) && i < N; i++) {
		if(GetVote(i) == cnd) cnt++;
	}

	for(int i = 1; i <= ID || ID + i < NODES; i <<= 1) {
		if(ID & i) {
			PutInt(ID - i, cnt);
			Send(ID - i);
			break;
		} else if(ID + i < NODES) {
			Receive(ID + i);
			cnt += GetInt(ID + i);
		}
	}

	if(ID == 0) {
		if(cnt > N - cnt) {
			printf("%d\n", cnd);
		} else {
			printf("NO WINNER\n");
		}
	}

	return 0;
}
