// ****************************************************
// 
// Program for computing the sum of the vector elements
// 
// created by Dmitry Gorpinchenko
//
// ****************************************************

#include <iostream>
#include <vector>

using namespace std;

//size of the vector to be summed
const size_t N = 40;

//sums vector's d elements and
//stores the sum in address pointed by p 
inline void sum(const vector<int>& d, int* p);

int main() 
{
    //initialize the data
    vector<int> data(N);
    for(int i = 0; i < N; ++i) 
    {
        data[i] = i;
    }
    
    //compute and print the sum of the data
    int accum = 0;
    sum(data, &accum);
    cout << "sum is " << accum << endl;
    
    return 0;
}

inline void sum(const vector<int>& d, int* p) 
{
    *p = 0;
    for(size_t i = 0; i < d.size(); *p += d[i++]);
}
