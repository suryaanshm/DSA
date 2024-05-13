// https://leetcode.com/discuss/interview-question/357982/Optiver-or-OA-or-Summer-SWE-Intern-2020
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int main(){
    string s;
    getline(cin,s);
    unordered_map<string, vector<string>> adj;
    unordered_set<string> hasParents;
    unordered_set<string> nodes;
    int edgesCount = 0;
    for(int i=0;i<s.size();i++){
        string parent = "";
        string child = "";
        i+=1; // for (
        parent = s[i++];
        i+=1; // for ,
        child = s[i++];
        i+=1; // for ) and space
        if(!isupper(parent[0]) || !isupper(child[0])){
            cout<<"E1"<<endl;
            return 0;
        }
        auto it = find(adj[parent].begin(), adj[parent].end(), child);
        if(it!=adj[parent].end()){
            cout<<"E2"<<endl;
            return 0;
        }
        adj[parent].push_back(child);
        hasParents.insert(child);
        nodes.insert(parent);
        nodes.insert(child);
        edgesCount+=1;
    }
    for(auto it : adj){
        string parent = it.first;
        auto children = it.second;
        if(children.size()>2){
            cout<<"E3"<<endl;
            return 0;
        }
    }
    if(nodes.size()-hasParents.size()>1){
        cout<<"E4"<<endl;
        return 0;
    }
    if(edgesCount > nodes.size()-1){
        cout<<"E5"<<endl;
        return 0;
    }
    cout<<endl;
    return 0;
}