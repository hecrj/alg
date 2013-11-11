#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

/**
 * Represents a paragraph.
 */
class Paragraph
{
    /**
     * Words of the paragraph.
     * From now on:
     * n = words.size()
     */
    vector<string> words;
    
    /**
     * Given 0 <= i < n:
     * lines[i] = k, where words[k] is the word that should start the line where
     * words[i] is located when words[0..i] are optimally arranged
     */
    vector<int> lines;
    
    /**
     * Given 0 <= i < n:
     * optimalCosts[i] = optimal cost of arranging the words[0..i]
     */
    vector<int> optimalCosts;
    
    /**
     * Given 0 <= i < n:
     * lineWidths[i] = width ot line that contains words[0..i]
     */
    vector<int> lineWidths;
    
    /**
     * Calculates the line widths given the target line width.
     * 
     * Cost:
     * Cost of the main loop = O(n)
     * More details below.
     * 
     * @param width Target line width
     */
    void computeLineWidths()
    {
        // Initialize lineWidths structure
        lineWidths = vector<int>(words.size());
        
        // Base case
        // Line width of the line that only contains words[0] equals to words[0] width
        lineWidths[0] = words[0].size();
        
        // Line widths
        // Given 0 <= i < n:
        // Calculate the width for each line that ends with words[i]
        for(int i = 1; i < words.size(); ++i)
        {
            // The width of the line that starts with words[0] and ends with words[i] can be
            // defined recursively as the sum of:
            // 1. The width of the line that starts with words[0] and ends with words[i-1]
            // 2. Width of words[i]
            // 3. 1 (because the additional space separator)
            lineWidths[i] = lineWidths[i-1] + words[i].size() + 1;
            
            // We are doing constant work O(1) for every word i. Thus, the cost of this loop is:
            // (n-1) * O(1) = O(n)
        }
    }
    
    /**
     * Calculates the cost of having words[i..j] on the same line.
     * @param i Word that starts the line
     * @param j Word that ends the line
     * @param width Target line width
     * @return Cost of having words[i..j] on the same line
     */
    int cost(int i, int j, int width)
    {
        // Square root of the cost of having words[0..j] on the same line
        int cost_2 = lineWidths[j] - width;
        
        // If i > 0, then we remove the width of the line words[0..i] and the additional space
        if(i > 0)
            cost_2 = cost_2 - lineWidths[i-1] - 1;
        
        // Return the cost
        return cost_2 * cost_2;
    }
    
    /**
     * Prints the paragraph words[0..index] in lines recursively.
     * 
     * Cost:
     * Constant work for every words[0..index] => O(index)
     * 
     * @param index Index of the last word to print
     */
    void printLines(int index)
    {
        // Base case
        if(index < 0)
            return;
        
        // lines[index] - 1 is the word before the actual line
        // We can use recursivity to print words[0..lines[index]-1] correctly (by induction).
        printLines(lines[index] - 1);
        
        // Now, we only need to print the current line that contains words[lines[index]..index]
        for(int i = lines[index]; i <= index; ++i)
        {
            cout << words[i];
            
            if(i != index)
                cout << ' ';
        }
        
        cout << endl;
    }
    
public:
    /**
     * Reads a paragraph from the default input stream.
     * It reads from the input stream until a blank line is read.
     *
     * Cost: O(n), where n is the number of words read
     */
    void read()
    {
        string s;
        
        while(getline(cin, s))
        {
            istringstream line(s);
            
            bool empty = true;

            while(line >> s)
            {
                words.push_back(s);
                empty = false;
            }
            
            if(empty)
                break;
        }
        
        // Paragraph consists in only one line by default
        // Every word is in the same line of the first word
        lines = vector<int>(words.size(), 0);
    }
    
    /**
     * Tells whether the paragraph is empty or not.
     * @return True if the paragraph does not have any word, false otherwise
     */
    bool empty()
    {
        return words.size() < 1;
    }
    
