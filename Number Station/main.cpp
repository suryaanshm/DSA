#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

class IPrinter
{
public:
    virtual ~IPrinter() = default;
    virtual void OnMessageComplete(const std::string &message) const = 0;
};

class Printer
    : public IPrinter
{
public:
    void OnMessageComplete(const std::string &message) const override
    {
        std::cout << message << std::endl;
    }
};

class Decoder
{
private:
    long long latestCompletedSeqID = -1;
    set<long long> s;
    unordered_map<long long, char> m;
public:
    Decoder(const IPrinter &printer)
        : mPrinter(printer)
    {
    }

    void ProcessSample(const uint64_t sequence, const char character)
    {
        if(character=='-'){
            if (s.empty()){
                s.insert(sequence);
                return;
            }
            auto itNext = s.upper_bound(sequence);
            bool isNextComplete = true;
            if(itNext!=s.end()){
                int indNext = *itNext;
                string temp = "";
                for(int i=sequence+1; i<indNext;i++){
                    if(m.find(i)!=m.end()){
                        temp += m[i];
                    }
                    else{
                       isNextComplete = false;
                       break; 
                    } 
                }
                if(isNextComplete){
                    if(indNext-1>latestCompletedSeqID){
                        latestCompletedSeqID = indNext-1;
                        mPrinter.OnMessageComplete(temp);
                    }
                }
            }
            if(!isNextComplete && itNext!=s.begin()){
                auto itPrev = itNext--;
                int indPrev = *itPrev;
                string temp = "";
                bool isPrevComplete = true;
                for(int i=indPrev+1; i<sequence;i++){
                    if(m.find(i)!=m.end()){
                        temp += m[i];
                    }
                    else{
                       isPrevComplete = false;
                       break; 
                    } 
                }
                if(isPrevComplete){
                    if(sequence-1>latestCompletedSeqID){
                        latestCompletedSeqID = sequence-1;
                        mPrinter.OnMessageComplete(temp);
                    }
                }
            }
            s.insert(sequence);
        }
        else{
            m[sequence] = character;
        }
    }

    const IPrinter &mPrinter;
};

int main()
{
    Printer printer;
    Decoder decoder(printer);

    uint64_t seqNum;
    char message;

    int n;
    cin>>n;
    while (n--)
    {
        std::cin >> seqNum >> message;
        decoder.ProcessSample(seqNum, message);
    }

    return 0;
}