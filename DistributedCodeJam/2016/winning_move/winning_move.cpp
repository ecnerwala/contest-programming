#include "message.h"
#include "winning_move.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int BNODES = NODES / 5;
const int ID = MyNodeId();

const ll N = GetNumPlayers();

const ll S = (N + NODES - 1) / NODES;
const ll B = (N + BNODES - 1) / BNODES;

void unique2(vector<ll> &v) {
	int ind = 0;
	for(int i = 0; i < int(v.size()); i++) {
		if(ind >= 2 && v[i] == v[ind - 1] && v[i] == v[ind - 2]) {
			continue;
		} else {
			v[ind] = v[i];
			ind++;
		}
	}
	v.resize(ind);
}

const ll L = 1e6;

int cnt[2 * L];
int num_big;
vector<int> bigrange;
int bigcnt[30][2 * L];

vector<ll> cutoffs;

vector<ll> myvals;

int main() {
	if(ID < BNODES) {
		vector<ll> vals;
		for(ll i = B * ID; i < N && i < B * (ID + 1); i++) {
			vals.push_back(GetSubmission(i) - 1);
		}
		sort(vals.begin(), vals.end());
		unique2(vals);

		for(ll i : vals) {
			cnt[i / L / L]++;
		}

		if(ID) {
			for(int i = 0; i < L; i++) {
				PutInt(0, cnt[i]);
			}
			Send(0);
		}
		if(!ID) {
			for(int z = 1; z < BNODES; z++) {
				int source = Receive(-1);
				for(int i = 0; i < L; i++) {
					cnt[i] += GetInt(source);
				}
			}
			//cerr << "counts summed\n";
		}


		if(!ID) {
			for(int i = 0; i < L; i++) {
				if(cnt[i] > 2 * L) bigrange.push_back(i);
			}
			num_big = int(bigrange.size());
			for(int i = 1; i < BNODES; i++) {
				PutInt(i, num_big);
				for(int v : bigrange) {
					PutInt(i, v);
				}
				Send(i);
			}
			//cerr << "bigs sent\n";
		}
		if(ID) {
			Receive(0);
			num_big = GetInt(0);
			bigrange.resize(num_big);
			for(int z = 0; z < num_big; z++) {
				bigrange[z] = GetInt(0);
			}
			//cerr << "bigs received\n";
		}
		for(int z = 0; z < num_big; z++) {
			cnt[bigrange[z]] = -1-z;
		}
		for(int z = 0; z < num_big; z++) {
			int v = bigrange[z];
			for(ll i : vals) {
				if(int(i / L / L) == v) {
					bigcnt[z][i / L % L] ++;
				}
			}
		}
		if(ID) {
			for(int z = 0; z < num_big; z++) {
				PutInt(0, z);
				for(int i = 0; i < L; i++) {
					PutInt(0, bigcnt[z][i]);
				}
				Send(0);
			}
			//cerr << "bitcnts sent\n"
		}
		if(!ID) {
			for(int t = 1; t < BNODES; t++) {
				for(int z = 0; z < num_big; z++) {
					int source = Receive(-1);
					int v = GetInt(source);
					for(int i = 0; i < L; i++) {
						bigcnt[v][i] += GetInt(source);
					}
				}
			}
			//cerr << "bigcnts received\n";
		}

		if(!ID) {
			// now we decide cutoffs

			ll v = 0, s = 0;
			while(v < L * L * L) {
				assert(v % L == 0);
				if(cnt[v / L / L] >= 0) {
					assert(v % (L * L) == 0);
					s += cnt[v / L / L];
					if(s > 3 * L) {
						cutoffs.push_back(v);
						s = cnt[v / L / L];
					}
					v += (L * L);
				} else {
					int z = -1 - cnt[v / L / L];
					s += bigcnt[z][v / L % L];
					if(s > 3 * L) {
						cutoffs.push_back(v);
						s = bigcnt[z][v / L % L];
					}
					v += L;
				}
			}
			assert(int(cutoffs.size()) < NODES);
			for(int i = 1; i < BNODES; i++) {
				PutInt(i, int(cutoffs.size()));
				for(ll c : cutoffs) {
					PutLL(i, c);
				}
				Send(i);
			}
		}
		if(ID) {
			Receive(0);
			cutoffs.resize(GetInt(0));
			for(ll &v : cutoffs) {
				v = GetLL(0);
			}
		}
		//cerr << "got cutoffs\n";
		//for(ll i : cutoffs) cerr << i << ' '; cerr << '\n';
		for(int i = 0, v = 0; i < int(vals.size()); ) {
			if(v < int(cutoffs.size()) && vals[i] >= v) {
				v++;
			} else {
				if(v == ID) {
					myvals.push_back(vals[i]);
				} else {
					PutLL(v, vals[i]);
				}
				i++;
			}
		}
		for(int i = 0; i < NODES; i++) {
			if(i == ID) continue;
			PutLL(i, ll(-1));
			Send(i);
		}
	}

	for(int i = 0; i < BNODES; i++) {
		if(i == ID) continue;
		int source = Receive(i);
		while(true) {
			ll v = GetLL(source);
			if(v == -1) break;
			myvals.push_back(v);
		}
	}
	//cerr << "hi\n";
	myvals.push_back(-1);
	myvals.push_back(ll(1e20));
	sort(myvals.begin(), myvals.end());
	ll best = -1;
	for(int i = 1; i + 1 < int(myvals.size()); i++) {
		if(myvals[i] != myvals[i + 1] && myvals[i] != myvals[i - 1]) {
			best = myvals[i];
			break;
		}
	}
	if(ID) {
		Receive(ID - 1);
		ll prv = GetLL(ID - 1);
		if(prv != -1) best = prv;
	}
	if(ID + 1 < NODES) {
		PutLL(ID + 1, best);
		Send(ID + 1);
	} else {
		printf("%lld\n", best + 1);
	}

	return 0;
}
