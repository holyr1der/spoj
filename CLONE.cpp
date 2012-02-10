#include <iostream>
#include <map>
#include <string>
#include <cstring>

using namespace std;

int main(){
 int n,m;
 string d;
 map<string, int> mp;
 map<string, int>::iterator it;
 for (;;){
 mp.clear();
 cin>>n>>m;
 if (n == 0 && m == 0) {break;}
 int *res = new int[n];
 memset(res, 0, n * sizeof(int));
 for (int i = 0;i < n;++i){
  cin>>d;
  it = mp.find(d);
  if (it == mp.end()){
   mp[d] = 0;
   res[0]++;
  }
  else {
   res[it->second]--;
   it->second++;
   res[it->second]++;
  }
 }
 for (int i = 0;i < n;++i){
  cout<<res[i]<<endl;
 }
 }
 return 0;
}
