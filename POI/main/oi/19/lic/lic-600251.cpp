#include "cliclib.h"

#include<algorithm>
#include<map>
#include<cassert>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

// pile, limit

pii dpot(pii s) {
	return pii(s.A * 2, s.B);
}

pii tpot(pii s) {
	return pii(s.A * 3, s.B);
}

pii pass(pii s) {
	return pii(1, s.B - s.A);
}

pii makemove(pii s, int a) {
	switch(a) {
		case 1: return pass(s);
		case 2: return dpot(s);
		case 3: return tpot(s);
		default: assert(false);
	}
}

map<pii, int> mem;
int getmove(pii s) {
	assert(s.B > 0);
	assert(s.A > 0);
	if(s.A >= s.B) return 0;
	if(mem.count(s)) return mem[s];
	int res = 0;
	for(int i = 3; i >= 1; i--) {
		if(getmove(makemove(s, i)) == 0) {
			// losing for opponent
			res = i;
			break;
		}
	}
	//cerr << s.A << ' ' << s.B << ' ' << res << '\n';
	//if(s.B == 30000) cerr << s.A << '\n';
	//cerr << mem.size() << '\n';
	return mem[s] = res;
}

int main() {
	int N = inicjuj();
	pii st (1, N);
	while (st.B > 0) {
		int move = getmove(st);
		assert(move);
		alojzy(move);
		st = makemove(st, move);
		int x = bajtazar();
		st = makemove(st, x);
	}
	return 0;
}
