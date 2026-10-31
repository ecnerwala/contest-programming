#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int MAXB = (1 << 12);

int N;
vector <int> elem;
map <int, string> formula[MAXB];

ll mul[MAXB];

string ez[100100];

string get_exp (int mask)
{
    string res = "";
    for (int i = 0; i < N; i++)
    {
        if (mask & (1 << i))
        {
            if (!res.length())
                res = to_string(elem[i]);
            else
                res = ("(" + res + "*" + to_string(elem[i]) + ")");
        }
    }
    return res;
}

int main()
{
    ios_base::sync_with_stdio(0);

    for (int i = 0; i < 100100; i++)
        ez[i] = "";

    N = 12;
    for (int i = 0; i < N; i++)
        elem.push_back(8 + i);

    for (int i = 0; i < (1 << N); i++)
    {
        mul[i] = 1;
        for (int j = 0; j < N; j++)
            if (i & (1 << j))
                mul[i] *= elem[j];
    }

    for (int i = 0; i < N; i++)
    {
        formula[1<<i][elem[i]] = to_string(elem[i]);
    }

    for (int i = 1; i < (1 << N); i++)
    {
        if (__builtin_popcount(i) > 4) continue;
        for (int j = i; j > i / 2; j = (j - 1) & i)
        {
            for (pair <int, string> pp : formula[i-j])
            {
                for (pair <int, string> pp2 : formula[j])
                {
                    int a = pp.first, b = pp2.first;
                    if (a + b >= 0 && a + b <= 1e5)
                        if (formula[i].find(a+b) == formula[i].end())
                            formula[i][a+b] = "(" + pp.second + "+" + pp2.second + ")";
                    if (a - b >= 0 && a - b <= 1e5)
                        if (formula[i].find(a-b) == formula[i].end())
                            formula[i][a-b] = "(" + pp.second + "-" + pp2.second + ")";
                    if (b - a >= 0 && b - a <= 1e5)
                        if (formula[i].find(b-a) == formula[i].end())
                            formula[i][b-a] = "(" + pp2.second + "-" + pp.second + ")";
                    if (a * b >= 0 && a * b <= 1e5)
                        if (formula[i].find(a*b) == formula[i].end())
                            formula[i][a*b] = "(" + pp.second + "*" + pp2.second + ")";
                }
            }
        }

        for (pair <int, string> pp : formula[i])
        {
            int a = pp.first;
            if (a >= 0 && a < 1e5 && !ez[a].length())
                ez[a] = pp.second;
        }
    }

    int Q; cin >> Q;

    cout << N << "\n";
    for (int i = 0; i < N; i++)
    {
        if (i) cout << " ";
        cout << elem[i];
    }
    cout << "\n";

    for (int q = 0; q < Q; q++)
    {
        int c; cin >> c;
        if (c < 1e5 && ez[c].length())
        {
            cout << ez[c] << "\n";
            continue;
        }

        int ct = 0;
        ll nbest = 1e9;
        int found = 0;
        string res = "";
        for (int i = 0; i < (1 << N); i++)
        {
            if (found) break;
            if (mul[i] > 2 * c + 100 || mul[i] < c / 2 - 100)
                continue;

            ll ndiff = abs (c - mul[i]);
            for (int j = i; j > 0; j = (j - 1) & i)
            {
                if (j == i) continue;
                if (found) break;

                if (ndiff < mul[j] / 2 || ndiff > mul[j] * 100) continue;
                //ct++;
                for (int k = 0; k < N; k++)
                {
                    if (i & (1 << k)) continue;
                    ll nleft = abs (ndiff - elem[k] * mul[j]);
                    if (nleft > 1e5) continue;

                    int nmask = (1 << N) - 1 - i - (1 << k);
                    for (int l = nmask; l > 0; l = (l - 1) & nmask)
                    {
                        if (formula[l].find(nleft) != formula[l].end())
                        {
                            found = 1;

                            string s = get_exp (i - j);
                            string s2 = get_exp (j);
                            if (c > mul[i])
                            {
                                s = "(" + s + "+" + to_string(elem[k]) + ")" + "*" + s2;
                            }
                            else
                            {                                
                                s = "(" + s + "-" + to_string(elem[k]) + ")" + "*" + s2;
                            }
                            ll v = mul[i];
                            if (c > mul[i]) v += elem[k] * mul[j];
                            else v -= elem[k] * mul[j];
                            if (c > v)
                                s = s + "+" + formula[l][nleft];
                            else
                                s = s + "-" + formula[l][nleft];
                            res = s;
                            break;
                        }
                    }
                    ct++;
                    nbest = min (nbest, abs (ndiff - elem[k] * mul[j]));
                    //if (i + j + k == (1 << N) - 1) continue;
                    //s.insert((mul[i] + elem[k]) * mul[j]);
                    //s.insert((mul[i] - elem[k]) * mul[j]);
                }
            }
        }
        assert(found == 1);
        cout << res << "\n";
        //cout << ct << " " << nbest << " " << found <<"\n";
    }
}
