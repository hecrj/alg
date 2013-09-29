#include <iostream>
#include <vector>

using namespace std;

/**
 * Selects the pivot between start, end and (start + end) / 2.
 * Puts the pivot at end position.
 * Cost: Constant
 * @param elements The elements vector
 * @param start Start index
 * @param end End index
 * @return The selected pivot
 */
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


/**
 * Selects a pivot p of elements[start..end] and partitionates the elements vector:
 *      elements[start..j-1] <= p
 *      elements[j+1..end] >= p
 * Where j is the position of p after partitionating (the position that p will hold if the vector was sorted).
 * Cost: O(end - start) = Linear
 * @param elements Elements vector
 * @param start Start index
 * @param end End index
 * @return Position of the pivot j after partitioning
 */
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

/**
 * Performs a binary search on elements[start..end-1], that must be sorted.
 * It returns an index i where start <= i < end:
 *      If elements[start] <= k <= elements[end-1] => elements[i] <= k < elements[i+1]
 *      If k < elements[start] => i = start
 *      if k > elements[end-1] => i = end - 1
 * Cost: O(log (end - start))
 * @param elements Elements vector
 * @param k The element to find
 * @param start Start index
 * @param end End index
 * @return The index i described above
 */
template<class T> int bsearch(const vector<T> &elements, T k, int start, int end)
{
    if(start + 1 == end)
        return start;
    
    int center = (start + end) / 2;
    
    if(k < elements[center])
        return bsearch(elements, k, start, center);
    
    return bsearch(elements, k, center, end);
}

/**
 * Resolves the multiselection problem.
 * So after applying this function:
 *      r = ranges[i] where rangeStart <= i <= rangeEnd => elements[r] is the r-smallest element in the vector
 * Cost: (elementEnd - elementStart) * log(rankEnd - rankStart)
 * Proof of correctness:
 *      We define: n = elementEnd - elementStart
 *      Induction hypothesis:
 *      multiselect works correctly if n <= h
 * Note: We use x = vector[start..end] to denote every element x = vector[i] where start <= i <= end
 * @param elements Elements vector
 * @param ranks Ranges to select
 * @param elementStart Element start index
 * @param elementEnd Element end index
 * @param rankStart Range start index
 * @param rankEnd Range end index
 */
template<class T> void multiselect(vector<T> &elements, const vector<int> &ranks,
        int elementStart, int elementEnd, int rankStart, int rankEnd)
{
    // Base case:
    // If n = 0 or there are no ranks -> We don't need to do anything
    if(rankStart > rankEnd || elementStart >= elementEnd)
        return;
    
    // We need to show: n = h+1 => multiselect works correctly
    // Thus, we suppose n = h+1
    // Select a pivot with index k and partitionate the elements vector
    int k = partition(elements, elementStart, elementEnd);
    
    // Now, we know:
    //   x = elements[elementStart..k-1] => x <= elements[k]
    //   x = elements[k+1..elementEnd]   => x >= elements[k]
    
    // Find the closest rank index l in the vector
    int l = bsearch(ranks, k, rankStart, rankEnd + 1);
    
    if(ranks[l] == k)
    {
        // ranks[l] == k => ranks[l] is correctly positioned
        // This means that:
        //   r = ranks[rankStart..l-1] => elementStart <= r < k
        //   r = ranks[l+1..rankEnd]   => k < r <= elementEnd
        // Thus, we only need to take care of ranks[rankStart..l-1] and ranks[l+1..rankEnd] accordingly
        // and we can exclude the pivot of the elements.
        multiselect(elements, ranks, elementStart, k-1, rankStart, l-1);
        multiselect(elements, ranks, k+1, elementEnd, l+1, rankEnd);
        
        // Because elementStart <= k <= elementEnd and h = elementEnd - elementStart - 1, then:
        //   (k - 1 - elementStart) <= h < h+1 => first call works!
        //   (elementEnd - (k+1))   <= h < h+1 => second call works!
    }
    else if(ranks[l] < k)
    {
        // ranks[l] < k => ranks[rankStart..l] < k and ranks[l+1..rankEnd] > k
        // This means that:
        //   r = ranks[rankStart..l] => elementStart <= r <= k-1
        //   r = ranks[l+1..rankEnd] => k+1 <= r <= elementEnd
        // Thus, we can divide the problem, excluding k, and solve it recursively!
        multiselect(elements, ranks, elementStart, k-1, rankStart, l);
        multiselect(elements, ranks, k+1, elementEnd, l+1, rankEnd);
        
        // It was proven before that these two calls work by induction hypothesis. 
    }
    else
    {
        // ranks[l] > k => k < ranks[rankStart..rankEnd] 
        // This means that:
        //   r = ranks[rankStart..rankEnd] => elementStart <= k < r <= elementEnd
        // Thus we exclude elements[elementStart..k] because they are not ranks.
        multiselect(elements, ranks, k+1, elementEnd, rankStart, rankEnd);
        
        // It was proven before that this call works by induction hypothesis. 
    }
}

/**
 * Solves the multiselection problem.
 * Simple shortcut for immersion.
 * @param elements Elements vector
 * @param ranks Ranks to select
 */
template<class T> void multiselect(vector<T> &elements, const vector<int> &ranks)
{
    multiselect(elements, ranks, 0, elements.size() - 1, 0, ranks.size() - 1);
}

/**
 * Reads a vector from the input stream.
 * @param v Destination vector
 */
template<class T> void read_vector(vector<T>& v)
{
    for(int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

/**
 * Reads a vector of integers and substracts 1 to each element.
 * @param ranks Ranks destination vector
 */
void read_ranks(vector<int>& ranks)
{
    for(int i = 0; i < ranks.size(); ++i)
    {
        int aux;
        cin >> aux;
        
        ranks[i] = aux - 1;
    }
}

/**
 * Prints a vector v using indexes as keys.
 * @param v Vector elements to print
 * @param indexes Indexes of v to print
 */
template<class T> void print_vector(const vector<T>& v, const vector<T> &indexes)
{
    for(int i = 0; i < indexes.size(); ++i)
    {
        if(i != 0)
            cout << ' ';
        
        cout << v[indexes[i]];
    }
    
    cout << endl;
}

/**
 * Solves the multiselection problem:
 *   1. Reads two integers: n, p where p <= n
 *   2. Reads a sequence of p sorted integers representing the ranks
 *   3. Reads a sequence of n integers representing the elements
 *   4. For every rank r in ranks, outputs the r-smallest element in elements
 * @return Execution status
 */
int main()
{
    int n, p;
    cin >> n >> p;
    
    vector<int> elements(n);
    vector<int> ranges(p);
    
    read_ranks(ranges);
    read_vector(elements);
    
    multiselect(elements, ranges);
    
    print_vector(elements, ranges);
    
    return 0;
}
