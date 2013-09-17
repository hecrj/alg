#include <iostream>
#include <vector>

using namespace std;

// Constant cost: O(1)
template<class T> inline void myswap(T &a, T &b)
{
    T aux = a;
    a = b;
    b = aux;
}

// Lineal cost: O(n)
template<class T> int partition(vector<T> &elements, int start, int end)
{
    int i = start+1;
    int j = end;
    T p = elements[start];
    
    while(i <= j)
    {
        while(i <= j and elements[i] <= p)
            i++;
        
        while(i < j and elements[j] >= p)
            j++;
        
        if(i <= j)
            myswap<T>(elements[i], elements[j]);
    }
    
    myswap<T>(elements[start], elements[j]);
    
    return j;
}

// Cost: n log s
template<class T> int quickselect(vector<T> &elements, int select, int start, int end)
{
    if(start == end)
        return elements[start];
    
    int k = partition<T>(elements, start, end);
    
    if(select == k)
        return elements[k];
    
    if(select < k)
        return quickselect<T>(elements, start, select, k - 1);
    
    return quickselect<T>(elements, k + 1, select, end);    
}

// Cost: n log p
template<class T> void mselect(vector<T> &elements, const vector<int> &ranges, int elementStart, int elementEnd,
        int rangeStart, int rangeEnd)
{
    int rangesSize = rangeEnd - rangeStart;
    
    if(rangesSize <= 0)
        return;
    
    if(rangesSize == 1)
        quickselect<T>(elements, ranges[0], elementStart, elementEnd);
    else
    {
        int p = rangeStart + rangesSize / 2;
        int pivot = ranges[p];
        
        quickselect<T>(elements, pivot, elementStart, elementEnd);
        
        mselect<T>(elements, ranges, elementStart, p-1, rangeStart, p-1);
        mselect<T>(elements, ranges, p+1, elementEnd, p+1, rangeEnd);
    }
}

int main()
{
    int n, p;
    cin >> n >> p;

    vector<int> ranges(p);
    
    for(int i = 0; i < p; ++i)
        cin >> ranges[i];
    
    vector<int> elements(n);
    
    for(int i = 0; i < n; ++i)
        cin >> elements[i];
    
    mselect<int>(elements, ranges, 0, elements.size(), 0, ranges.size());
    
    for(int i = 0; i < p; ++i)
        cout << elements[ranges[i]];
    
    cout << endl;
    
    return 0;
}