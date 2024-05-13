#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node{
    int open;
    int close;
    int complete;

    Node(){
        open = 0;
        close = 0;
        complete = 0;
    }

    Node(int o, int c, int com){
        open = o;
        close = c;
        complete = com;
    }
};

Node mergeNodes(Node a, Node b){
    Node merged;
    merged.complete = min(a.open,b.close) + a.complete + b.complete;
    merged.open = a.open + b.open - min(a.open,b.close);
    merged.close = a.close + b.close - min(a.open,b.close);
    return merged;
}

class SGTree{
private:
    vector<Node> seg;
    string str;
    int size;

    void build(int ind, int low, int high){
        if(low==high){
            seg[ind] = Node(str[low]=='(', str[low]==')', 0);
            return;
        }
        int mid = low +(high-low)/2;
        build(2*ind+1,low,mid);
        build(2*ind+2,mid+1,high);
        seg[ind] = mergeNodes(seg[2*ind+1],seg[2*ind+2]);
    }

    Node query(int ind, int low, int high, int l, int r){
        // no overlap
        if(high<l || low>r) return Node(0,0,0);
        // complete overlap
        if(low>=l && high<=r) return seg[ind];
        // partial overlap
        int mid = low + (high-low)/2;
        Node leftNode = query(2*ind+1,low,mid,l,r);
        Node rightNode = query(2*ind+2,mid+1,high,l,r);
        return mergeNodes(leftNode, rightNode);
    }
public:
    SGTree(string s){
        size = s.size();
        str = s;
        seg.resize(size*4);
        build(0,0,size-1);
    }
    int getCompleteBrackets(int l, int r){
        return query(0,0,size-1,l,r).complete*2;
    }
};

int main(){
    string s;
    getline(cin,s);
    SGTree st(s);
    int m;
    cin>>m;
    while(m--){
        int l,r;
        cin>>l>>r;
        cout<<st.getCompleteBrackets(l-1,r-1)<<endl;;
    }
}