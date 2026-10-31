#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;
typedef long long ll;
const int MAXN = 410;

int N, K;
int X[MAXN], Y[MAXN];

int cd (int i, int j)
{
    int xd = X[i] - X[j], yd = Y[i] - Y[j];
    return xd * xd + yd * yd;
}

bool works (int ind)
{
    int nt = 2 * ind + 2 * N;
    for (int i = ind + N - 1; i >= ind; i--)
    {
        int j = nt - i;
        if (cd (i, ind) == cd (j, ind) &&
            cd (i, ind + N) == cd (j, ind + N))
            continue;
        else
        {
            //cout << ind << " " << i << "\n";
            return false;
        }
    }
    return true;
}

void op (int x)
{
    if (x % 2 == 0)
        cout << x/2 << "/1";
    else
        cout << x << "/2";
}

void gogo()
{
    cin >> N >> K;
    for (int i = 0; i < N; i++)
    {
        cin >> X[2*i] >> Y[2*i];
    }
    X[2*N] = X[0];
    Y[2*N] = Y[0];
    for (int i = 1; i < 2 * N; i += 2)
    {
        X[i] = X[i-1] + X[i+1];
        Y[i] = Y[i-1] + Y[i+1];
    }
    for (int i = 0; i <= 2 * N; i += 2)
    {
        X[i] *= 2;
        Y[i] *= 2;
    }
    for (int i = 2 * N; i <= 4 * N; i++)
    {
        X[i] = X[i-2*N];
        Y[i] = Y[i-2*N];
    }

    for (int i = 0; i <= 2 * N; i++)
    {
        if (works (i))
        {
            cout << "POSSIBLE\n";
            op(X[i]);
            cout << " ";
            op(Y[i]);
            cout << " ";
            op(X[i+N]);
            cout << " ";
            op(Y[i+N]);
            cout << "\n";
            return;
        }
    }
    cout << "IMPOSSIBLE\n";
}

int main()
{
    ios_base::sync_with_stdio(0);

    int T;
    cin >> T;
    for (int i = 0; i < T; i++)
    {
        cout << "Case #" << i + 1 << ": ";
        gogo();
    }
}