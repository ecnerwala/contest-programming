#include<bits/stdc++.h>
using namespace std;

int main() {
    int T = 100;
    cout << T << '\n';
    for (int t = 1; t <= T; t++) {
        int R = (t <= 10 ? 100 : 50);
        int C = (t <= 10 ? 100 : 50);
        cout << R << ' ' << C << '\n';
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                if (i == 0 && j == 0) {
                    cout << 'N';
                } else if (i == R-1 && j == C-1) {
                    cout << 'M';
                } else {
                    cout << '.';
                }
            }
            cout << '\n';
        }
    }
}
