#include <bits/stdc++.h>
#include "flipbits.h"

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef long double ld;

#define fi first
#define se second
#define pb push_back
#define mp make_pair

void initialize()
{
	
}

int guess(char S[])
{
  int c = 0,k = 19;
  for(int i=0;i<200;i++)
    if(S[i]=='1')c++;
  if(100-k<=c && c<=100+k)return 1;
  if(c%2) return 1;
  return 2;
}


