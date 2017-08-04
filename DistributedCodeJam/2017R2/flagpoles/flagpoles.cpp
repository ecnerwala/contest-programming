#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "flagpoles.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = GetNumFlagpoles() - 1;

int S(int id) {
	return (N / NODES) * id + min(id, N % NODES);
}

struct data {
	int len = 0;
	int max_inside = 0;
	int max_prefix = 0;
	ll prefix_val = 0;
	int max_suffix = 0;
	ll suffix_val = 0;

	data() { }

	data(ll d) {
		len = 1;
		max_inside = max_prefix = max_suffix = 1;
		prefix_val = suffix_val = d;
	}

	void append(const ll &a) {
		if (len == 0) {
			*this = data(a);
			return;
		}
		if (a == suffix_val) {
			max_suffix ++;
			max_inside = max(max_inside, max_suffix);
		} else {
			suffix_val = a;
			max_suffix = 1;
		}

		if (max_prefix == len && prefix_val == a) {
			max_prefix++;
		}

		len++;
	}

	void append(const data &d) {
		if (d.len == 0) {
			return;
		}
		if (len == 0) {
			*this = d;
			return;
		}

		max_inside = max(max_inside, d.max_inside);
		if (suffix_val == d.prefix_val) {
			max_inside = max(max_inside, max_suffix + d.max_prefix);
		}

		// prefix
		if (len == max_prefix && prefix_val == d.prefix_val) {
			max_prefix += d.max_prefix;
		}

		if (d.len == d.max_suffix && suffix_val == d.suffix_val) {
			max_suffix += d.max_suffix;
		} else {
			max_suffix = d.max_suffix;
		}

		suffix_val = d.suffix_val;

		len += d.len;
	}
};

ll GetDiff(int i) {
	return GetHeight(i + 1) - GetHeight(i);
}

int main() {
	int st = S(ID), en = S(ID + 1);
	data d;
	for (int i = st; i < en; i++) {
		d.append(GetDiff(i));
	}
	if (ID != 0) {
		PutInt(0, d.len);
		PutInt(0, d.max_inside);
		PutInt(0, d.max_prefix);
		PutLL(0, d.prefix_val);
		PutInt(0, d.max_suffix);
		PutLL(0, d.suffix_val);
		Send(0);
	} else {
		for (int id = 1; id < NODES; id++) {
			data o;
			Receive(id);
			o.len = GetInt(id);
			o.max_inside = GetInt(id);
			o.max_prefix = GetInt(id);
			o.prefix_val = GetLL(id);
			o.max_suffix = GetInt(id);
			o.suffix_val = GetLL(id);
			d.append(o);
		}
		assert(d.len == N);
		cout << d.max_inside + 1 << '\n';
	}
	return 0;
}
