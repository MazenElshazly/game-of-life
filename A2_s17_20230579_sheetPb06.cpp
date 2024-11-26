#include <iostream>
#include <vector>
#include <random>
#include <windows.h>
#include <thread>
using namespace std;

// Function to set the console text and background color
///this function is to facilitate seeing the changes as they happen, much easier with diff. colors
///if using online compiler then comment out the <windows.h> header file and this function
///you select the a line in the console to see the zeros and ones behind the filled boxes.
void SetColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
                            (bgColor << 4) | textColor);
}

//class definition
class Universe {
private:
    int rows, cols;
    vector<vector<bool>> Grid;
    float prob;

public:
    Universe(int rows, int cols,float probability ) : rows(rows), cols(cols),prob(probability), Grid(rows, vector<bool>(cols, false)) {}

    void initialize(float probability_alive);
    void reset();
    int count_neighbors(int x, int y);
    void next_generation( vector<vector<bool>>& grid);
    void display();
    void setGrid(int row ,int row2, int col, int col2);
    void run(int generations, int delay_ms = 500);
    void Run2(int generation, int delayMs);
};


                            //class implementation\\
///this Function gives a random start to the program where each cell has a predetermined chance of being alive

void Universe::initialize(float probability_alive) {
    probability_alive = prob;
    // Create a random number generator engine
    random_device rd;            // Seed for random generator
    mt19937 gen(rd());           // Mersenne Twister engine

    // Define a uniform real distribution for probabilities in the range [0, 1)
    uniform_real_distribution<float> distribution(0.0, 1.0);

    // Resize the Grid to the specified dimensions and initialize
    Grid.resize(rows, vector<bool>(cols, false));

    // Populate the Grid with random live/dead cells based on the given probability
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float random_value = distribution(gen);
            Grid[i][j] = (random_value < probability_alive); // Cell is alive if random < probability_alive
        }
    }

    // Print the Grid

}
void Universe::reset() {
    for (int i = 0; i <rows ; ++i) {
        for (int j = 0; j < cols; ++j) {
            Grid[i][j]=false;
        }
    }
 }
 void Universe::display() {
     ///this function's only purpose is to display the Grid to the user
     for (int i = 0; i < rows; ++i) {
         for (int j = 0; j < cols; ++j) {
             if(Grid[i][j]){
                 SetColor(10,10);
                 cout<<'1';
             } else{
                 SetColor(8,8);
                 cout<<'0';
                }
             SetColor(15,0);
             }

         cout << '\n'; // Newline after each row
     }
}
int Universe::count_neighbors(int x, int y) {
//the function intakes the coords of the cell to be checked(x,y),
//then the following code creates a 3*3 Grid to check around the cell(x,y) and see whether its dead or alive

    int aliveCount = 0;

    for (int i = -1; i <=1 ; ++i) {
        for (int j = -1; j <= 1; ++j) {\
        //here we don't pass the i and j directly as at the boundaries of the Grid there would be cases of "out of bounds"

            if(i==0 and j ==0)continue;
            int neighbour_x = x +i;
            int neighbour_y = y+ j;

            if(neighbour_x >= 0 && neighbour_x<rows &&
               neighbour_y >=0 && neighbour_y < cols ){

                aliveCount += Grid[neighbour_x][neighbour_y];

            }
        }
    }
    return aliveCount;
}
void Universe::next_generation(vector<vector<bool>>& grid) {
    /// determines whether a cell should be dead or alive according to game rules
    vector <vector<bool>> temp = grid;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int numOfAlive =count_neighbors(i,j);
            if (grid[i][j] && numOfAlive < 2){
                temp[i][j] = false;
            }
            if (grid[i][j] && numOfAlive >3){
                temp[i][j] = false;
            }
            if (!grid[i][j] && numOfAlive == 3){
               temp [i][j]= true;
            }

        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Grid[i][j]= temp[i][j];
        }
    }
}
void Universe::setGrid(int row, int row2, int col, int col2) {
    ///this function sets the rows and columns determined by user to alive

    for (int i = row; i < row2+1; ++i) {
        for (int j = 0; j < cols; ++j) {
            Grid[i][j] = true;
        }
    }

    for (int i = col; i < col2+1 ; ++i) {
        for (int j = 0; j < rows; ++j) {
            Grid[j][i]= true;
        }
    }
}
void Universe::run(int generations, int delay_ms) {
    ///runs the whole program
    initialize(prob);
    for (int gen = 0; gen < generations; ++gen) {

        // Display the current grid
        display();
        cout << "\n------------------------\n";

        //system cls doesn't work in clion
        //system("cls");


        // Wait for a short duration
        this_thread::sleep_for(chrono::milliseconds(delay_ms));

        // Update the grid to the next generation
        next_generation(Grid);
    }
}
void Universe::Run2(int generation, int delayMs) {
    for (int gen = 0; gen < generation; ++gen) {

        // Display the current grid
        display();
        cout << "\n------------------------\n";

        //system cls doesn't work in clion
        //system("cls");


        // Wait for a short duration
        this_thread::sleep_for(chrono::milliseconds(delayMs));

        // Update the grid to the next generation
        next_generation(Grid);
    }
}


