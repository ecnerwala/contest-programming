#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2e5;

struct seg {
	static const int L = 18;
	static const int S = 1 << L;

	struct data {
		ll size;
		ll sum;
		ll lazy;
	};

	data d[2 * S];

	seg () {
		for (int i = 2 * S - 1; i > 0; i--) {
			if (i >= S) {
				d[i].size = 1;
				d[i].sum = 0;
				d[i].lazy = 0;
			} else {
				d[i].size = d[2 * i].size + d[2 * i + 1].size;
				d[i].sum = d[2 * i].sum + d[2 * i + 1].sum;
				d[i].lazy = 0;
			}
		}
	}

	void propogate(int a) {
		if (d[a].lazy) {
			d[2 * a].lazy = d[2 * a].lazy ? d[2 * a]
		}
	}

	void update(int a) {
		if (d[a].lazy) {
			d[a].sum = d[a].size * d[a].lazy;
		}
	}

	void update_all(int )

	void update(int k, int l, int r) {
		assert(k > 0);
		r++;
	}

	ll query(int l, int r) {
		r++;
	}
};

data seg[2][2 * S];


int main() {

	return 0;
}
