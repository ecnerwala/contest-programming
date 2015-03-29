#include<bits/stdc++.h>
using namespace std;

const int MAXM = 2e6;

typedef pair<int, int> pii;
#define A first
#define B second

vector<int> st[MAXM * 2]; // stores position of starts
vector<pii> stq; // stores value, position of suffix minima
vector<pii> enq; // stores value, position of suffix maxima

// remove from the back of the stq
void stq_remove_back() {
	assert(!stq.empty());

	pii v = stq.back();
	stq.pop_back();

	assert(!st[v.A - v.B + MAXM].empty());
	assert(st[v.A - v.B + MAXM].back() == v.B);
	st[v.A - v.B + MAXM].pop_back();
}

void stq_insert_back(pii v) {
	stq.push_back(v);
	st[v.A - v.B + MAXM].push_back(v.B);
}

vector<pii> res;

void process(pii v) {
	while(!stq.empty() && stq.back().A > v.A) {
		stq_remove_back();
	}

	while(!enq.empty() && enq.back().A < v.A) {
		enq.pop_back();
	}

	if(!st[v.A - v.B + MAXM].empty()) {

		int start = st[v.A - v.B + MAXM].back();
		assert(start < v.B);

		if(enq.empty() || enq.back().B < start) {

			res.push_back(pii(start, v.B));
			while(!stq.empty()) stq_remove_back();
			while(!enq.empty()) enq.pop_back();
		}

	}

	stq_insert_back(v);
	enq.push_back(v);
}

int main() {
	int T;
	scanf("%d", &T);
	for(int t = 0; t < T; t++) {

		int M;
		scanf("%d", &M);

		res.clear();

		for(int i = 1; i <= M; i++) {
			int v;
			scanf("%d", &v);
			process(pii(v, i));
		}

		while(!stq.empty()) stq_remove_back();
		while(!enq.empty()) enq.pop_back();

		printf("%d\n", int(res.size()));
		for(pii v : res) {
			printf("%d %d\n", v.A, v.B);
		}

	}
	return 0;
}
