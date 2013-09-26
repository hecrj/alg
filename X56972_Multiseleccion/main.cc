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

// Average cost: O(log n)
template<class T> int bsearch(const vector<T> &elements, int k, int start, int end)
{
    if(start + 1 == end)
        return start;
    
    int center = (start + end) / 2;
    
    if(k < elements[center])
        return bsearch(elements, k, start, center);
    
    return bsearch(elements, k, center, end);
}

// Cost: elements.size() * log(ranges.size())
// Proof of correctness is inside the function using induction:
// Induction hypothesis:
// multiselect works correctly given rangesSize <= h, where h < n
template<class T> void multiselect(vector<T> &elements, const vector<int> &ranges,
        int elementStart, int elementEnd, int rangeStart, int rangeEnd)
{
    if(rangeStart > rangeEnd || elementStart >= elementEnd)
        return;
    
    int k = partition(elements, elementStart, elementEnd);
    int l = bsearch(ranges, k, rangeStart, rangeEnd + 1);
    
    if(ranges[l] == k)
    {
        multiselect(elements, ranges, elementStart, k-1, rangeStart, l-1);
        multiselect(elements, ranges, k+1, elementEnd, l+1, rangeEnd);
    }
    else if(ranges[l] < k)
    {
        multiselect(elements, ranges, elementStart, k-1, rangeStart, l);
        multiselect(elements, ranges, k+1, elementEnd, l+1, rangeEnd);
    }
    else
        multiselect(elements, ranges, k+1, elementEnd, l, rangeEnd);
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
    {
        int aux;
        cin >> aux;
        
        ranges[i] = aux - 1;
    }
    
    vector<int> elements(n);
    
    for(int i = 0; i < n; ++i)
        cin >> elements[i];
    
    multiselect(elements, ranges);
    
    for(int i = 0; i < p; ++i)
    {
        if(i != 0)
            cout << ' ';
        
        cout << elements[ranges[i]];
    }
    
    cout << endl;
    
    return 0;
}
