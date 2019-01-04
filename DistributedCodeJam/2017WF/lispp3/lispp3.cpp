#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "lispp3.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();
const int MASTER = NODES - 1;

const int MAXN = int(1e9) + 5000;
const int N = GetLength();

template <typename T> T split(T n, T i, T tot) {
	return n / tot * i + min(i, n % tot);
}

const int MAXNODES = 120;
int levels[MAXNODES];
int bottoms[MAXNODES];
int bottom_plus[MAXNODES];

int res = N;

struct paren_stack {
	static const int MAXD = MAXN / 100;
	int min_level, max_level, cur_level;
	struct level_data {
		bool is_mine;
		bool has_plus;
	} data[MAXD];
	// (mi, ma]
	void init(int mi, int ma, int cur) {
		min_level = mi;
		max_level = ma;
		cur_level = cur;
		memset(data, 0, sizeof(data));
		assert((max_level - min_level) < MAXD);
	}
	void set_down(int nxt_level) {
		assert(nxt_level <= cur_level);
		cur_level = nxt_level;
	}
	void set_up(int nxt_level) {
		assert(nxt_level >= cur_level);
		for (int i = max(cur_level+1, min_level); i <= min(nxt_level, max_level); i++) {
			data[i].is_mine = false;
			data[i].has_plus = false;
		}
		cur_level = nxt_level;
	}
	// return true if good
	bool process_character(int i) {
		char c = GetCharacter(i), p = (i > 0 ? GetCharacter(i-1) : '(');
		if (i > 0 && c != '(' && p == '+') {
			return false;
		}
		if (c == '(') {
			cur_level++;
			if (min_level < cur_level && cur_level <= max_level) {
				data[cur_level - min_level].is_mine = true;
				data[cur_level - min_level].has_plus = false;
			}
			return true;
		} else if (c == ')') {
			if (cur_level == 0) return false;
			assert(i);
			if (min_level < cur_level && cur_level <= max_level) {
				if (data[cur_level - min_level].is_mine) {
					if (!(p == '(' || data[cur_level - min_level].has_plus)) {
						return false;
					}
				}
				data[cur_level - min_level].is_mine = false;
				data[cur_level - min_level].has_plus = false;
			}
			cur_level --;
			return true;
		} else if (c == '+') {
			if (p != ')') return false;
			if (cur_level == 0) return false;
			if (min_level < cur_level && cur_level <= max_level) {
				if (data[cur_level - min_level].is_mine) {
					if (data[cur_level - min_level].has_plus) {
						return false;
					}
					data[cur_level - min_level].has_plus = true;
				}
			}
			return true;
		} else assert(false);
		return false;
	}
} stk;

void phase0() {
	assert(N);
	if (GetCharacter(0) != '(') {
		// Some edge cases, make sure the first character's good.
		// This way we can safely assume that all failure points are non-zero (positive).
		if (ID == 0) cout << 0 << '\n';
		exit(0);
	}
	int st = split(N, ID, NODES), en = split(N, ID+1, NODES);
	int my_level = 0;
	int my_bottom = 0;
	int my_plus = 0;
	for (int i = st; i < en; i++) {
		char c = GetCharacter(i);
		if (c == '(') my_level ++;
		if (c == ')') my_level --;
		if (c == '+' && my_bottom == my_level) {
			assert(i);
			if (my_plus == 0) my_plus = i;
		}
		if (my_level < my_bottom) {
			my_bottom = my_level;
			my_plus = false;
		}
	}
	for (int i = 0; i < NODES; i++) {
		PutInt(i, my_level);
		PutInt(i, my_bottom);
		PutInt(i, my_plus);
		Send(i);
	}
	levels[0] = 0;
	for (int i = 0; i < NODES; i++) {
		Receive(i);
		levels[i+1] = levels[i] + GetInt(i);
		bottoms[i] = levels[i] + GetInt(i);
		bottom_plus[i] = GetInt(i);
	}
}

void go_range(int ind) {
	int st = split(N, ind, NODES), en = split(N, ind+1, NODES);
	int min_level = bottoms[ind];
	int max_level = min_level + (en - st) + 1;
	int start_level = levels[ind];
	stk.init(min_level, max_level, start_level);
	for (int i = st; i < en; i++) {
		if (!stk.process_character(i)) {
			res = min(res, i);
			return;
		}
	}
}

void phase1() {
	go_range(ID);
}

int num_jobs = 0;
void go_range2(int ind1, int ind2) {
	assert(ind1 < ind2);
	int min_level = max(bottoms[ind1], bottoms[ind2]);
	int max_level = min(levels[ind1+1], levels[ind2]);
	for (int i = ind1+1; i <= ind2-1; i++) {
		max_level = min(max_level, bottoms[i]);
	}
	if (min_level >= max_level) return;
	assert(min_level < max_level);
	if (num_jobs++ % NODES != ID) {
		// give it to someone else
		return;
	}
	int start_level = levels[ind1];
	stk.init(min_level, max_level, start_level);
	{
		int st = split(N, ind1, NODES), en = split(N, ind1+1, NODES);
		for (int i = st; i < en; i++) {
			if (!stk.process_character(i)) {
				res = min(res, i);
				return;
			}
		}
	}
	stk.set_down(max_level);
	for (int i = ind1+1; i <= ind2-1; i++) {
		if (bottoms[i] == max_level) {
			if (bottom_plus[i]) {
				if (!stk.process_character(bottom_plus[i])) {
					res = min(res, bottom_plus[i]);
					return;
				}
			}
		}
	}
	stk.set_up(levels[ind2]);
	{
		int st = split(N, ind1, NODES), en = split(N, ind1+1, NODES);
		for (int i = st; i < en; i++) {
			if (!stk.process_character(i)) {
				res = min(res, i);
				return;
			}
		}
	}
}

void phase2() {
	for (int i = 0; i < NODES; i++) {
		for (int j = i+1; j < NODES; j++) {
		}
	}
}

void phase_print() {
	assert(res <= N);
	assert(res >= 0);
	if (ID) {
		PutInt(0, res);
		Send(0);
	} else {
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			int nres = GetInt(i);
			res = min(res, nres);
		}
		assert(res >= 0);
		if (res == N && levels[NODES] == 0) {
			res = -1;
		}
		cout << res << '\n';
	}
}

int main() {
	phase0();
	phase1();
	phase2();
	phase_print();
	return 0;
}
