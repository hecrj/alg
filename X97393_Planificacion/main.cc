#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <list>

using namespace std;

/**
 * Represents a set of tasks with dependencies.
 */
class Project
{
    static string message_cycles() { return "Proyecto contiene ciclos"; }
    
    /**
     * Represents a task.
     */
    struct Task
    {
        /**
         * Id of the task
         */
        string id;
        
        /**
         * Time the task needs until completion.
         */
        int duration;
        
        /**
         * List of tasks from which the current task is prerequisite.
         */
        vector<int> childs;
        
        /**
         * List of tasks that are prerrequisite of the current task.
         */
        vector<int> prerequisites;
        
        /**
         * Earliest start time of the task.
         */
        int min_start;
        
        /**
         * Latest finish time of the task.
         */
        int max_end;
        
        Task() : duration(0), min_start(0), max_end(0)
        { }
    };
    
    /**
     * Set of tasks of the project
     */
    vector<Task> tasks;
    
    /**
     * Maps every task identifier with its index in the tasks vector.
     */
    unordered_map<string, int> indexes;
    
public:
    /**
     * Reads a project from the input stream.
     * Each task gets reserved an index in the tasks vector in the order
     * they are read.
     * Each task gets related with its prerequisites and the tasks from which
     * is prerequisite. This is, actually, like building two dependency graphs.
     * 
     * Additionally to the read tasks, two default tasks are added:
     * - START task, which is prerequisite of all the read tasks that don't have
     *   any prerequisite.
     * - END task, which has as prerequisites all the read tasks that are not
     *   prerequisite of any task.
     * 
     * Average cost:
     * Suppose n is the number of tasks read and m is the number of prerequisite
     * relations read.
     * For each task i the cost is:
     * Reserve or find previous index + Read prerequiste relations = O(1) + O(mi)
     * Where mi is the number of prerequisite relations of the task i.
     * 
     * If we have n tasks, the cost of reading all of them is:
     * sum{ i=0 -> n }( O(1) + O(mi) ) = O(n) + O(m) = O(n + m)
     * The cost of adding the START and END task is O(n), thus the total cost is:
     * O(n) + O(n + m) = O(n + m)
     */
    void read()
    {
        int n;
        cin >> n;
        
        // We leave some space for the START and END tasks
        tasks.resize(n+2);
        
        for(int i = 1; i <= n; ++i)
        {
            string id;
            cin >> id;
            
            // Reserve an index in the tasks vector if id has not been seen before
            int index = get_index(id);
            
            // Read duration
            cin >> tasks[index].duration;
            
            // Start reading prerequisite relations
            cin >> id;
            while(id != "@")
            {
                int requirement_index = get_index(id);
                
                // Establish the prerequisite relations
                tasks[index].childs.push_back(requirement_index);
                tasks[requirement_index].prerequisites.push_back(index);
                
                cin >> id;
            }
        }
        
        // Add START and END tasks
        Task& start = tasks[0];
        Task& end = tasks.back();
        
        start.id = "START";
        end.id = "END";
        
        for(int i = 1; i <= n; ++i)
        {
            // Relate START with tasks that have no prerequisite
            if(tasks[i].prerequisites.size() == 0)
            {
                start.childs.push_back(i);
                tasks[i].prerequisites.push_back(0);
            }
            
            // Relate END with tasks that aren't prerequisite of any task
            if(tasks[i].childs.size() == 0)
            {
                end.prerequisites.push_back(i);
                tasks[i].childs.push_back(n+1);
            }
        }
    }
    
    /**
     * Tries to plan the current project.
     * If the project has a cycle then a warning message is printed.
     * If the project has not a cycle, then the early and latest times of
     * every task in the project are calculated and the planning table of
     * the project is printed.
     * 
     * Average cost:
     * calculate_early_times() has a cost of O(n + m)
     * calculate_latest_times() has a cost of O(n + m)
     * print has a cost of O(n)
     * Where n is the number of the project tasks and m is the number of
     * prerequisite relations.
     * 
     * Thus, the total cost is:
     * O(n + m) + O(n + m) + O(n) = O(n + m)
     * If m >> n then the cost is O(m).
     */
    void plan()
    {
        if(!calculate_early_times())
            cout << message_cycles() << endl;
        
        else
        {
            calculate_latest_times();
            print();
        }
    }
    
private:
    /**
     * Gets the reserved index in the tasks vector for the given task id.
     * If the id has been seen before it returns the previously reserved index.
     * If the id has not been seen before it reserves and returns a new index.
     * 
     * Average cost:
     * Find index + Add index = O(1) + O(1) = O(1)
     * 
     * @param id A task id
     * @return The reserved index
     */
    int get_index(string id)
    {
        // Try to find the id in the indexes map
        unordered_map<string, int>::iterator it = indexes.find(id);
        
        // If not found, reserve and return a new index
        if(it == indexes.end())
            return add(id);
        
        // If found, return the previously reserved index
        return it->second;
    }
    
