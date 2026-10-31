#include<bits/stdc++.h>
using namespace std;

ifstream in("input.bin", ios::in | ios::binary);
int readInt() {
	char c[4];
	in.read(c, 4);
	int v = (uint8_t(c[0]) << 0) | (uint8_t(c[1]) << 8) | (uint8_t(c[2]) << 16) | (uint8_t(c[3]) << 24);
	//cerr << "Input " << v << '\n';
	return v;
}
ofstream out("output.bin", ios::out | ios::binary);
void writeInt(int v) {
	//cerr << "Output " << v << '\n';
	out << char((v >> 0) & 255) << char((v >> 8) & 255) << char((v >> 16) & 255) << char((v >> 24) & 255);
}

const int MAXV = 2.1e6;
const int MAXN = 1.1e6;
int N;
stack<int> curFree;
int finalSlots[MAXN];
int V;
int finalLoc[MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	N = readInt();
	V = 0;
	for (int i = 1; i <= N; i++) {
		finalSlots[i] = readInt();
		if (finalSlots[i]) {
			V = max(V, finalSlots[i]);
			finalLoc[finalSlots[i]] = i;
		}
	}

	if (V == 0) {
		writeInt(0);
		exit(0);
	}

	assert(finalLoc[V]);

	int nextOpen = 1;
	vector<int> ans;
	for (int v = 1; v <= V; v++) {
		//cerr << v << ' ' << "finalLoc" << ' ' << finalLoc[v] << '\n';
		//cerr << nextOpen << '\n';
		if (!finalLoc[v]) {
			if (nextOpen <= N) {
				// assign it in the next open one
				nextOpen++;
			} else {
				// might as well clear out the last one
				ans.push_back(finalLoc[V]);
				//cerr << "ans " << finalLoc[V] << '\n';
			}
			ans.push_back(0);
			//cerr << "ans " << 0 << '\n';
		} else {
			if (finalLoc[v] == nextOpen) {
				// do nothing
				nextOpen++;
			} else if (finalLoc[v] < nextOpen) {
				ans.push_back(finalLoc[v]);
				//cerr << "ans " << finalLoc[v] << '\n';
			} else {
				writeInt(-1);
				exit(0);
			}
			ans.push_back(0);
			//cerr << "ans " << 0 << '\n';
		}
	}

	for (int i = 1; i < nextOpen; i++) {
		if (!finalSlots[i]) {
			ans.push_back(i);
		}
	}

	writeInt(int(ans.size()));
	for (int i : ans) {
		writeInt(i);
	}

	return 0;
}
