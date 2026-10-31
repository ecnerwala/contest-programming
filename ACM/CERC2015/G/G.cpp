#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	vector<int> H(N);
	for (int i = 0; i < N; i++) {
		cin >> H[i];
	}

	vector<int> nxt(N);
	vector<int> prv(N);
	for (int i = 0; i < N; i++) {
		nxt[i] = i+1;
		prv[i] = i-1;
	};

	auto deletePt = [&](int i) {
		int p = prv[i], n = nxt[i];
		prv[i] = nxt[i] = i;
		if (p != -1) nxt[p] = n;
		if (n != N) prv[n] = p;
	};

	int curA = 0, curB = 0;

	vector<int> va(N);
	vector<int> vb(N);

	auto getCurH = [&](int i) {
		return H[i] + va[i] * curA + vb[i] * curB;
	};

	auto updateVel = [&](int i) {
		//cerr << "updateVel" << i << '\n';
		int curH = getCurH(i);
		va[i] = (prv[i] == -1) ? 0 : (curH < getCurH(prv[i]));
		vb[i] = (nxt[i] == N) ? 0 : (curH < getCurH(nxt[i]));
		H[i] = curH - va[i] * curA - vb[i] * curB;
	};

	for (int i = 0; i < N; i++) {
		updateVel(i);
	}

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq[3][3];

	auto addEvts = [&](int i) {
		//cerr << "addEvts" << i << '\n';
		assert(prv[i] != i);
		if (prv[i] == -1) return;

		int p = prv[i];

		int dva = va[i] - va[p];
		int dvb = vb[i] - vb[p];
		int dh = H[p] - H[i];

		if (dh < 0) {
			dh = -dh, dva = -dva, dvb = -dvb;
		}

		// we look for curA * dva + curB * dvb >= dh
		pq[dva+1][dvb+1].emplace(dh, i);
	};

	for (int i = 1; i < N; i++) {
		addEvts(i);
	}

	auto checkPt = [&](int a) {
		assert(a);
		assert(prv[a] != -1);
		if (prv[a] == a) return;
		int p = prv[a];
		if (getCurH(prv[a]) == getCurH(a)) {
			deletePt(a);
			updateVel(p);
			addEvts(p);
			if (nxt[p] != N) {
				addEvts(nxt[p]);
			}
		}
	};

	auto checkAll = [&]() {
		while (true) {
			bool found = false;
			for (int dva = -1; dva <= 1; dva++) {
				for (int dvb = -1; dvb <= 1; dvb++) {
					int cur = curA * dva + curB * dvb;
					auto& q = pq[dva+1][dvb+1];
					while (!q.empty() && cur >= q.top().first) {
						int i = q.top().second; q.pop();
						checkPt(i);
						found = true;
					}
				}
			}
			if (!found) break;
		}
	};

	auto dbg = [&]() {
		int lastH = -1;
		for (int i = 0; i < N; i++) {
			if (prv[i] != i) lastH = getCurH(i);
			cerr << lastH << " \n"[i+1==N];
		}
	};

	checkAll();

	string OPS; cin >> OPS;
	for (char c : OPS) {
		if (c == 'A') {
			curA++;
			checkAll();
		} else if (c == 'B') {
			curB++;
			checkAll();
		} else assert(false);
	}

	int lastH = -1;
	for (int i = 0; i < N; i++) {
		if (prv[i] != i) lastH = getCurH(i);
		cout << lastH << " \n"[i+1==N];
	}

	return 0;
}
