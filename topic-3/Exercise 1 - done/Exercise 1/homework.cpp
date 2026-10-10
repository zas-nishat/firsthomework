#include <iostream>
#include <vector>
#include <string>
#include "raylib.h"

#include <chrono>
#include <thread>

using namespace std;

/*
    Exercise 3: Cells and Grids + variation of Knights Tour problem

    If you have issues with raylib after installing it, make sure files inside .vscode point to the right locations.

    In this exercise you will start by creating two classes:

        "Cell": represents one square in a grid, and can draw one character.
            Each Cell should store:
                - its row number    (int)   (make this private)
                - its column number (int)   (make this private)
                - a character       (char)  (make this public)
            Create a constructor that receives the row and column, and automatically inserts space character (' ').
            Create draw() method, that calls the provided drawCell() method

        "Grid": represents a collection of Cell objects.
            The grid is 2 dimensional. I recommend using a 2d vector (i.e. nested vector) for storage.
            It would be initialised as such: vector<vector<Cell>> cells;
            The variable storing the Cell objects must be private.

            Create a constructor that receives the number of rows and columns, and creates the Cell objects
            Create a setCell() method that receives a row, column, and character, finds the corresponding Cell, and changes its value.
            Create a getCell() method that receives row and column, and returns corresponding Cell object.
            Create draw() method that iterates though every Cell in the Grid and calls draw() on them.

    Once this is done, running the application should show you an 8x8 grid. The application will stop once you close the grid.
*/

////////////////////////////////////////////////////
// Provided GUI interface
//
// You only need to worry about the arguments
//     that you need to pass to this function
////////////////////////////////////////////////////

const int CELL_SIZE = 60;

void drawCell(int row, int column, char value)
{
    int x = (column + 1) * CELL_SIZE;
    int y = (row + 1) * CELL_SIZE;

    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, RAYWHITE);
    DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, DARKGRAY);

    if (value != ' ')
    {
        string text(1, value);

        int fontSize = 30;
        int textWidth = MeasureText(text.c_str(), fontSize);

        DrawText(
            text.c_str(),
            x + (CELL_SIZE - textWidth) / 2,
            y + (CELL_SIZE - fontSize) / 2,
            fontSize,
            BLACK);
    }
}

////////////////////////////////////////////////////
// Your code starts here
////////////////////////////////////////////////////

class Cell
{
private:
    int row;
    int column;

public:
    char value;

    Cell(int r, int c) : row(r), column(c), value(' ') {}

    void draw()
    {
        drawCell(row, column, value);
    }
    
    int getRow() { return row; }
    int getCol() { return column; }
};

class Grid
{
private:
    vector<vector<Cell>> cells;
    int rows;
    int cols;

public:
    Grid(int r, int c) : rows(r), cols(c)
    {
        for (int i = 0; i < rows; ++i)
        {
            vector<Cell> row;
            for (int j = 0; j < cols; ++j)
            {
                row.push_back(Cell(i, j));
            }
            cells.push_back(row);
        }
    }

    void setCell(int r, int c, char val)
    {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            cells[r][c].value = val;
        }
    }

    Cell& getCell(int r, int c)
    {
        return cells[r][c];
    }

    void draw()
    {
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                cells[r][c].draw();
            }
        }
    }
    
    int getRows() { return rows; }
    int getCols() { return cols; }
};

// Once you have Grid and Cell working, you may move to part 2 of this exercise, which is below.

/*  Part 2 of this exercise: calculating minimum moves required by a knight in chess to go from square A to square B

    Below you see a processIteration() function.
    Whatever you insert into it, it will be performed, and if you retuned 0, it will be called again after 500ms.

    you will be passed:
    - the grid that was created using the class you defined
    - current interation number (i.e. first time this runs, you get 1. Second time, 2. Etc.)
    - start and end cells from the grid (start cell will be filled with 'S', end cell will be kept empty)

    The way you should implement this algorithm:
    - every step, calculate which cells can be reached from cells that currently have value other than ' '
    - for each of those cells, if they don't already have a number, set their value to the current iteration
    - repeat until the endCell is filled with a number. That number is the solution that you should return in processIteration()
        (if you have not reached the endCell yet, return 0 to continue in the next iteration)
*/

int processIteration(Grid &grid, int curIter, Cell &startCell, Cell &endCell)
{
    int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

    struct Pos { int r, c; };
    vector<Pos> current_filled;
    int rows = grid.getRows();
    int cols = grid.getCols();
    
    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c < cols; ++c) {
            char val = grid.getCell(r, c).value;
            if (val != ' ') {
                current_filled.push_back({r, c});
            }
        }
    }

    for(Pos p : current_filled) {
        int r = p.r;
        int c = p.c;
        for(int i = 0; i < 8; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (grid.getCell(nr, nc).value == ' ') {
                    char it_val = '0' + curIter;
                    if (curIter > 9) it_val = 'A' + (curIter - 10);
                    grid.getCell(nr, nc).value = it_val;
                    
                    if (nr == endCell.getRow() && nc == endCell.getCol()) {
                        return curIter;
                    }
                }
            }
        }
    }
    return 0;
}

////////////////////////////////////////////////////
// Your code ends here
////////////////////////////////////////////////////

void draw(Grid grid, bool testing, Color color)
{
    if (!testing)
    {
        BeginDrawing();
        ClearBackground(color);
        grid.draw();
        EndDrawing();
        this_thread::sleep_for(1000ms);
    }
}

int main(int argc, char *argv[])
{
    Grid grid(8, 8);

    int startRow = 2;
    int startCol = 1;
    int endRow = 7;
    int endCol = 2;

    bool testing = argc > 1;
    if (testing)
    {
        startRow = stoi(argv[1]);
        startCol = stoi(argv[2]);
        endRow = stoi(argv[3]);
        endCol = stoi(argv[4]);
    }

    grid.setCell(startRow, startCol, 'S');
    Cell &startCell = grid.getCell(startRow, startCol);
    Cell &endCell = grid.getCell(endRow, endCol);

    if (!testing)
    {
        SetTraceLogLevel(LOG_NONE);
        InitWindow(600, 600, "OOP GUI Practical");
        SetTargetFPS(30);
    }

    int curIter = 1;

    while (testing || !WindowShouldClose())
    {
        draw(grid, testing, LIGHTGRAY);

        int answer = processIteration(grid, curIter++, startCell, endCell);
        if (answer)
        {
            draw(grid, testing, GREEN);
            if (!testing)
                CloseWindow();

            std::cout << answer << endl;
            break;
        }
    }

    return 0;
}