    /**
     * Reserves a new index for the given task id.
     * 
     * Average cost: O(1)
     * 
     * @param id A task id
     * @return The reserved index
     */
    int add(string id)
    {
        // Reserve new index
        int index = indexes.size() + 1;
        indexes[id] = index;
        
        // Set task id
        tasks[index].id = id;
        
        return index;
    }
    
    /**
     * Calculates the early times of every task in the project.
     * @return True if success, false if the project contains a cycle
     */
    bool calculate_early_times()
    {
        // Proof of correctness:
        // Let unvisited be the number of tasks that have not been visited
        // yet. Initially, all tasks are unvisited.
        int unvisited = tasks.size();
        
        // prerequisite_count[i] contains the number of prerequisites tasks
        // of the task i which have not been visited yet
        vector<int> prerequisite_count(unvisited);
        
        // Initially all tasks are unvisited, thus the prerequisite count
        // of a task i is the total number of prerequisites of the task i
        for(int i = 0; i < unvisited; ++i)
            prerequisite_count[i] = tasks[i].prerequisites.size();
        
        // pending contains the tasks i where prerequisite_count[i] == 0
        queue<int> pending;
        
        // Initially the START task is the only one with prerequisite_count == 0.
        // This is because when reading the project the START task is set as prerequisite
        // of all the tasks that have no prerequisites.
        pending.push(0); // Add START task
        
        // Invariants:
        // - unvisited = number of tasks not visited yet
        // - prerequisite_count[i] = number of prerequisites tasks of task i not visited yet
        // - pending = tasks where prerequisite_count[i] == 0
        // - unvisited tasks have set the early start time as the maximum of the early end times
        //   of its prerequisite visited tasks. This implies that visited tasks have the earliest
        //   start time set correctly.
        while(!pending.empty())
        {
            // Get and pop first unvisited task
            Task& current = tasks[pending.front()];
            pending.pop();
            
            // The earliest start time of a task i is the maximum of the earliest
            // end times of its prerequisites
            // Calculate the earliest end time of the current task
            int min_end = current.min_start + current.duration;
            
            // For each task i that is a child of the current task:
            for(int i = 0; i < current.childs.size(); ++i)
            {
                int child = current.childs[i];
                
                // If the earliest end time of the current task is greater than the
                // earliest start time of the task i. We update the start time to
                // fulfill the invariant.
                if(min_end > tasks[child].min_start)
                    tasks[child].min_start = min_end;
                
                // Current task is a visited task now, thus is necessary to update the
                // prerequisite count of the task i, and if it results to be 0 we add the
                // task i to the pending queue to fulfill the invariant.
                if(--prerequisite_count[child] == 0)
                    pending.push(child);
            }
            
            // Current task is a visited task now
            --unvisited;
        }
        
        // Now, by invariant, all the visited tasks have the earliest start time set correctly.
        // But the project can contain a cycle.
        // If, and only if, the project contains a cycle then unvisited is not equal to 0.
        
        // This can be proven easily:
        // Project contains a cycle => unvisited != 0
        // A task is only marked as visited when all its prerequisites are visited.
        // If the project has a cycle, it means that exists a prerequisite relation:
        // A -> T1 -> T2 -> ... Tn-1 -> Tn -> A, where n >= 0
        // Thus, A is only going to be marked as visited when Tn is visited, and Tn only
        // when Tn-1, and so on. Lastly we found that T1 can only be visited if A is visited.
        // Thus we found that A can only be visited if A has been visited, and that
        // is impossible.
        
        // unvisited != 0 => Project contains a cycle
        // Suppose that unvisited != 0 and project does not contain a cycle.
        // This means (by invariant) that exists some task t that remains unvisited, but this
        // can only happen if the task has a prerequisite that has not been visited, and so on.
        // But this can not go forever, because there is a finite number of tasks and the
        // prerequisite graph is connected (because START and END tasks).
        // Thus, there is a cycle of unvisited tasks.
        
        // If unvisited equals 0 then the project does not have a cycle, this means that all
        // tasks have the earliest start time set correctly, then we return true.
        // If unvisited is not equal to 0 then the project contains a cycle and we return false,
        // as described above.
        return unvisited == 0;
    }
    
