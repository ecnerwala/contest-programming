#include "message.h"
#include "rps.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = int(GetN());

const int D = min(N, (NODES == 10) ? 3 : 6);
const int S = 1 << (N - D);

unordered_map<int, char> cache;
char move(int a) {
	if(cache.count(a)) return cache[a];
	else return cache[a] = GetFavoriteMove(a);
}

int winner(int a, int b) {
	char x = move(a), y = move(b);
	if(x == y) return min(a, b);
	if(x == 'P' && y == 'R') return a;
	if(x == 'R' && y == 'S') return a;
	if(x == 'S' && y == 'P') return a;
	return b;
}

int winner(int a) {
	if(a >= (1 << N)) {
		return a - (1 << N);
	} else {
		return winner(winner(2 * a), winner(2 * a + 1));
	}
}

int bests[3 << 6];

int winner0(int a) {
	if(a >= (1 << D)) {
		return bests[a - (1 << D)];
	} else {
		return winner(winner(2 * a), winner(2 * a + 1));
	}
}

int main() {

	if(ID >= (1 << D)) {
		return 0;
	}

	int mine = winner((1 << D) + ID);

	if(ID) {
		PutInt(0, mine);
		Send(0);
	} else {
		bests[0] = mine;
		for(int z = 1; z < (1 << D); z++) {
			int source = Receive(-1);
			bests[source] = GetInt(source);
		}
		
		printf("%d\n", winner0(1));
	}

	return 0;
}
