#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L, Q; cin >> L >> Q;

	std::vector<std::array<int, 2>> A(L);
	for (auto& a : A) {
		cin >> a[0] >> a[1]; a[1]++;
	}

	const int B = 30;
	const int MAX_NODES = int(5e5)+1;
	std::vector<std::array<int, 2>> ch; ch.reserve(MAX_NODES);
	ch.push_back({-1, -1});

	std::vector<int> query_nodes(Q);
	for (int q = 0; q < Q; q++) {
		string S; cin >> S;
		int cur = 0;
		for (char c : S) {
			assert(c == '0' || c == '1');
			int v = c - '0';
			int nxt = ch[cur][v];
			if (nxt == -1) {
				ch[cur][v] = nxt = int(ch.size());
				ch.push_back({-1, -1});
			}
			cur = nxt;
		}
		query_nodes[q] = cur;
	}

	const int NUM_NODES = int(ch.size());

	std::vector<int> fail(NUM_NODES);
	std::vector<int> node_queue; node_queue.reserve(NUM_NODES);

	fail[0] = -1;
	node_queue.push_back(0);
	for (int z = 0; z < NUM_NODES; z++) {
		int cur = node_queue[z];
		for (int v = 0; v < 2; v++) {
			int nxt = ch[cur][v];
			if (nxt != -1) {
				fail[nxt] = cur ? ch[fail[cur]][v] : 0;
				node_queue.push_back(nxt);
			} else {
				ch[cur][v] = cur ? ch[fail[cur]][v] : 0;
			}
		}
	}

	std::array<std::vector<std::array<int, 2>>, B> jump;
	jump[0] = ch;
	for (int b = 1; b < B; b++) {
		jump[b].resize(NUM_NODES);
		const auto& pjump = jump[b-1];
		auto& njump = jump[b];
		for (int i = 0; i < NUM_NODES; i++) {
			njump[i][0] = pjump[pjump[i][0]][1];
			njump[i][1] = pjump[pjump[i][1]][0];
		}
	}

	std::array<std::vector<std::array<int64_t, 2>>, B> jump_cnt;
	for (int b = 0; b < B; b++) {
		jump_cnt[b].resize(NUM_NODES, {0, 0});
	}

	{
		int cur = 0;
		for (auto [l, r] : A) {
			assert(l <= r);
			while (l < r) {
				assert(l > 0);
				int b = __builtin_ctz(l);
				if (l + (1 << b) > r) {
					b = 32 - 1 - __builtin_clz(l ^ r);
				}
				assert(l + (1 << b) <= r);

				int v = __builtin_popcount(l) & 1;

				jump_cnt[b][cur][v]++;
				cur = jump[b][cur][v];

				l += (1 << b);
			}
		}

		// add one more jump for that match
		jump_cnt[0][cur][0]++;
	}

	for (int b = B-1; b > 0; b--) {
		const auto& pcnt = jump_cnt[b];
		auto& ncnt = jump_cnt[b-1];
		const auto& njump = jump[b-1];
		for (int i = 0; i < NUM_NODES; i++) {
			ncnt[i][0] += pcnt[i][0];
			ncnt[njump[i][0]][1] += pcnt[i][0];

			ncnt[i][1] += pcnt[i][1];
			ncnt[njump[i][1]][0] += pcnt[i][1];
		}
	}

	std::vector<int64_t> cnt(NUM_NODES);
	for (int i = 0; i < NUM_NODES; i++) {
		cnt[i] = jump_cnt[0][i][0] + jump_cnt[0][i][1];
	}

	for (int z = NUM_NODES-1; z > 0; z--) {
		int cur = node_queue[z];
		cnt[fail[cur]] += cnt[cur];
	}

	for (int q = 0; q < Q; q++) {
		cout << cnt[query_nodes[q]] << '\n';
	}

	return 0;
}
