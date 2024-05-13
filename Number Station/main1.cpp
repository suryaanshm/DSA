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
    long long seq = -1;
    unordered_map<long long, char> m;
public:
    Decoder(const IPrinter &printer)
        : mPrinter(printer)
    {
    }

    void ProcessSample(const uint64_t sequence, const char character)
    {
        if(character=='-'){
            string temp = "";
            bool isFullString = true;
            int i = sequence + 1;
            while(m.count(i) && m[i]!='-'){
                if(m.find(i)==m.end()){
                    isFullString = false;
                    break;
                }
                temp += m[i];
                i++;
            }
            if(isFullString && i-1>seq){
                seq = i-1;
                mPrinter.OnMessageComplete(temp);
            }
            if(!isFullString){
                int j = sequence - 1;
                string temp1 = "";
                bool complete = true;
                while(m.count(j) && m[j]!='-'){
                    if(m.find(j)==m.end()){
                        complete = false;
                        break;
                    }
                    temp = m[j] + temp;
                    j--;
                }
                if(complete && sequence-1>seq){
                    seq = sequence-1;
                    mPrinter.OnMessageComplete(temp);
                }
            }
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