int main() {
    int height;
    int width;
    int choice;
    int rowS;
    int rowE;
    int col_S;
    int col_E;
    cout<<"height of grid"<<'\n';
    cin>>height;
    cout<<"enter width of grid"<<'\n';
    cin>>width;
    cout<<"choose 1 if you want to specify which rows and columns to fill, \nchoose 2 if you want it to be at random"<<'\n';
    cin>>choice;
    while(choice!= 1 and choice != 2){
        cout<<"enter a correct choice"<<'\n';
        cin>>choice;
    }




    if(choice == 2) {
        cout << "enter percentage of alive cells from 0.1-0.9"<<endl;
        float prob;
        cin >> prob;
        ///the constructor intakes the grid size and the probability that each cell is alive

        while (prob > 1.0 or prob < 0.0) {
            cout << "invalid, enter a valid value"<<endl;
            cin >> prob;
            ///the constructor intakes the grid size and the probability that each cell is alive
            Universe world(height, width, prob);
        }
        Universe world(height,width,prob);
        cout<<"enter the number of generations to run the program for"<<endl;
        int genNum;
        cin>>genNum;
        cout<<"how fast or slow would you like the program to run?\nenter the delay between each generation in millie-seconds(between 250-2000 for the best display)"<<'\n';
        int delay;
        cin>>delay;
        world.run(genNum,delay);
    }






        if(choice == 1){
            cout<<"enter the range to fill from"<<'\n';
            cout<<"from row number: ";
            cin>>rowS;
            while(rowS > height or rowS < 0 ){
                cout<<"invalid, enter again"<<'\n';
                cin>>rowS;
            }
            cout<<"to row number: ";
            cin>>rowE;
            while(rowE < rowS or rowE > height){
                cout<<"invalid, it cant be smaller than the row start or bigger than height of the grid, enter again"<<'\n';
                cin>>rowE;
            }
            cout<<"from column number: ";
            cin >> col_S;
            while(col_S > width or col_S < 0 ){
                cout<<"invalid, it cant be bigger than thr grid width, enter again"<<endl;
                cin >> col_S;
            }
            cout<<"To column number: ";
            cin>>col_E;
            while(col_E < col_S or col_E > width){
                cout<<"invalid, it cant be smaller than the column start or bigger than width of the grid, enter again"<<'\n';
                cin>>col_E;
            }
            Universe world(height,width,0.3);
            world.setGrid(rowS-1,rowE-1,col_S-1,col_E-1);
            cout<<"enter the number of generations to run the program for"<<endl;
            int genNum;
            cin>>genNum;
            cout<<"how fast or slow would you like the program to run?\nenter the delay between each generation in millie-seconds(between 250-2000 for the best display)"<<'\n';
            int delay;
            cin>>delay;
            world.Run2(genNum,delay);
        }


}
