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
const int N = 1e6 + 100;
int n, t, sum_x, sum_y,m,x,y,arr[25][25],allmask;
ll dp[N], ans;
vector<int> subList[25];

//if ith bit of mask is set than this means that for some student j with index < (no. of set bit in mask) must have liked that subject
//with subject_index i and we did the assignment of this subject to that student represented in the mask

ll countWays(int mask){
	//Find the number of set bits in the mask and name it as index
	int index = __builtin_popcount(mask);
	//if every student had choosen a subject
	if (index == n && mask == allmask)
		return 1;
	if (dp[mask] != -1)
		return dp[mask];
	ll ways = 0;
	//Assign subject 'j' to each student who had liked the subject and reccur for rest
	for (int j = 0; j < n; j++){
		//if the student number index does not like the subject j then we cannot do the assignment
		if (!arr[index][j])
			continue;
		//if we had already assigned a subject j to a student in this mask then we cannot do the assignment
		if (mask & (1 << j))
			continue;
		//If the subject no. j is not assigned to any student and student number index likes the subject than we assign it to the student
		else
			ways += countWays(mask | (1 << j));
	}
	return dp[mask] = ways;
}
int main(){
	scanf("%d", &t);
	while (t--){
		memset(dp, -1, sizeof (dp));
		for (int i = 0; i < 25; i++)
			subList[i].clear();
		scanf("%d", &n);
		allmask = pow(2, n) - 1;
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				scanf("%d", &arr[i][j]);
				if (arr[i][j])
					subList[j].push_back(i);//student i likes subject j i.e. subject j is liked by student i
			}
		}
		ans = countWays(0);
		printf("%llu\n", ans);
	}
	return 0;
}
