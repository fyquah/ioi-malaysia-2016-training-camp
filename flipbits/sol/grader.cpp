#include <bits/stdc++.h>
#include "flipbits.h"

using namespace std;

typedef long long ll;
typedef vector<ll> vi;
typedef pair<ll,ll> ii;
typedef pair<ll, ii> iii;
typedef long double ld;

#define fi first
#define se second
#define pb push_back
#define mp make_pair

const int TTT = 100000;

char cc[200];

int main()
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	freopen("flipbits.txt", "w", stdout);
	initialize();
	for(int i = 0; i < TTT; i++)
	{
		for(int j = 0; j < 200; j++)
		{
			cin >> cc[j];
		}
		cout << guess(cc) << '\n';
	}
	return 0;
}

