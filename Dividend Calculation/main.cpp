#include <vector>
#include <iostream>

using namespace std;

class SGTree{
private:
    vector<int> seg;
    vector<int> lazy;
    int size;
public:
    SGTree(){
        size = (int)(1e6+1);
        seg.resize(4*size,0);
        lazy.resize(4*size,0);
    }

    void updateDividend(int amount, int day){
        updateRange(0,0,size-1,day,size-1,-amount);
    }

    int getPrice(int day){
        if(day>size-1){
            return query(0,0,size-1,size-1,size-1);
        }
        return query(0,0,size-1,day,day);
    }
private:
    void updateRange(int ind, int low, int high, int l, int r, int val){
        // update previous lazy
        if(lazy[ind]!=0){
            seg[ind] += (high-low+1)*lazy[ind];
            if(low!=high){
                lazy[2*ind+1] += lazy[ind];
                lazy[2*ind+2] += lazy[ind];
            }
            lazy[ind]=0;
        }
        // no overlap
        if(high<l || high>r) return;
        // complete overlap
        if(low>=l && high<=r){
            seg[ind] += (high-low+1)*val;
            if(low!=high){
                lazy[2*ind+1] += val;
                lazy[2*ind+2] += val;
            }
            return;
        }
        // partial overlap
        int mid = low + (high-low)/2;
        updateRange(2*ind+1,low,mid,l,r,val);
        updateRange(2*ind+2,mid+1,high,l,r,val);
        seg[ind] = seg[2*ind+1] + seg[2*ind+2];
    }

    int query(int ind, int low, int high, int l, int r){
        // update previous lazy
        if(lazy[ind]!=0){
            seg[ind] += (high-low+1)*lazy[ind];
            if(low!=high){
                lazy[2*ind+1] += lazy[ind];
                lazy[2*ind+2] += lazy[ind];
            }
            lazy[ind]=0;
        }
        // no overlap
        if(high<l || low>r) return 0;
        // complete overlap
        if(low>=l && high<=r) return seg[ind];
        // partial overlap
        int mid = low + (high-low)/2;
        int leftAns = query(2*ind+1,low,mid,l,r);
        int rightAns = query(2*ind+2,mid+1,high,l,r);
        return leftAns + rightAns;
    }

};

int main(){
    int s,n,q;
    cin>>s>>n>>q;
    vector<int> amounts(n);
    vector<int> days(n);
    int maxDay = -1;
    for(int i=0;i<n;i++){
        cin>>amounts[i];
        cin>>days[i];
    }
    SGTree st;
    for(int i=0;i<n;i++){
        st.updateDividend(amounts[i], days[i]);
    }
    while(q--){
        int type;
        cin>>type;
        // get price
        if(type==1){
            int day;
            cin >> day;
            cout<<s + st.getPrice(day)<<endl;
        }
        // update dividend
        if(type==2){
            int i,a,d;
            cin>>i>>a>>d;
            st.updateDividend(-1*amounts[i], days[i]);
            st.updateDividend(a,d);
            amounts[i]=a;
            days[i]=d;
        }
    }
}