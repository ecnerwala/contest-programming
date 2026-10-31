#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

using namespace std;

int conta(int N, vector<int> &A, vector<int> &P, vector<int> &D);

int main() {
    // Input da file:
    // freopen("input.txt", "r", stdin);

    // Output su file:
    // freopen("output.txt", "w", stdout);

    int N;
    assert(scanf("%d", &N) == 1);

    vector<int> A(N), P(N), D(N);
    for (int &a : A)
        assert(scanf("%d", &a) == 1);
    for (int &p : P)
        assert(scanf("%d", &p) == 1);
    for (int &s : D)
        assert(scanf("%d", &s) == 1);

    printf("%d\n", conta(N, A, P, D));
    return EXIT_SUCCESS;
}
