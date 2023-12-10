#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <queue>


using namespace std;

// Define the 8-puzzle state


string heur;  // by default it is h1

vector<vector<int>> goalState = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8}
};


struct PuzzleState {
    vector<vector<int>> board;
    int blankRow, blankCol;
    int heuristic;
    PuzzleState* parent; // Pointer to the parent state

    PuzzleState(const vector<vector<int>> &initialBoard, PuzzleState* p = nullptr)
    {

        board = initialBoard;
        parent = p;

        // Find the position of the blank tile
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == 0) {
                    blankRow = i;
                    blankCol = j;
                    break;
                }
            }
        }


        heuristic = calculateHeuristic();
    }

    // Check if two states are equal
    bool operator==(const PuzzleState& other) const {
        return board == other.board;
    }

    // Check if one state is less than another
    bool operator<(const PuzzleState& other) const {
        return board < other.board; // You may need to implement a proper comparison
    }

    // Calculate the heuristic value (Manhattan distance)
    int calculateHeuristic() const{
        int h = 0;
        int value;

        if(heur == "h1")
        {


        for (int i = 0; i < 3; ++i)
        {
          for (int j = 0; j < 3; ++j)
          {
                value = board[i][j];
                if (value != 0)
                {
                    int targetRow = (value - 1) / 3;
                    int targetCol = (value - 1) % 3;
                    h += abs(i - targetRow) + abs(j - targetCol);
                }

          }

        }
         return h;
        }

        else
        {

           for (int i = 0; i<3; i++)
           {
               for(int j = 0; j<3; j++)
               {
                   value = board[i][j];

                   if(value != 0 && value != goalState[i][j] )
                   {
                      h = h+1;

                   }


               }

           }

           return h;
        }



    }

};

// Define the comparison function for the priority queue
struct CompareStates {
    bool operator()(const PuzzleState* a, const PuzzleState* b) const {
        return (a->heuristic) > (b->heuristic);
    }
};


// A* search algorithm
void solvePuzzle(const vector<vector<int>>& initialState) {
    PuzzleState startState(initialState);
    int counter = 0;

    // Priority queue to store states based on total cost
    priority_queue<PuzzleState*, vector<PuzzleState*>, CompareStates> pq;

    // Map to keep track of visited states
    map<PuzzleState, bool> visited;

    // Enqueue the start state
    pq.push(&startState);
    visited[startState] = true;

    while (!pq.empty()) {
        // Dequeue the state with the lowest total cost
        PuzzleState* currentState = pq.top();
        pq.pop();
        // Check if the current state is the goal state
        if (*currentState == PuzzleState(goalState)) {
            // Trace and print the solution path
            while (currentState->parent != nullptr) {
                // Print the state (or store it in a vector for later use)
                // In this example, it prints the entire state matrix
                cout<<"Move "<<++counter<<" \n";
                for (const auto& row : currentState->board) {
                    for (int value : row) {
                        cout << value << " ";
                    }
                    cout << endl;
                }
                cout << "\n";

                PuzzleState* temp = currentState->parent;
                delete currentState; // Free memory
                currentState = temp;
            }

            cout<<"puzzle solved in "<<counter<<" moves \n";

            // Free memory for the initial state
            delete currentState;

            return;
        }

        // Generate successors
        int moves[][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

        for (int i = 0; i < 4; ++i) {
            int newRow = currentState->blankRow + moves[i][0];
            int newCol = currentState->blankCol + moves[i][1];

            // Check if the position is valid
            if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3)
            {
                // Create a new state by swapping the blank tile with the adjacent tile
                PuzzleState* nextState = new PuzzleState(currentState->board, currentState);
                swap(nextState->board[currentState->blankRow][currentState->blankCol],
                     nextState->board[newRow][newCol]);

                // Check if the new state has not been visited before
                if (!visited[*nextState]) {
                    nextState->blankRow = newRow;
                    nextState->blankCol = newCol;
                    nextState->heuristic = nextState->calculateHeuristic();

                    // Enqueue the new state
                    pq.push(nextState);
                    visited[*nextState] = true;
                } else {
                    delete nextState; // Free memory for duplicate state
                }
            }
        }
    }

    cout << "Puzzle cannot be solved.\n";
}

int main() {
    // Example initial state
    vector<vector<int>> initialBoard;
    int element=0;
    cout<<"welcome to the 8 puzzle problem of greedy search..\nplease fill in the values for the initial state >>> Note for blank space use 0\n";

    for (int i=0;i<3;i++)
    {
        vector<int> vec;

      for (int j=0; j<3; j++)
      {
        cout<<"\nenter for element position ["<<i<<"]["<<j<<"] ";
        cin >>element;
        vec.push_back(element);
      }
      initialBoard.push_back(vec);

    }

    for ( auto it : initialBoard)
    {
        for (auto i : it)
      {
        cout<<i<<" ";
      }
      cout<<"\n";

    }

    cout<<"which heuristic would you like, enter h1 for manhattan and h2 for misplaced tiles?\n";
    cin >> heur;

    cout<<"ok now searching in progress......\n\n";
    solvePuzzle(initialBoard);

    return 0;
}
