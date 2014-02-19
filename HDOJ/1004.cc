#include<map>
#include<vector>
#include<algorithm>
#include<string>
#include<iostream>

using namespace std;

bool cmp(pair<string, int>& a,pair<string, int>& b)
{
    if(a.second < b.second)
        return true;
    else
        return false;
}

int main()
{
    int N;
    map<string, int> mp;
    vector<pair<string, int> > v;
    string str;
    while( cin>>N && N){
        v.clear();
        mp.clear();
        for(int i=0; i<N; i++){
            cin>>str;
            map<string, int>::const_iterator it = mp.find(str);
            if(it != mp.end())
                mp[str]++;
            else
                mp[str] = 0;
        }
        for(map<string, int>::iterator it=mp.begin(); it!=mp.end(); ++it)
            v.push_back(*it);
        sort(v.begin(),v.end(), cmp);
        cout<< (v.end()-1)->first <<endl;
    }
    return 0;
}
