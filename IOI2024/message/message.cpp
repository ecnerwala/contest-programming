#include "message.h"

#include <bits/stdc++.h>

void send_message(std::vector<bool> M, std::vector<bool> C) {
	// Pad M first
	M.push_back(1);
	M.resize(1025, 0);

	int idx = 0;
	std::vector<std::vector<bool>> R(66, std::vector<bool>(31));
	for (int i = 0; i < 31; i++) {
		if (C[i] == 0) {
			bool is_pref = true;
			for (int j = 0; j < 66; j++) {
				if (is_pref) {
					is_pref = R[j][i] = C[(i+1+j)%31];
				} else {
					R[j][i] = M[idx++];
				}
			}
		}
	}
	assert(idx == int(M.size()));
	for (auto p : R) send_packet(p);
}

std::vector<bool> receive_message(std::vector<std::vector<bool>> R) {
	std::vector<int> nxt(31);
	for (int i = 0; i < 31; i++) {
		int j = 0; while (j < 30 && R[j][i] == 1) j++;
		nxt[i] = (i+j+1)%31;
	}
	std::vector<bool> C;
	for (int i = 0; true; i++) {
		assert(i <= 15);
		std::vector<bool> C_cnd(31, 1);
		bool good = true;
		int cur = i;
		for (int z = 0; z < 16; z++) {
			C_cnd[cur] = 0;
			cur = nxt[cur];
			if ((z == 15) != (cur == i)) {
				good = false;
				break;
			}
		}
		if (good) {
			C = C_cnd;
			break;
		}
	}

	std::vector<bool> M(1025);

	int idx = 0;
	for (int i = 0; i < 31; i++) {
		if (C[i] == 0) {
			bool is_pref = true;
			for (int j = 0; j < 66; j++) {
				if (is_pref) {
					assert(R[j][i] == C[(i+1+j)%31]);
					is_pref = R[j][i];
				} else {
					M[idx++] = R[j][i];
				}
			}
		}
	}
	assert(idx == int(M.size()));

	while (true) {
		bool b = M.back();
		M.pop_back();
		if (b == 1) break;
	}
	return M;
}
