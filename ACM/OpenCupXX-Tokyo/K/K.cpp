#include<bits/stdc++.h>
using namespace std;

const int MAXX = 1.01e6;
const int MAX_VBITS = 30;
const int MAXV = 1 << MAX_VBITS;

struct seg_node {
	int len = 0;

	int64_t totCnt = 0;
	int64_t totSum = 0;

	int numCarry = 0;
	int numNeed = 1; // number you need to convert the first kill into a carry, 0 for no way

	int64_t numKill() const {
		return totCnt - numCarry;
	}
	int64_t numSteps() const {
		return totSum - int64_t(numCarry) * len;
	}

	int64_t kCost(int64_t K) const {
		assert(numKill() <= K);
		K -= numKill();
		return numSteps() + K * (len - 1); // 2 * K * len - K * (len+1)
	}
};

seg_node init_node(int cnt) {
	assert(0 <= cnt && cnt < MAXV);
	seg_node r;

	r.len = 1;

	r.totCnt = cnt;

	r.totSum = 0;

	r.numCarry = cnt / 2;
	r.numNeed = cnt % 2 ? 1 : 2;

	return r;
}

seg_node merge_nodes(seg_node a, seg_node b) {
	if (!a.len) return b;
	if (!b.len) return a;

	seg_node r;
	r.len = a.len + b.len;

	r.totCnt = a.totCnt + b.totCnt;
	r.totSum = a.totSum + b.totSum + int64_t(b.totCnt) * a.len;

	r.numCarry = b.numCarry;
	if (b.numNeed && a.numCarry >= b.numNeed) {
		r.numCarry++;
		a.numCarry -= b.numNeed;
		if (b.len >= MAX_VBITS) {
			b.numNeed = 0;
		} else {
			b.numNeed = 1 << b.len;
			r.numCarry += a.numCarry >> b.len;
			a.numCarry &= (1 << b.len) - 1;
		}
	}

	if (b.numNeed) {
		assert(b.numNeed > a.numCarry);
		b.numNeed -= a.numCarry;
	}

	if (b.numNeed && a.numNeed) {
		if (b.numNeed > 1 && (a.len >= MAX_VBITS || b.numNeed - (a.numNeed < (1<<a.len)) >= (1 << (MAX_VBITS - a.len)))) {
			r.numNeed = 0;
		} else {
			r.numNeed = ((b.numNeed-1) << a.len) + a.numNeed;
		}
	} else {
		r.numNeed = 0;
	}

	return r;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);

	const int S = 1 << 20;
	vector<seg_node> seg(2*S);
	for (int i = 0; i < S; i++) {
		seg[S+i] = init_node(i==S-1); // put one extra at S-1
	}
	for (int i = S-1; i; i--) seg[i] = merge_nodes(seg[2*i], seg[2*i+1]);

	int num1 = 0;

	int Q; cin >> Q;
	while (Q--) {
		int op; cin >> op;

		if (op == 1) {
			int x, y; cin >> x >> y;
			if (x == 1) {
				num1 = y;
			} else {
				seg[S+x] = init_node(y);
				for (int a = (S+x)/2; a; a /= 2) seg[a] = merge_nodes(seg[2*a], seg[2*a+1]);
			}
		} else if (op == 2) {
			int64_t K; cin >> K;
			int ans1 = int(min<int64_t>(num1, K));
			K -= ans1;

			assert(K < seg[1].numKill());
			seg_node cur;
			for (int b = 1; b < S; ) {
				assert(merge_nodes(cur, seg[b]).numKill() > K);
				b *= 2;
				seg_node cnd = merge_nodes(cur, seg[b]);
				if (cnd.numKill() <= K) {
					cur = cnd;
					b++;
				}
				assert(merge_nodes(cur, seg[b]).numKill() > K);
			}
			cout << ans1 + cur.kCost(K) << '\n';
		}
	}

	return 0;
}
