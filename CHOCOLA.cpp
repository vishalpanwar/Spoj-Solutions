#include<bits/stdc++.h>
typedef long long ll;
#define fast std::ios::sync_with_stdio(false);std::cin.tie(false)
#define endl "\n"
//#define abs(a) a >= 0 ? a : -a
#define ll long long
#define mod 1000000007
#define Endl endl
using namespace std;
ll powmod(ll a, int b) { ll res = 1; if (a >= mod)a %= mod; for (; b; b >>= 1){ if (b & 1)res = res*a; if (res >= mod)res %= mod; a = a*a; if (a >= mod)a %= mod; }return res; }
ll gcd(ll a, ll b){ if (a < b)std::swap(a, b); return b == 0 ? a : gcd(b, a % b); }
ll lcm(ll a, ll b){ return ((a*b) / gcd(a, b)); }
int l_bound(int *arr, int lo, int hi, int val){ while (lo <= hi){ int mid = (lo + hi) >> 1; if (arr[mid] >= val)hi = mid - 1; else lo = mid + 1; }return lo; }
int u_bound(int *arr, int lo, int hi, int val){ while (lo <= hi){ int mid = (lo + hi) >> 1; if (arr[mid] > val)hi = mid - 1; else lo = mid + 1; }return hi; }
int bsearch(int *arr, int n, int val){ int lo = 0, hi = n - 1; while (hi >= lo){ int mid = (hi + lo) >> 1; if (arr[mid] == val)return mid; if (arr[mid] > val)hi = mid - 1; else lo = mid + 1; }return -1; }
const int N = 1000 + 100;
int n, t, sum_x, sum_y,m,ans,x,y,dp[N][N];
vector<int> v_x, v_y;
int main(){
	scanf("%d", &t);
	while (t--){
		v_x.clear();
		v_y.clear();
		scanf("%d %d", &m, &n);
		for (int i = 0; i < m-1; i++){
			scanf("%d", &x);
			v_x.push_back(x);
		}
		for (int j = 0; j < n-1; j++){
			scanf("%d", &y);
			v_y.push_back(y);
		}
		--m;//actual no. of vertical cuts
		--n;//actual no. of vertical cuts

		//Now we will sort both the vectors as we want to cut the cut with max weight first as it would then incurr min cost.
		sort(v_x.rbegin(), v_x.rend());
		sort(v_y.rbegin(), v_y.rend());

		dp[0][0] = 0; //Case when both m and n are 1 i.e. chocolate already contains a single piece
		for (int i = 1; i <= m; i++)dp[i][0] = dp[i - 1][0] + v_x[i-1]; //We can only cut the chocolate vertically
		for (int j = 1; j <= n; j++)dp[0][j] = dp[0][j - 1] + v_y[j-1]; //We can only cut the chocolate horizontally
		int vcut, hcut;
		for (int i = 1; i <= m; i++){
			for (int j = 1; j <= n; j++){
				vcut = dp[i - 1][j] + v_x[i-1] * (j + 1);//we cut the last column vertically (j+1) i.e. no. of horizontal cuts plus 1 times.
				hcut = dp[i][j - 1] + v_y[j-1] * (i + 1);//we cut the last row horizontally (i+1) i.e. no. of vertical cuts plus 1 times.
				dp[i][j] = min(vcut, hcut);
			}
		}
		printf("%d\n", dp[m][n]);
	}
}
