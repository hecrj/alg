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
// Proof of correctness is inside the function using induction:
// Induction hypothesis:
// multiselect works correctly given rangesSize <= h, where h < n
template<class T> void multiselect(vector<T> &elements, const vector<int> &ranges,
        int elementStart, int elementEnd, int rangeStart, int rangeEnd)
{
    // We define the range size as it follows
    int rangesSize = rangeEnd - rangeStart + 1;
    // We suppose that n = rangeSize
    
    // If the range size is less than 1, we don't need to do anything
    if(rangesSize < 1)
        return;
    
    // Base case occurs when range size is equal to 1
    // All we need to do is quick select the range
    if(rangesSize == 1)
        quickselect(elements, ranges[rangeStart], elementStart, elementEnd);
    
    else
    {
        // If range size is greater than 1, we select the center of the ranges as the pivot
        int center = rangeStart + rangesSize / 2;
        int pivot = ranges[center];
        
        // Then we quickselect the pivot
        int j = quickselect<T>(elements, pivot, elementStart, elementEnd);
        
        // Now pivot is placed at position j in elements, which is the position that would take in case elements
        // were sorted and that is the correct position.
        // Now we can divide, because quickselect uses partition, the elements as follows:
        //      elements[i] given elementStart <= i < j => elements[i] <= pivot
        //      elements[i] given j < i <= elementEnd   => elements[i] >= pivot
        //
        // Because the ranges are sorted in increasing order, they can be divided as follows:
        //      ranges[i] given rangeStart <= i < center => ranges[i] < pivot
        //      ranges[i] given center < i <= rangeEnd   => ranges[i] > pivot
        //
        // So, we only need to call multiselect recursively for each of this parts using the range part accordingly.
        multiselect(elements, ranges, elementStart, j-1, rangeStart, center-1);
        multiselect(elements, ranges, j+1, elementEnd, center+1, rangeEnd);
        
        // We know that the rangesSize of every call is going to be rangesSize - 1, because we extract the center.
        // Therefore (rangesSize - 1) < n and the induction hypothesis guarantees that these recursive calls work
        // correctly.
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
