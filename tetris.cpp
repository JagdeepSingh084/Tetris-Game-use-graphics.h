#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

const int M = 20; // Number of rows
const int N = 10; // Number of columns

int field[M][N] = {0}; // Game field
int score = 0; // Score variable

struct Point {
    int x, y;
} a[4], b[4]; // Current and previous figure points

// Figures represented in a 2D array
int figures[7][4] = {
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5  // O
};

// Function to check if the figure can be placed
bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return false;
        else if (field[a[i].y][a[i].x])
            return false;
    }
    return true;
}

// Function to draw the playing field
void drawField() {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j]) {
                setcolor(field[i][j]);
                rectangle(j * 20 + 50, i * 20 + 50, j * 20 + 70, i * 20 + 70);
                floodfill(j * 20 + 55, i * 20 + 55, field[i][j]);
            } else {
                setcolor(WHITE);
                rectangle(j * 20 + 50, i * 20 + 50, j * 20 + 70, i * 20 + 70);
            }
        }
    }
}

// Function to draw the current active figure
void drawFigure(int colorNum) {
    setfillstyle(SOLID_FILL, colorNum);
    for (int i = 0; i < 4; i++) {
        int x = a[i].x * 20 + 50;
        int y = a[i].y * 20 + 50;
        setcolor(colorNum);
        rectangle(x, y, x + 20, y + 20);
        floodfill(x + 5, y + 5, colorNum);
    }
}

// Function to hard drop the figure
void hardDrop() {
    while (true) {
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].y += 1;
        }
        if (!check()) {
            for (int i = 0; i < 4; i++)
                a[i] = b[i]; // Revert to the last valid position
            break;
        }
    }
}

// Function to check if a row is completely filled
bool isRowFilled(int row) {
    for (int j = 0; j < N; j++) {
        if (field[row][j] == 0) {
            return false; // Found an empty cell in the row
        }
    }
    return true; // Row is completely filled
}

// Function to clear filled rows and update score
void clearFilledRows() {
    int clearedRows = 0; // Count of cleared rows
    for (int row = M - 1; row >= 0; row--) {
        if (isRowFilled(row)) { // Check if the current row is filled
            // Clear the filled row
            for (int j = 0; j < N; j++) {
                field[row][j] = 0; 
            }
            clearedRows++; // Increment cleared row count
            
            // Shift down rows above the cleared row
            for (int k = row - 1; k >= 0; k--) {
                for (int j = 0; j < N; j++) {
                    field[k + 1][j] = field[k][j]; // Shift down
                }
            }
            // Clear the original row position after moving
            for (int j = 0; j < N; j++) {
                field[row][j] = 0; // Ensure the cleared row is empty
            }
            row++; // Check this row again after moving
        }
    }
    
    // Update score based on the number of cleared rows
    if (clearedRows > 0) {
        score += clearedRows * 10; // Increment score by 10 for each cleared row
    }
}

// Function to check if the game is over
bool isGameOver() {
    for (int i = 0; i < N; i++) {
        if (field[0][i] != 0)
            return true;
    }
    return false;
}

// Function to show the "Game Over" message and final score
void displayGameOverMessage() {
    cleardevice();

    // Display final score
    char finalScoreText[30];
    sprintf(finalScoreText, "Final Score: %d", score);
    setcolor(WHITE);
    settextstyle(10, 0, 4); // Set text style for the final score
    outtextxy(150, 150, finalScoreText); // Print final score above "Game Over"

    // Display "Game Over" message
    setcolor(RED);
    settextstyle(10, 0, 5); // Set text style for the "Game Over" message
    outtextxy(150, 200, "GAME OVER");

    // Display menu for restarting or quitting
    setcolor(WHITE);
    settextstyle(10, 0, 2);
    outtextxy(150, 300, "Press R to Restart");
    outtextxy(150, 350, "Press Q to Quit");

    // Wait for user input to restart or quit
    while (true) {
        if (GetAsyncKeyState('R')) break;  // Restart if 'R' is pressed
        else if (GetAsyncKeyState('Q')) {
            closegraph();
            exit(0);  // Quit if 'Q' is pressed
        }
    }
}
// Function to reset the game board
void resetGame() {
    score = 0; // Reset score
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            field[i][j] = 0;
        }
    }
}

// Function to check if a new figure can be spawned
bool canSpawnNewFigure(int figureIndex) {
    for (int i = 0; i < 4; i++) {
        a[i].x = figures[figureIndex][i] % 2 + 4; // Center the figure in the field
        a[i].y = figures[figureIndex][i] / 2;

        if (field[a[i].y][a[i].x] != 0) {
            return false;
        }
    }
    return true;
}

// Function to display the score
void displayScore() {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(400, 50, scoreText); // Display score on the right side
}

// Function to display the instructions
void displayInstructions() {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(300, 100, "Controls:");
    outtextxy(300, 130, "Arrow Up    : Rotate");
    outtextxy(300, 160, "Arrow Down  : Hard Drop");
    outtextxy(300, 190, "Arrow Left  : Move Left");
    outtextxy(300, 220, "Arrow Right : Move Right");
    outtextxy(300, 250, "Esc         : Exit");
    outtextxy(300, 280, "R           : Restart");
}

int main() {
    int gd = DETECT, gm;
    char path[] = "C:\\MinGW\\lib\\libbgi.a";
    initgraph(&gd, &gm, path);

    srand(time(0));
    int dx = 0, rotate = 0, colorNum = 1;
    float timer = 0, fallDelay = 0.3;

    // Spawn the first figure
    int n = rand() % 7;
    if (!canSpawnNewFigure(n)) {
        displayGameOverMessage();
        resetGame();
    }

    while (true) {
        delay(50); // Control speed

        // Check if user pressed Q to quit
        if (GetAsyncKeyState('Q')) {
            closegraph();
            exit(0);
        }

        // Check if user pressed R to restart
        if (GetAsyncKeyState('R')) {
            resetGame();  // Reset the game state
            n = rand() % 7; // Spawn a new figure
            if (!canSpawnNewFigure(n)) {
                displayGameOverMessage();
                resetGame();
            }
            continue; // Skip the rest of the loop and start fresh
        }

        if (GetAsyncKeyState(VK_ESCAPE)) break;

        if (GetAsyncKeyState(VK_RIGHT)) dx = 1;
        else if (GetAsyncKeyState(VK_LEFT)) dx = -1;
        else if (GetAsyncKeyState(VK_UP)) rotate = 1;
        else if (GetAsyncKeyState(VK_DOWN)) hardDrop(); // Hard drop when down arrow is pressed
        else dx = 0, rotate = 0, fallDelay = 0.3;

        // Move the figure left/right
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];

        // Rotate the figure
        if (rotate) {
            Point p = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check())
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
        }

        // Move the figure down
        timer += 0.05;
        if (timer > fallDelay) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check()) {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;

                // Clear filled rows and update score
                clearFilledRows();

                // Check for game over
                if (isGameOver()) {
                    displayGameOverMessage();
                    resetGame();
                }

                // Spawn new figure
                colorNum = 1 + rand() % 7;
                n = rand() % 7;
                while (!canSpawnNewFigure(n)) {
                    displayGameOverMessage();
                    resetGame();
                }
            }

            timer = 0; // Reset timer
        }

        // Clear screen and redraw everything
        cleardevice();
        drawField();
        drawFigure(colorNum);
        displayScore(); // Display the current score
        displayInstructions(); // Display the controls instructions

        // Check for game over conditions after drawing
        if (isGameOver()) {
            displayGameOverMessage();
            resetGame();
        }
    }

    // Close the graphics mode
    closegraph();
    return 0;
}