    /**
     * Calculates the latest times of every task in the project.
     * Pre: The project does not contain a cycle
     */
    void calculate_latest_times()
    {
        // Proof of correctness:
        // child_count[i] contains the number of childs of the task i that have not been
        // visited yet
        vector<int> child_count(tasks.size());
        
        // END task latest end time is equal to the earlist end time
        Task& end = tasks.back();
        end.max_end = end.min_start + end.duration;
        
        for(int i = 0; i < tasks.size(); ++i)
        {
            // Initially all tasks are unvisited, thus the child count
            // of a task i is the total number of childs of the task i
            child_count[i] = tasks[i].childs.size();
            
            // The latest end time of a task i is the minimum of the latest
            // start times of its childs.
            // Thus, initially the latest end time of all the tasks is set to the
            // end time of the project as the limit.
            tasks[i].max_end = end.max_end; // Set maximum project time
        }
        
        // pending contains the tasks i where prerequisite_count[i] == 0
        queue<int> pending;
        
        // Initially the END task is the only one with child_count == 0.
        // This is because when reading the project the END task is set as child
        // of all the tasks that have no childs.
        pending.push(tasks.size() - 1); // Add END task
        
        // Invariants:
        // - child_count[i] = number of child tasks of task i not visited yet
        // - pending = tasks where child_count[i] == 0
        // - unvisited tasks have set the lastest end time as the minimum of the latest
        //   start times of its child visited tasks. This implies that visited tasks have
        //   the latest end time set correctly.
        while(!pending.empty())
        {
            Task& current = tasks[pending.front()];
            pending.pop();
            
            // Calculate the latest start time of the current task
            int max_start = current.max_end - current.duration;
            
            // For each prerequisite of the current task i:
            for(int i = 0; i < current.prerequisites.size(); ++i)
            {
                int prerrequisite = current.prerequisites[i];
                
                // If the current task (child) has a latest start time less than the
                // latest end time of i, then is necessary to update the end time to
                // fulfill the invariant.
                if(max_start < tasks[prerrequisite].max_end)
                    tasks[prerrequisite].max_end = max_start;
                
                // Current task is a visited task now, thus is necessary to update the
                // child count of the task i, and if it results to be 0 we add the task i to
                // the pending queue to fulfill the invariant.
                if(--child_count[prerrequisite] == 0)
                    pending.push(prerrequisite);
            }
        }
        
        // Now, by invariant, and because the project does not contain any cicles (precondition),
        // all the visited tasks have the latest end time set correctly.
    }
    
    /**
     * Prints the planning table of the project.
     * The planning table format is:
     * 
     * TASK_ID EARLY_START_TIME EARLY_END_TIME LATEST_START_TIME LATEST_END_TIME CRITICAL?
     * 
     * Pads every column to 6 characters (fills with spaces), except the last one.
     */
    void print()
    {
        // Pad left
        cout << left;
        
        for(int i = 0; i < tasks.size(); ++i)
        {
            Task& task = tasks[i];
            
            int min_end = task.min_start + task.duration;
            
            cout << setw(6) << task.id;
            cout << setw(6) << task.min_start;
            cout << setw(6) << min_end;
            cout << setw(6) << task.max_end - task.duration;
            cout << setw(6) << task.max_end;
            
            if(min_end == task.max_end)
                cout << '*';
            
            cout << endl;
        }
    }
};

/**
 * Reads a project planning problem and tries to solve it.
 * If the project contains cycles prints a warning message.
 * If the project does not contain any cycle the planning table of the project is printed.
 * 
 * Average cost:
 * Read + Plan = O(n + m) + O(n + m) = O(n + m)
 * Where n is the number of tasks of the project and m is the number of prerequisites relations
 * between tasks of the project.
 * If m >> n then the cost is: O(m)
 * 
 * @return Execution status
 */
int main()
{
    Project project;
    
    project.read();
    project.plan();
    
    return 0;
}
