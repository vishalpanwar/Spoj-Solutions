#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<cstdlib>
#include<iomanip>
#include<map>
#include<vector>
#include<list>
#include<set>
#include<queue>
#include<stack>
#include<cassert>
#include<sstream>
#include<string>
#include<cmath>
#include<algorithm>
#include<stdio.h>
#include<functional>
#include<bitset>

#define fast std::ios::sync_with_stdio(false);std::cin.tie(false)
#define endl "\n"
#define FOR(i,a,b) for(ll i=(ll)(a) ; i < (ll)(b);++i)
#define rep(i,n) FOR(i,0,n)
#define max(a,b) (int)(a) > (int)(b) ? a : b
#define min(a,b) (int)(a) < (int)(b) ? a : b
#define tr(c,it) for(typedef c.begin()::iterator it = c.begin();it!=c.end();it++)
#define PB push_back
#define MP make_pair
#define INF 1e+9
#define EPS 1e-9
#define gc getchar//_unlocked
#define pc putchar//_unlocked
typedef long long ll;
ll mod = 1e9 + 7;
ll powmod(ll a, int b) { ll res = 1; if (a >= mod)a %= mod; for (; b; b >>= 1){ if (b & 1)res = res*a; if (res >= mod)res %= mod; a = a*a; if (a >= mod)a %= mod; }return res; }
ll gcd(ll a, ll b){ return b == 0 ? a : gcd(b, a%b); }
const ll MAX = 200002;
using namespace std;
ll bit[MAX], arr[MAX], temp[MAX], n;

//----------------------------------------BIT CODE************************************************************************************
void update(int ind, int val){
	while (ind <= n){
		bit[ind] += val;
		ind += ind & (-ind);
	}
}
ll get(int ind){
	ll sum = 0;
	while (ind >= 1){
		sum += bit[ind];
		ind -= ind & (-ind);
	}
	return sum;
}
void construct(){
	for (int i = 1; i < n; i++)
		update(n, i);
}
//***************************************************************BIT CODE------------------------------------------------------------------

ll b_search(ll val){
	int start = 1, end = n;
	while (start <= end){
		int mid = (start + end) >> 1;
		if (temp[mid] == val)return mid;
		else if (temp[mid] > val)end = mid - 1;
		else start = mid + 1;
	}
}
int main(int argv, char** args)
{
	ll t;
	cin >> t;
	while (t--){
		ll sum = 0;
		cin >> n;
		for (int i = 1; i <= n; i++)bit[i] = 0;
		for (int i = 1; i <= n; i++){
			cin >> arr[i];
			temp[i] = arr[i];
		}
		sort(temp + 1, temp + n + 1);
		//for (int i = 1; i <= n; i++)cout << temp[i] << " "; cout << endl; for (int i = 1; i <= n; i++)cout << arr[i] << " "; cout << endl;
		for (ll i = 1; i <= n; i++){
			ll pos = b_search(arr[i]);
			//cout << arr[i] << "  " << pos << endl;
			sum += get(n) - get(pos);
			update(pos, 1);
		}
		cout << sum << endl;
	}
	return 0;
}