    /**
     * Formats the words of the paragraphs in lines optimally (lowest penalty).
     * Detailed information: https://www.jutge.org/problems/X57785_es/statement
     * 
     * Every optimum arrangement can be described recursively:
     * If n = 1, the optimum arrangement is a line with the word itself.
     * If n > 1, there exists k: 0 <= k < n, such that the optimum arrangement is:
     * optimumArrangement(k)
     * words[k+1..n]
     * 
     * Thus, we can find the optimum arrangement for n words recursively:
     * 1. Find the optimum arrangement for k words, for every k: 0 <= k < n.
     * 2. Find the optimum arrangement for n words:
     *    - For every words[i] try to add a line with the words[i..n] to the arrangement
     *      i-1.
     *    - The optimum arrangement is the one with the lowest cost.
     * 
     * Cost:
     * The cost is the sum of:
     * 1. Cost of computeLineCosts = O(n^2)
     * 2. Cost of main loop:
     *    sum{j=0 -> n}( sum{i=1 -> j}( O(1) ) ) = O(n^2)
     * 
     * Thus, the total cost is:
     * O(n^2) + O(n^2) = O(n^2)
     * 
     * @param width Target line width
     * @return The penalty/cost of arranging the words[0..n-1] optimally
     */
    int wordwrap(int width)
    {
        // First, compute the line widths
        computeLineWidths();
        
        // Initialize optimal costs
        optimalCosts = vector<int>(words.size());
        
        // Given 0 <= j < words.size()
        // Calculate the optimal cost of arranging the words[0..j]
        for(int j = 0; j < words.size(); ++j)
        {
            // Set default minimum cost and index
            int min_cost = cost(0, j, width);
            int min_index = 0;
            
            // First iteration can be skipped because the default
            // minimum cost.
            // 
            // Given 0 < i <= j:
            // Check if there exists a better way to arrange the words[0..j] putting
            // the words[i..j] on a new line. If it exists, update min_cost and min_index
            // accordingly.
            //
            // Invariants:
            // 1. min_cost  = minimum cost of arranging the words[0..j] considering
            //    having the words[k..j] on its own line, for every 1 <= k <= i
            // 2. min_index = index of the word that should start the line that produces
            //    min_cost
            for(int i = 1; i <= j; ++i)
            {
                // Cost of adding the line with words[i..j] to the optimum arrangement
                // of words[0..i-1]
                int new_cost = optimalCosts[i-1] + cost(i, j, width);
                
                // If necessary, update minimum cost and index to fulfill invariant
                if(new_cost < min_cost)
                {
                    min_cost = new_cost;
                    min_index = i;
                }
            }
            
            // Note that we only update the min_index if the cost of adding the new line is
            // strictly less than the min_cost. This guarantees that if there is more than one
            // optimum (sub)arrangement the one that has more words in the last line is selected,
            // because default min_index is 0 and variable i is always incremented.
            
            // By invariant and recursive approach:
            // 1. min_cost  = minimum cost of arranging the words[0..j]
            // 2. min_index = index of the word that should start the line that produces
            //    min_cost 
            optimalCosts[j] = min_cost;
            lines[j] = min_index;
        }
        
        // Return the penalty/cost of arranging the words[0..n-1] optimally
        return optimalCosts[words.size() - 1];
    }
    
    /**
     * Prints the paragraph in the default output stream.
     * 
     * Cost:
     * Cost of printLines(n) = O(n)
     */
    void print()
    {
        printLines(words.size() - 1);
    }
};

/**
 * Reads a target line width and paragraphs from the default input stream.
 * Prints every read paragraph optimally formatted and its penalty.
 * 
 * Solution to the problem: https://www.jutge.org/problems/X57785_es/statement
 * @return Execution status
 */
int main()
{
    // Read target line width
    int width;
    cin >> width;
    
    // First paragraph?
    bool first = true;
    
    // While not end of file/input reached
    while(!cin.eof())
    {
        // Read a new paragraph
        Paragraph p;
        p.read();
        
        if(not p.empty())
        {
            // Wordwrap paragraph and get penalty
            int penalty = p.wordwrap(width);
            
            // Print separator, if necessary
            if(first) first = false;
            else cout << endl;
            
            // Print paragraph and penalty
            p.print();
            cout << "Penalty: " << penalty << endl;
        }
    }
    
    return 0;
}
