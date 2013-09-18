#include <iostream>
#include <vector>

using namespace std;

// Constant cost: O(1)
template<class T> inline T selectPivot(vector<T> &elements, int start, int end)
{
    int center = (start + end) / 2;
    
    if(elements[end] < elements[center])
        swap(elements[center], elements[start]);
    
    if(elements[start] < elements[center])
        swap(elements[center], elements[start]);
    
    if(elements[start] < elements[end])
        swap(elements[start], elements[end]);
    
    return elements[end];
}


// Lineal cost: O(end - start)
template<class T> int partition(vector<T> &elements, int start, int end)
{
    T p = selectPivot(elements, start, end);
    int i = start;
    int j = end;

    while(i < j)
    {
        while(i < j and elements[i] <= p)
            i++;
        
        while(i < j and elements[j] >= p)
            j--;
        
        if(i < j)
            swap(elements[i], elements[j]);
    }
    
    swap(elements[end], elements[j]);
    
    return j;
}

// Average cost: O(n)
template<class T> int quickselect(vector<T> &elements, int k, int start, int end)
{
    if(start == end)
        return end;

    int j = partition(elements, start, end);

    if(k == j)
        return j;
    
    if(k < j)
        return quickselect(elements, k, start, j-1);

    return quickselect(elements, k, j+1, end);
}

// Cost: elements.size() * log(ranges.size())
template<class T> void multiselect(vector<T> &elements, const vector<int> &ranges,
        int elementStart, int elementEnd, int rangeStart, int rangeEnd)
{
    int rangesSize = rangeEnd - rangeStart + 1;
    
    if(rangesSize < 1)
        return;
    
    if(rangesSize == 1)
        quickselect(elements, ranges[rangeStart] - 1, elementStart, elementEnd);
    
    else
    {
        int center = rangeStart + rangesSize / 2;
        int pivot = ranges[center];
        
        int j = quickselect<T>(elements, pivot - 1, elementStart, elementEnd);
        
        multiselect(elements, ranges, elementStart, j-1, rangeStart, center-1);
        multiselect(elements, ranges, j+1, elementEnd, center+1, rangeEnd);
    }
}

/**
 * Simple shortcut for immersion.
 * @param elements
 * @param ranges
 */
template<class T> void multiselect(vector<T> &elements, const vector<int> &ranges)
{
    multiselect(elements, ranges, 0, elements.size() - 1, 0, ranges.size() - 1);
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
    
    multiselect(elements, ranges);
    
    for(int i = 0; i < p; ++i)
    {
        if(i != 0)
            cout << ' ';
        
        cout << elements[ranges[i] - 1];
    }
    
    cout << endl;
    
    return 0;
}
