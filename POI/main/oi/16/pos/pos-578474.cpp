/*
Rozwiazanie przykladowe dla zawodnikow do zadania POS
*/
#include "poslib.h"
#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;
typedef pair<ll, pii> plp;
#define A first
#define B second

ll N, A, B;

map<pii, ll> mapCh;

//returns (a + b) choose b
ll choose(ll a, ll b) {
	if(a > b) swap(a, b);
	if(a == 0) return 1;
	pii cur = pii(a, b);
	if(mapCh.count(cur)) return mapCh[cur];
	return mapCh[cur] = choose(a, b - 1) + choose(a - 1, b);
}


class depth_comp {
	//returns whether r is sooner
	public:
	bool operator() (pii l, pii r) {
		return r.A * A + r.B * B < l.A * A + l.B * B;
	}
};

vector<pii> choice;

int main() {
	int t1, t2, t3;
	inicjuj(&t1, &t2, &t3);
	N = t1, A = t2, B = t3;

	//precomputation
	ll numVert = 0;
	ll numB = 0;

	priority_queue<pii, vector<pii>, depth_comp > pq;
	pq.push(pii(0, 0));

	while(numVert <= N) {
		pii val = pq.top(); pq.pop();
		numVert += choose(val.A, val.B);
		if(val.B) numB += choose(val.A, val.B - 1);
		//cerr << val.A << ' ' << val.B << ' ';
		//cerr << numVert << ' ' << numB << '\n';

		choice.push_back(pii(numVert, numB));

		pq.push(pii(val.A, val.B + 1));
		if(!val.B) pq.push(pii(val.A + 1, 0));
	}

	ll l = 1, r = N; //[l, r]
	while(r > l) {

		ll d = r - l + 1;
		// must have >= (r - l + 1) leaves
		// must have at least 2d-1 nodes
		
		d *= 2, d--;
 
		// given at least d vertices, how many on b-side
		
		ll v = lower_bound(choice.begin(), choice.end(), pii(d, 0))->second; // number of vertices on b-side

		//call function 
		ll p = l + (v + 1) / 2; // number of leaves on b-side
		if(pytaj('W', p)) l = p + 1; else r = p;
	}
	
	odpowiedz(l);
	return 0;
}
