#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>

int* initBoard(int*, int, int);
void swapTiles(int*, int, int, int, int, int, int);
void checkConnections(int* board, int width, int height, int stack[][2], int visited[][2], int *stackCount, int *visitedCount, int sourceX, int sourceY);


int* initBoard(int* board, int width, int height) {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            board[x*width + y] = rand() % 5;
        }
    }
    return board;
}


void swapTiles(int* board, int width, int height, int x1, int y1, int x2, int y2) {
    int xDiff = abs(x1 - x2);
    int yDiff = abs(y1 - y2);

    // check tiles aren't more than two away and are on same row/column and are not equal
    if (xDiff > 1 || yDiff > 1 || (xDiff && yDiff) || (!xDiff && !yDiff)) {
        return;
    }

    int tempTile = board[x1*width + y1];
    board[x1*width + y1] = board[x2*width + y2];
    board[x2*width + y2] = tempTile;

    int visited[width*height][2];
    int stack[width*height][2];
    int visitedCount = 0;
    int stackCount = 0;
    checkConnections(board, width, height, stack, visited, &stackCount, &visitedCount, x1, y1);
    if (visitedCount >= 3) {
        for (int i = 0; i < visitedCount; i++) {
            board[visited[i][0] * width + visited[i][1]] = -1;
        }
    }

    memset(visited, 0, sizeof(visited[0][0]) * width*height*2);
    memset(stack, 0, sizeof(stack[0][0]) * width*height*2);
    visitedCount = 0;
    stackCount = 0;
    checkConnections(board, width, height, stack, visited, &stackCount, &visitedCount, x2, y2);
    if (visitedCount >= 3) {
        for (int i = 0; i < visitedCount; i++) {
            board[visited[i][0] * width + visited[i][1]] = -1;
        }
    }

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {

            // if black box (aka cleared box) is encountered, perform falling fill
            if (board[x*width + y] == -1) {

                // shift blocks down and add a random block to the start
                for(int z = y; z > 0; z--) {
                    board[x*width + z] = board[x*width + z-1];
                }
                board[x*width + 0] = rand() % 5;
            }
        }
    }
}


// UP-LEFT-DOWN-RIGHT hierarchy DFS to find connected cells of same colour
void checkConnections(int* board, int width, int height, int stack[][2], int visited[][2], int *stackCount, int *visitedCount, int sourceX, int sourceY) {
    // check if cell is in stack already, if so: exit
    for(int i = 0; i < *stackCount; i++) {
        if (stack[i][0] == sourceX && stack[i][1] == sourceY) {
            return;
        }
    }

    // add current
    stack[*stackCount][0] = sourceX;
    stack[(*stackCount)++][1] = sourceY;

    // Check if there is an 'up' neighbour of same colour, if so: perform DFS on it
    if (sourceY-1 >= 0 && board[sourceX*width + sourceY-1] == board[sourceX*width + sourceY]) {
        checkConnections(board, width, height, stack, visited, stackCount, visitedCount, sourceX, sourceY-1);
    }
    // Check if there is an 'left' neighbour of same colour, if so: perform DFS on it
    if (sourceX-1 >= 0 && board[(sourceX-1)*width + sourceY] == board[sourceX*width + sourceY]) {
        checkConnections(board, width, height, stack, visited, stackCount, visitedCount, sourceX-1, sourceY);
    }
    // Check if there is an 'down' neighbour of same colour, if so: perform DFS on it
    if (sourceY+1 < height && board[sourceX*width + sourceY+1] == board[sourceX*width + sourceY]) {
        checkConnections(board, width, height, stack, visited, stackCount, visitedCount, sourceX, sourceY+1);
    }
    // Check if there is an 'right' neighbour of same colour, if so: perform DFS on it
    if (sourceX+1 < width && board[(sourceX+1)*width + sourceY] == board[sourceX*width + sourceY]) {
        checkConnections(board, width, height, stack, visited, stackCount, visitedCount, sourceX+1, sourceY);
    }

    // set cell as visited
    visited[*visitedCount][0] = sourceX;
    visited[(*visitedCount)++][1] = sourceY;
}


int main() {
    unsigned int windowWidth = 1000;
    unsigned int windowHeight = 1000;
    int boardWidth = 10;
    int boardHeight = 10;
    int gap = 2;

    int board[boardWidth * boardHeight];
    int prevSelectedX = -1;
    int prevSelectedY = -1;
    int mouseX = -1;
    int mouseY = -1;

    float cellWidth = (windowWidth - ((boardWidth+1) * gap)) / (float)boardWidth;
    float cellHeight = (windowHeight - ((boardHeight+1) * gap)) / (float)boardHeight;

    // create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
                            "Tile Match", sf::Style::Titlebar | sf::Style::Close);

    initBoard(board, boardWidth, boardHeight);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {

                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseMoved:
                    mouseX = event.mouseMove.x / (windowWidth/boardWidth);
                    mouseY = event.mouseMove.y / (windowHeight/boardHeight);
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {

                        int selectedX = event.mouseButton.x / (windowWidth/boardWidth);
                        int selectedY = event.mouseButton.y / (windowHeight/boardHeight);
                        if (prevSelectedX == -1 && prevSelectedY == -1) {
                            prevSelectedX = selectedX;
                            prevSelectedY = selectedY;
                        } else {
                            swapTiles(board, boardWidth, boardHeight,
                                      selectedX, selectedY, prevSelectedX, prevSelectedY);
                            prevSelectedX = -1;
                            prevSelectedY = -1;
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        window.clear(sf::Color::Black);

        for(int x = 0; x < boardWidth; x++) {
            for(int y = 0; y < boardHeight; y++) {

                sf::RectangleShape rectangle(sf::Vector2f(cellWidth, cellHeight));
                sf::Color color;
                switch(board[x*boardWidth + y]) {
                    case 0:
                        color = sf::Color::White;
                        break;
                    case 1:
                        color = sf::Color::Green;
                        break;
                    case 2:
                        color = sf::Color::Cyan;
                        break;
                    case 3:
                        color = sf::Color::Red;
                        break;
                    case 4:
                        color = sf::Color::Yellow;
                        break;
                    default:
                        color = sf::Color::Black;
                        break;
                }

                if ((x == prevSelectedX && y == prevSelectedY) || (x == mouseX && y == mouseY)) {
                    color.a = 128;
                }

                rectangle.setFillColor(color);
                rectangle.setPosition(x*cellWidth + (x+1)*gap, y*cellHeight + (y+1)*gap);

                window.draw(rectangle);
            }
        }

        window.display();
    }

    return 0;
}