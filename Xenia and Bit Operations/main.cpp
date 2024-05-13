#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

class SegTree{
public:
	int n;
    int size;
	vector<int> seg;
	vector<int> arr;
	
	void build(int ind, int low, int high, bool orr){
		if(low==high){
			seg[ind] = arr[low];
			return;
		}
		int mid = low + (high-low)/2;
		build(2*ind+1,low,mid,!orr);
		build(2*ind+2,mid+1,high,!orr);
		if(orr) seg[ind] = seg[2*ind+1] | seg[2*ind+2];
		else seg[ind] = seg[2*ind+1] ^ seg[2*ind+2];
	}

public:
	SegTree(vector<int>& nums, int _n){
		n = _n;
		arr = nums;
        size = nums.size();
		seg.resize(4*size);
		if(n%2==0) build(0,0,size-1,false);
		else build(0,0,size-1,true);
	}
	
	void update(int ind, int low, int high, bool orr, int i, int val){
		if(low==high){
            seg[ind] = val;
            return;
        }
        int mid = low + (high-low)/2;
		if(i<=mid) update(2*ind+1,low,mid,!orr,i,val);
		else update(2*ind+2,mid+1,high,!orr,i,val);
		if(orr) seg[ind] = seg[2*ind+1] | seg[2*ind+2];
		else seg[ind] = seg[2*ind+1] ^ seg[2*ind+2];
	}

    int updateAndGetVal(int i, int val){
        if(n%2==0) update(0,0,size-1,false,i,val);
        else update(0,0,size-1,true,i,val);
        return seg[0];
    }
};

int main() {
	int n, m;
	cin>>n>>m;
    int size = pow(2,n);
	vector<int> arr(size);
    for(int i=0;i<size;i++) cin>>arr[i];
    SegTree st(arr,n);
    while(m--){
        int i, val;
        cin>>i>>val;
        cout<<st.updateAndGetVal(i-1,val)<<endl;
    }
	return 0;
}