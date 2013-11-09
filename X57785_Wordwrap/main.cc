#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Paragraph
{
    vector<string> words;
    vector<int> lines;
    vector<int> optimalCosts;
    vector< vector<int> > lineCosts;
    
    void computeLineCosts(int width)
    {
        lineCosts = vector< vector<int> >(words.size(), vector<int>(words.size()));
        
        // First, calculate the lengths
        lineCosts[0][0] = 0; // Empty word
        
        for(int i = 1; i < lineCosts.size(); ++i)
        {
            lineCosts[i][i] = words[i].size();
            
            for(int j = i+1; j < lineCosts.size(); ++j)
                lineCosts[i][j] = lineCosts[i][j-1] + words[j].size() + 1;
        }
        
        // Reusing the matrix, calculate the costs
        for(int i = 0; i < lineCosts.size(); ++i)
        {
            for(int j = i; j < lineCosts.size(); ++j)
            {
                int cost_2 = lineCosts[i][j] - width;
                
                lineCosts[i][j] = cost_2 * cost_2;
            }
        }
    }
    
    void computeLineArrangement()
    {
        lines = vector<int>(words.size());
        optimalCosts = vector<int>(words.size());
        optimalCosts[0] = lineCosts[0][0];
        
        for(int j = 1; j < words.size(); ++j)
        {
            int min_cost = lineCosts[1][j];
            int min_index = 1;
            
            for(int i = 2; i <= j; ++i)
            {
                int cost = optimalCosts[i-1] + lineCosts[i][j];
                
                if(cost < min_cost)
                {
                    min_cost = cost;
                    min_index = i;
                }
            }
            
            optimalCosts[j] = min_cost;
            lines[j] = min_index;
        }
    }
    
    void printLines(int index)
    {
        if(index < 1)
            return;
        
        printLines(lines[index] - 1);
        
        for(int i = lines[index]; i <= index; ++i)
        {
            cout << words[i];
            
            if(i != index)
                cout << ' ';
        }
        
        cout << endl;
    }
    
public:
    void read()
    {
        string s;
        
        // Empty word for simplicity
        words.push_back("");
        
        while(getline(cin, s))
        {
            if(s.empty() and not empty())
                break;
            
            istringstream line(s);

            while(line >> s)
                words.push_back(s);
        }
        
        lines = vector<int>(words.size());
        
        // Paragraph consists in only one line by default
        lines[lines.size() - 1] = 1; // Skip simple word
    }
    
    bool empty()
    {
        return words.size() < 2;
    }
    
    int wordwrap(int width)
    {
        computeLineCosts(width);
        computeLineArrangement();
        
        return optimalCosts[words.size() - 1];
    }
    
    void print()
    {
        printLines(lines.size() - 1);
    }
};

int main()
{
    int w;
    cin >> w;
    
    bool first = true;
    while(!cin.eof())
    {
        Paragraph p;
        p.read();
        
        if(not p.empty())
        {   
            int penalty = p.wordwrap(w);
            
            // Print separator, if necessary
            if(first) first = false;
            else cout << endl;
            
            p.print();
            cout << "Penalty: " << penalty << endl;
        }
    }
    
    return 0;
}
