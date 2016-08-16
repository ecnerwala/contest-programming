#include<bits/stdc++.h>
using namespace std;

typedef long double ld;

const ld INF = 1e99;

const int MAXN = 2e5 + 20;

int N, T, Q;

int P[MAXN]; // prize in ith raffle
int O[MAXN]; // number of other tickets in i'th raffle
int R[MAXN]; // total number of tickets in i'th raffle

priority_queue< pair<ld, int> > to_add, to_remove;

int tot_used;
ld val;

ld get_add_value(int i) {
	assert(O[i] <= R[i] && R[i] <= 2 * O[i]);
	if(R[i] == O[i] * 2) return -INF;
	return ld(P[i]) * O[i] / (R[i]) / (R[i] + 1);
}

ld get_remove_value(int i) {
	assert(O[i] <= R[i] && R[i] <= 2 * O[i]);
	if(R[i] == O[i]) return -INF;
	assert(R[i] > O[i]);
	assert(O[i] >= 1);
	assert(R[i] > 1);
	return -ld(P[i]) * O[i] / (R[i]) / (R[i] - 1);
}

void update_vals(int r) {
	assert(O[r] <= R[r] && R[r] <= 2 * O[r]);
	to_add.emplace(get_add_value(r), r);
	to_remove.emplace(get_remove_value(r), r);
}

// returns true if successful
bool add_one() {
	while(to_add.top().first != get_add_value(to_add.top().second)) {
		to_add.pop();
		assert(!to_add.empty());
	}

	int r = to_add.top().second;

	if(to_add.top().first == -INF) return false;
	assert(R[r] < 2 * O[r]);

	val += to_add.top().first;

	to_add.pop();

	R[r] ++;
	tot_used ++;

	update_vals(r);

	return true;
}

// returns true if successful
bool remove_one() {
	while(to_remove.top().first != get_remove_value(to_remove.top().second)) {
		to_remove.pop();
		assert(!to_remove.empty());
	}

	int r = to_remove.top().second;

	if(to_remove.top().first == -INF) return false;
	assert(R[r] > O[r]);

	val += to_remove.top().first;

	to_remove.pop();

	R[r] --;
	tot_used --;

	update_vals(r);

	return true;
}

void update() {
	while(tot_used > T) {
		assert(remove_one());
	}
	while(tot_used < T) {
		if(!add_one()) break;
	}
}

int main() {

	scanf("%d %d %d", &N, &T, &Q);

	for(int i = 0; i < N; i++) {
		scanf("%d", &P[i]);
	}

	for(int i = 0; i < N; i++) {
		scanf("%d", &O[i]);
		R[i] = O[i];
	}

	tot_used = 0;
	val = 0;

	for(int r = 0; r < N; r++) {
		update_vals(r);
	}

	assert(int(to_add.size()) == N);
	assert(int(to_remove.size()) == N);

	update();

	for(int q = 0; q < Q; q++) {
		int t, r; cin >> t >> r; r--;
		assert(O[r] <= R[r] && R[r] <= 2 * O[r]);

		if(t == 1) remove_one();

		if(t == 1 && R[r] == O[r]) {
			// it doesn't get any better, just exit in this case
			R[r] ++;
			O[r] ++;
		} else if(t == 1) {
			assert(O[r] < R[r]);
			O[r] ++;
			val -= ld(P[r]) / R[r];
			tot_used --;
		} else if(t == 2 && R[r] == 2 * O[r]) {
			// we have to kill the extra ticket too
			R[r] -= 2;
			O[r] --;
			tot_used --;
			// we don't change the value of 1/2
		} else if(t == 2 && R[r] == 2 * O[r] - 1) {
			// we have to kill an extra ticket
			if(R[r] == 1 && O[r] == 1) {
				R[r] --;
				O[r] --;
			} else {
				val += ld(P[r]) * O[r] / R[r]; // add the chance of losing
				R[r] --;
				O[r] --;
				val -= ld(P[r]) * O[r] / R[r];
			}
		} else if (t == 2) {
			assert(O[r] > 1);
			O[r] --;
			val += ld(P[r]) / R[r];
			tot_used ++;
		} else assert(false);

		assert(O[r] <= R[r] && R[r] <= 2 * O[r]);

		update_vals(r);

		update();

		printf("%.10Lf\n", val);
	}
}
