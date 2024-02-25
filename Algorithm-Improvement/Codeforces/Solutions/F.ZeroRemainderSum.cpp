#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int n, m, k;
vector<vector<int>> a;
vector<vector<vector<vector<int>>>> dp;

int MOD = -1e9 - 7;

int solve(int i, int j, int cur, int rem) {
    if (i == n) {
        return (rem == 0) ? 0 : MOD; // Is this a valid finish?
    }
    
    if (j == m)
        return solve(i + 1, 0, m/2, rem); // Go to the next line
    
    if (dp[i][j][cur][rem] != INT_MIN) // Seen
        return dp[i][j][cur][rem];
    
    int res = solve(i, j + 1, cur, rem); // Skip
    
    if (cur > 0) {
        int newRem = (rem + a[i][j]) % k; // Remainer after taking
        res = max(res, a[i][j] + solve(i, j + 1, cur - 1, newRem));
    }
    // res is the best we can get if we take or skip
    return dp[i][j][cur][rem] = res; 
}

int main() {
    cin >> n >> m >> k;
    a.assign(n, vector<int>(m));
    dp.assign(n, vector<vector<vector<int>>>(m, vector<vector<int>>(m / 2 + 1, vector<int>(k, INT_MIN))));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> a[i][j];
        }
    }

    int result = solve(0, 0, m/2, 0);
    cout << max(0, result) << endl;
    
    return 0;
}