#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "klingon.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int split (int a, int i, int n) {
	assert(a >= 0);
	return (a / n) * i + min(a % n, i);
}
int split (int a, int b, int i, int n) {
	return split(b-a, i, n) + a;
}

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

using ans_t = uint16_t;

const int Q = int(GetNumberOfQuestions());
const ans_t A = ans_t(GetNumberOfPossibleAnswers());
const int W = int(GetAllowedNumberOfWrongAnswers());

const int MAXQ = 30000;

void PutAns(int target, ans_t a) {
	PutChar(target, char(a >> 8));
	PutChar(target, char(a & 255));
}
ans_t GetAns(int source) {
	int res = GetChar(source) << 8;
	res |= GetChar(source);
	return ans_t(res);
}

int C = 0;
ans_t ans[MAXQ];

ll res = 0;
// Returns number of successes = first failure
int runTrial() {
	if (res) {
		return Q;
	}
	for (int i = 0; i < Q; i++) {
		ll v = Answer(ans[i] % A);
		if (v == -1) {
			return i;
		} else if (v > 0) {
			assert(i == Q-1);
			res = v;
			break;
		} else {
			continue;
		}
	}
	return Q;
}

int main() {
	assert(Q < MAXQ);
	for (int i = 0; i < Q; i++) {
		ans[i] = A;
	}
	while (true) {
		const int F = runTrial();
		if (F == Q) {
			// everyone is synchronized right now
			break;
		}
		const ans_t st = ans_t(split(1,A,ID,NODES));
		const ans_t en = ans_t(split(1,A,ID+1,NODES));
		int baseline = F;
		// We're going to fix each of these
		for (ans_t a = st; a < en; a++) {
			for (int i = C; i < F; i++) {
				if (ans[i] == A) ans[i] = a;
			}
			int cnt = runTrial() - baseline;
			if (cnt == Q) {
				goto done;
			}
			for (int i = C; i < F; i++) {
				if (ans[i] == a) ans[i] = A;
			}

			int mi = C, ma = F;
			while (cnt > 0) {
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					for (int i = mi; i < md; i++) {
						if (ans[i] == A) ans[i] = a;
					}
					int v = runTrial();
					if (v == Q) {
						goto done;
					}
					for (int i = mi; i < md; i++) {
						if (ans[i] == a) ans[i] = A;
					}
					assert(v >= baseline);
					if (v > baseline) {
						// we've found one
						ma = md;
					} else {
						mi = md;
					}
				}
				ans[mi] = a;
				baseline = runTrial();
				if (baseline == Q) {
					goto done;
				}
				cnt--;
				mi++;
				ma = F;
			}
		}

done:
		for (int z = 1; z < NODES; z++) {
			int n = (ID + z) % NODES;
			for (int i = C; i < F; i++) {
				PutAns(n, ans[i]);
			}
			Send(n);
		}

		for (int z = 1; z < NODES; z++) {
			int n = (ID + NODES - z) % NODES;
			Receive(n);
			for (int i = C; i < F; i++) {
				ans_t a = GetAns(n);
				if (a != A) {
					ans[i] = a;
				}
			}
		}
		C = F;
	}

	if (ID == 0) {
		cout << res << '\n';
	}

	return 0;
}
