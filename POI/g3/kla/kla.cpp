#include<bits/stdc++.h>
using namespace std;

int N;
char S;
char T;

int get_ring_start(int r) {
	r --;
	// 0 rings
	return N * N - (N - 2 * r) * (N - 2 * r);
}

// assuming north, right
int get_spiral(int x, int y) {
	if (x <= y && y <= N + 1 - x) {
		// left
		//cerr << "left" << '\n';
		int ring = x;
		int st = get_ring_start(ring) + 0 * (N + 1 - 2 * ring);
		return st + (y - x) + 1;
	} else if (N + 1 - y <= x && x <= y) {
		// top
		//cerr << "top" << '\n';
		int ring = N + 1 - y;
		int st = get_ring_start(ring) + 1 * (N + 1 - 2 * ring);
		return st + (x - (N + 1 - y)) + 1;
	} else if (N + 1 - x <= y && y <= x) {
		// right
		//cerr << "right" << '\n';
		int ring = N + 1 - x;
		int st = get_ring_start(ring) + 2 * (N + 1 - 2 * ring);
		return st + ((N + 1 - y) - (N + 1 - x)) + 1;
	} else if (y <= x && x <= N + 1 - y) {
		// bot
		//cerr << "bot" << '\n';
		int ring = y;
		int st = get_ring_start(ring) + 3 * (N + 1 - 2 * ring);
		return st + ((N + 1 - x) - y) + 1;
	} else assert(false);
	return -1;
}

// using s and t
int get_grid(int x, int y) {
	int a, b;
	switch(S) {
		case 'N': a = x, b = y; // north
				  break;
		case 'E': a = N+1-y, b = x; // north
				  break;
		case 'S': a = N+1-x, b = N+1-y; // north
				  break;
		case 'W': a = y, b = N+1-x; // north
				  break;
		default: assert(false);
	}
	switch(T) {
		case 'R': return get_spiral(a, b); // right
		case 'L': return get_spiral(N+1-a, b); // left
	}
	return -1;
}


int main() {
	string s;
	string t;
	cin >> N >> s >> t;
	if (s == "POLNOC") {
		S = 'N';
	} else if (s == "POLUDNIE") {
		S = 'S';
	} else if (s == "ZACHOD") {
		S = 'W';
	} else if (s == "WSCHOD") {
		S = 'E';
	} else assert(false);

	if (t == "PRAWO") {
		T = 'R';
	} else if (t == "LEWO") {
		T = 'L';
	} else assert(false);

	int x1, x2, y1, y2;
	cin >> x1 >> x2 >> y1 >> y2;
	for (int y = y2; y >= y1; y--) {
		for (int x = x1; x <= x2; x++) {
			printf("%d", get_grid(x, y));
			printf("%c", " \n"[x == x2]);
		}
	}

	return 0;
}
