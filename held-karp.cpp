#include <iostream>
#include <vector>
#include <limits>

using namespace std;

#define N 11 // number of nodes
#define G 5 // number of groups

int group_count[G] = { 1, 4, 3, 1, 2 }; // how many nodes per group
int groups[G][4] = { // nodes in each group
    { 0 },
    { 1, 2, 3, 4 },
    { 5, 6, 7 },
    { 8 },
    { 9, 10 },
};
int node_group[N] = { 0, 1, 1, 1, 1, 2, 2, 2, 3, 4, 4 }; // group each node is in

int matrix[N][N] = { // sample distance matrix
    //0   1    2    3    4    5    6    7    8    9    10
    {0,   200, 120, 110, 150, 130, 190, 230, 10,  30,  200}, // 0
    {200, 0,   140, 300, 380, 100, 190, 480, 210, 210, 100}, // 1
    {120, 140, 0,   170, 220, 140, 80,  400, 110, 140, 100}, // 2
    {110, 300, 170, 0,   120, 220, 220, 200, 80,  100, 230}, // 3
    {150, 380, 220, 120, 0,   220, 400, 70,  160, 130, 400}, // 4
    {130, 100, 140, 220, 220, 0,   220, 330, 170, 150, 180}, // 5
    {190, 190, 80,  220, 400, 220, 0,   480, 190, 210, 110}, // 6
    {230, 480, 400, 200, 70,  330, 480, 0,   220, 200, 480}, // 7
    {10,  210, 110, 80,  160, 170, 190, 220, 0,   40,  200}, // 8
    {30,  210, 140, 100, 130, 150, 210, 200, 40,  0,   210}, // 9
    {200, 100, 100, 230, 400, 180, 110, 480, 200, 210, 0}, // 10
}; // expected output: ?

string bin(int decimal) {
    if (decimal == 0) {
        return "0";
    }

    string binary = "";
    while (decimal > 0) {
        int remainder = decimal % 2;
        binary = to_string(remainder) + binary;
        decimal /= 2;
    }

    return binary;
}

pair<int, vector<int> > held_karp() {
    int num_subsets = 1 << N; // 2^N possible subsets
    int num_groupsets = 1 << G; // 2^G possible group combinations
    int group_subsets[G];
    for (int i = 0; i < G; i++) {
        group_subsets[i] = 1 << group_count[i];
    }

    // dp[i][j][k] = minimum distance with group mask i, subset j, ending at k
    vector<vector<vector<int> > > dp(num_groupsets, vector<vector<int> >(num_subsets, vector<int>(N, INT_MAX))); 
    vector<vector<vector<int> > > prev(num_groupsets, vector<vector<int> >(num_subsets, vector<int>(N, INT_MAX))); 


    dp[1][1][0] = 0; // base case - starting node to itself distance is 0
    prev[1][1][0] = -1; 

    for (int groupmask = 1; groupmask < num_groupsets; groupmask++) { // loop through all possible group sets
        for (int mask = 1; mask < num_subsets; mask++) { // loop through all possible subsets                      - SPEED UP BY LOOPING USING GROUPMASK?
            // check if mask is valid
            bool included[G]; // included[i] is true if a node from group i was already included
            for (int i = 0; i < G; i++) included[i] = false; // default to false
            bool subset_valid = true;
            for (int i = 0; i < N; i++) {
                if (mask & (1 << i)) { // i is in mask
                    int group = node_group[i];
                    if (included[group] || !(groupmask & (1 << group))) { // multiple nodes in one group or node not in group mask
                        subset_valid = false;
                        break;
                    } else {
                        included[group] = true;
                    }
                }
            }
            if (!subset_valid) continue; // if subset is invalid
            cout << "groupmask: " << bin(groupmask) << " mask: " << bin(mask) << endl;
            
            for (int i = 0; i < G; i++) {
                if (!(groupmask & (1 << i))) continue; // loop through included groups
                cout << "included group: " << i << endl;
                for (int j = 0; j < group_count[i]; j++) { 
                    int k = groups[i][j];
                    if (dp[groupmask][mask][k] == INT_MAX) continue; // loop through included nodes
                    cout << "included node: " << k << endl;
                    for (int x = 0; x < G; x++) {
                        if (groupmask & (1 << x)) continue; // loop through not included groups
                        cout << "  not included group: " << x << endl;
                        for (int y = 0; y < group_count[x]; y++) { // loop through each node in not included group
                            int z = groups[x][y];
                            cout << "    not included node: " << z << endl;
                            int newmask = mask | (1 << z); // subset with y included
                            cout << "    new mask: " << bin(newmask) << endl;
                            int newgroupmask = groupmask | (1 << x); // groupmask with group x included
                            cout << "    new group mask: " << bin(newgroupmask) << endl;
                            // update distance
                            int new_val = dp[groupmask][mask][k] + matrix[k][z];
                            if (new_val < dp[newgroupmask][newmask][z]) {
                                dp[newgroupmask][newmask][z] = new_val;
                                prev[newgroupmask][newmask][z] = k;
                            }
                            cout << "    new dp value: " << dp[newgroupmask][newmask][z] << endl;
                            cout << "    new prev value: " << prev[newgroupmask][newmask][z] << endl;
                        }
                    }
                }
            }
        }
    }

    int full_group_mask = (1 << G) - 1; // all groups in groupmask
    int shortest_mask = 0;
    int shortest_distance = INT_MAX; 
    int last_node = 0;
    for (int mask = 0; mask < num_subsets; mask++) { // check all subsets
        for (int i = 1; i < N; i++) { // find minimum distance of full subset back to starting node
            if (dp[full_group_mask][mask][i] == INT_MAX) continue;
            int new_val = dp[full_group_mask][mask][i] + matrix[i][0];
            if (new_val < shortest_distance) {
                shortest_distance = new_val;
                shortest_mask = mask;
                last_node = i;
            }
        }
    }

    vector<int> optimal_path; // optimal route
    optimal_path.push_back(0);
    int current_node = last_node;
    int current_mask = shortest_mask;
    int current_group_mask = full_group_mask;
    while (current_node != -1) {
        cout << bin(current_group_mask) << " " << bin(current_mask) << " " << current_node << endl;
        optimal_path.push_back(current_node);
        int next_node = prev[current_group_mask][current_mask][current_node];
        int next_mask = current_mask ^ (1 << current_node); // remove current_node from current_mask
        int next_group_mask = current_group_mask ^ (1 << node_group[current_node]); // remove current group
        current_node = next_node;
        current_mask = next_mask;
        current_group_mask = next_group_mask;
    }

    reverse(optimal_path.begin(), optimal_path.end());
    return make_pair(shortest_distance, optimal_path);
}


int main() {
    freopen("output.txt","w",stdout);
    pair<int, vector<int> > result = held_karp();
    cout << result.first << endl;
    for (int i = 0; i < result.second.size(); i++) {
        cout << result.second[i] << " ";
    }
    cout << endl;
    return 0;
}