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
        
        while(i <= j and elements[j] >= p)
            j--;
        
        if(i <= j)
            myswap<T>(elements[i], elements[j]);
    }
    
    myswap<T>(elements[start], elements[j]);
    
    return j;
}

// Cost: n log s
template<class T> T quick_select(vector<T> &elements, int k, int start, int end)
{
    if(start == end)
        return elements[end];

    int j = partition(elements, start, end);

    if(k == j)
        return elements[j];
    
    if(k < j)
        return quick_select(elements, k, start, j-1);

    return quick_select(elements, k, j+1, end);
}

// Cost: n log p
template<class T> void mselect(vector<T> &elements, const vector<int> &ranges,
        int elementStart, int elementEnd, int rangeStart, int rangeEnd)
{
    int rangesSize = rangeEnd - rangeStart + 1;
    
    if(rangesSize < 1)
        return;
    
    if(rangesSize == 1)
    {
        cout << rangeStart << endl;
        quick_select<T>(elements, ranges[rangeStart] + 1 - elementStart, elementStart, elementEnd);
    }
    else
    {
        int middle = rangeStart + rangesSize / 2;
        cout << middle << endl;
        int pivot = ranges[middle];
        
        quick_select<T>(elements, pivot + 1 - elementStart, elementStart, elementEnd);
        
        mselect<T>(elements, ranges, elementStart, middle-1, rangeStart, middle-1);
        mselect<T>(elements, ranges, middle+1, elementEnd, middle+1, rangeEnd);
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
    
    mselect<int>(elements, ranges, 0, elements.size() - 1, 0, ranges.size() - 1);
    
    for(int i = 0; i < p; ++i)
    {
        if(i != 0)
            cout << ' ';
        
        cout << elements[ranges[i] - 1];
    }
    
    cout << endl;
    
    return 0;
}
