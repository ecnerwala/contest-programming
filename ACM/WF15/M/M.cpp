#include<bits/stdc++.h>
using namespace std;

struct rect {
	int xlo, xhi, ylo, yhi;
};

int signum(int a) { return (a > 0) - (a < 0); }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int X, Y; cin >> X >> Y;
	vector<rect> windows;

	string OP;
	int commandNum = 1;
	for (; cin >> OP; commandNum++) {
		auto err = [&]() -> ostream& {
			return cout << "Command " << commandNum << ": " << OP << " - ";
		};

		auto doesNotFit = [&]() {
			err() << "window does not fit" << '\n';
		};
		auto noWindow = [&]() {
			err() << "no window at given position" << '\n';
		};
		auto moved = [&](int actual, int orig) {
			assert(0 <= actual && actual <= orig);
			if (actual < orig) {
				err() << "moved " << actual << " instead of " << orig << '\n';
			}
		};

		auto findWindow = [&](int x, int y) -> int {
			for (int i = 0; i < int(windows.size()); i++) {
				rect r = windows[i];
				if (r.xlo <= x && x < r.xhi && r.ylo <= y && y < r.yhi) {
					return i;
				}
			}
			return -1;
		};

		auto intersects = [&](rect a, rect r) -> bool {
			return max(a.xlo, r.xlo) < min(a.xhi, r.xhi) && max(a.ylo, r.ylo) < min(a.yhi, r.yhi);
		};

		auto checkFits = [&](rect a, int x) -> bool {
			if (a.xlo < 0 || a.ylo < 0 || a.xhi > X || a.yhi > Y) {
				return false;
			}
			for (int i = 0; i < int(windows.size()); i++) {
				if (i == x) continue;
				if (intersects(a, windows[i])) {
					return false;
				}
			}
			return true;
		};

		auto go = [&]() {
			if (OP == "OPEN") {
				int xlo, ylo, xhi, yhi; cin >> xlo >> ylo >> xhi >> yhi;
				xhi += xlo, yhi += ylo;
				rect n{xlo, xhi, ylo, yhi};
				if (!checkFits(n, -1)) {
					doesNotFit();
					return;
				}
				windows.push_back(n);
			} else if (OP == "CLOSE") {
				int x, y; cin >> x >> y;
				int i = findWindow(x, y);
				if (i == -1) {
					noWindow();
					return;
				}

				windows.erase(windows.begin() + i);
			} else if (OP == "RESIZE") {
				int x, y; cin >> x >> y;
				int w, h; cin >> w >> h;
				int i = findWindow(x, y);
				if (i == -1) {
					noWindow();
					return;
				}

				rect n = windows[i];
				n.xhi = n.xlo + w;
				n.yhi = n.ylo + h;
				if (!checkFits(n, i)) {
					doesNotFit();
					return;
				}

				windows[i] = n;
			} else if (OP == "MOVE") {
				int x, y; cin >> x >> y;
				int dx, dy; cin >> dx >> dy;
				int i = findWindow(x, y);
				if (i == -1) {
					noWindow();
					return;
				}

				const int INF = X+Y+1;
				int orig = abs(dx) + abs(dy);
				int actual = 0;
				auto getAMove = [&](rect a, rect b) -> int {
					if (dx < 0) {
						if (max(a.ylo, b.ylo) < min(a.yhi, b.yhi) && b.xhi <= a.xlo) {
							return a.xlo - b.xhi;
						}
					} else if (dx > 0) {
						if (max(a.ylo, b.ylo) < min(a.yhi, b.yhi) && a.xhi <= b.xlo) {
							return b.xlo - a.xhi;
						}
					} else if (dy < 0) {
						if (max(a.xlo, b.xlo) < min(a.xhi, b.xhi) && b.yhi <= a.ylo) {
							return a.ylo - b.yhi;
						}
					} else if (dy > 0) {
						if (max(a.xlo, b.xlo) < min(a.xhi, b.xhi) && a.yhi <= b.ylo) {
							return b.ylo - a.yhi;
						}
					} else assert(false);
					return INF;
				};

				vector<bool> toMove(windows.size());
				toMove[i] = true;
				while (orig > actual) {
					int legalAmt = orig - actual;
					for (int a = 0; a < int(windows.size()); a++) {
						if (!toMove[a]) continue;
						legalAmt = min(legalAmt, getAMove(windows[a], rect{0, X, -1, 0}));
						legalAmt = min(legalAmt, getAMove(windows[a], rect{0, X, Y, Y+1}));
						legalAmt = min(legalAmt, getAMove(windows[a], rect{-1, 0, 0, Y}));
						legalAmt = min(legalAmt, getAMove(windows[a], rect{X, X+1, 0, Y}));
					}

					if (legalAmt == 0) break; // hit a wall

					//cerr << "legalAmt " << legalAmt << '\n';

					for (int a = 0; a < int(windows.size()); a++) {
						if (!toMove[a]) continue;
						for (int b = 0; b < int(windows.size()); b++) {
							if (toMove[b]) continue;
							int d = getAMove(windows[a], windows[b]);
							legalAmt = min(legalAmt, d);
						}
					}
					actual += legalAmt;
					for (int a = 0; a < int(windows.size()); a++) {
						if (!toMove[a]) continue;
						windows[a].xlo += signum(dx) * legalAmt;
						windows[a].xhi += signum(dx) * legalAmt;
						windows[a].ylo += signum(dy) * legalAmt;
						windows[a].yhi += signum(dy) * legalAmt;
					}
					for (int a = 0; a < int(windows.size()); a++) {
						if (!toMove[a]) continue;
						for (int b = 0; b < int(windows.size()); b++) {
							if (toMove[b]) continue;
							int d = getAMove(windows[a], windows[b]);
							if (!d) {
								toMove[b] = true;
							}
						}
					}
				}
				moved(actual, orig);
			} else assert(false);
		};

		go();

		//cerr << "Finished command " << commandNum << '\n';
		//for (auto r : windows) { cerr << r.xlo << ' ' << r.ylo << ' ' << r.xhi << ' ' << r.yhi << '\n'; }
	}

	cout << windows.size() << " window(s):" << '\n';
	for (auto r : windows) {
		cout << r.xlo << ' ' << r.ylo << ' ' << r.xhi - r.xlo << ' ' << r.yhi - r.ylo << '\n';
	}

	return 0;
}
