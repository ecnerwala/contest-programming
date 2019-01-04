#include "grader.h"
#include<bits/stdc++.h>

namespace ecnerwala {

using std::min;
using std::max;

void go_small(int N, int K, int I, int P, int C) {
    assert(K <= 2500);
    // We don't need the second pass
    if (P) return;

    const int HEAD_IND = 5498;
    const int TAIL_IND = 5499;

    const int MMOD = 5300;
    int h, t;
    if (I == 0) {
        // initialize
        h = 0, t = 0;
    } else {
        h = get(HEAD_IND);
        t = get(TAIL_IND);
    }

    // remove I-K
    if (I >= K) {
        assert(0 <= h && h + 1 < MMOD);
        if (h != t && get(h+1) == I-K) {
            h += 2;
            if (h >= MMOD) h -= MMOD;
        }
    }

    // insert I
    while (h != t && get((t-2+MMOD) % MMOD) >= C) {
        t -= 2;
        if (t < 0) t += MMOD;
    }
    set(t, C);
    set(t+1, I);
    t += 2;
    if (t >= MMOD) t -= MMOD;
    assert(t != h);

    assert(0 <= h && h+1 < MMOD);
    assert(0 <= t && t+1 < MMOD);

    set(HEAD_IND, h);
    set(TAIL_IND, t);

    if (I+1 >= K) {
        shoutMinimum(get(h));
    }
}

void go(int N, int K, int I, int P, int C) {
    if (K <= 2500) {
        go_small(N, K, I, P, C);
        return;
    }

    assert(K > 2000);

    int B = I / 1000;
    assert(0 <= B && B < 1000);
    int J = I % 1000;

    if (P == 0) {
        if (J == 0) {
            set(B, C);
        } else {
            set(B, min(get(B), C));
        }
        return;
    }

    assert(P == 1);

    int nmid = (K-1) / 1000 - 1;
    assert(nmid >= 1);
    int nwide = (K - 1) % 1000;
    assert(0 <= nwide && nwide < 1000);

    if (P == 1 && I == 0) {
        // do mid-processing
        int A[1000];
        for (int i = 0; i < 1000; i++) {
            A[i] = get(i);
        }
        for (int i = 0; i + nmid + 1 < 1000; i++) {
            int v = A[i+1];
            assert(i + nmid < 1000);
            for (int j = i + 1; j <= i + nmid; j++) {
                v = min(v, A[j]);
            }
            assert(i + nmid + 1 < 1000);
            if (A[i] < v && A[i + nmid + 1] < v) {
                // B[i] = both
            }

            if (nwide && i + nmid + 2 < 1000) {
                assert(i + nmid + 2 < 1000);
                v = min(v, A[i + nmid + 1]);
                if (A[i + nmid + 2] < v) {
                }
            }
        }
    }

    // second pass through
}

}

void helpBessie(int ID) {
    int N = getTrainLength(), K = getWindowLength();
    int I = getCurrentCarIndex(), P = getCurrentPassIndex();
    ecnerwala::go(N, K, I, P, ID);
}
