#include <SFML/Graphics.hpp>
#include <iostream>

int* initBoard(int* board, int width, int height) {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            board[x*width + y] = rand() % 5;
        }
    }
    return board;
}

void swapTiles(int* board, int width, int x1, int y1, int x2, int y2) {
    int xDiff = abs(x1 - x2);
    int yDiff = abs(y1 - y2);

    // check tiles aren't more than two away and are on same row/column
    if (xDiff > 1 || yDiff > 1 || (xDiff && yDiff)) {
        return;
    }

    int tempTile = board[x1*width + y1];
    board[x1*width + y1] = board[x2*width + y2];
    board[x2*width + y2] = tempTile;
}

int main() {
    int windowWidth = 1000;
    int windowHeight = 1000;
    int boardWidth = 10;
    int boardHeight = 10;
    int gap = 10;
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
                        std::cout << "the left button was pressed" << std::endl;
                        std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                        std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                        std::cout << "tile x: " << event.mouseButton.x / (windowWidth/boardWidth) << std::endl;
                        std::cout << "tile y: " << event.mouseButton.y / (windowHeight/boardHeight) << std::endl;

                        int selectedX = event.mouseButton.x / (windowWidth/boardWidth);
                        int selectedY = event.mouseButton.y / (windowHeight/boardHeight);
                        if (prevSelectedX == -1 && prevSelectedY == -1) {
                            prevSelectedX = selectedX;
                            prevSelectedY = selectedY;
                        } else {
                            swapTiles(board, boardWidth, selectedX, selectedY, prevSelectedX, prevSelectedY);
                            prevSelectedX = -1;
                            prevSelectedY = -1;
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        for(int x = 0; x < boardWidth; x++) {
            for(int y = 0; y < boardHeight; y++) {
                sf::RectangleShape rectangle(sf::Vector2f(cellWidth, cellHeight));

                int tile = board[x*boardWidth + y];
                sf::Color color;
                switch(tile) {
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
                        std::cout << "Whoa that's wrong" << std::endl;
                        break;
                }

                if (x == prevSelectedX && y == prevSelectedY) {
                    rectangle.setOutlineThickness(gap/2);
                    if (tile == 0) {
                        rectangle.setOutlineColor(sf::Color::Red);
                    } else {
                        rectangle.setOutlineColor(sf::Color::White);
                    }
                }

                if (x == mouseX && y == mouseY) {
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