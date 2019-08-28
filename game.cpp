#include <SFML\Graphics.hpp>

using namespace std;

//create grid and randomly generate cells
vector<vector<int>> makeGrid(int cols, int rows)
{
	vector<vector<int>> grid(cols, vector<int>(rows, 1));
	for (auto &col : grid)
		for (auto &row : col)
			row = rand() % 2;
	return grid;
}

//count neighbors of current cell
int countLN(const vector<vector<int>> &grid,int x, int y, int cols, int rows)
{
	int sum = 0;
	for (int i = -1; i < 2; i++)
		for (int j = -1; j < 2; j++)
		{
			int col = (x + i + cols) % cols;
			int row = (y + j + rows) % rows;
			sum += grid[col][row];
		}
	sum-=grid[x][y];
	return sum;
}
//Apply the rules of game of life
int rules(int state, int neighbors)
{
	//if cell is dead and it has 3 neighbors, revive cell
	if (state == 0 && neighbors == 3)
		return 1;
	//if cell is alive, but it has less than 2 or more
	//than 3 neighbors kill cell
	else if (state == 1 && (neighbors < 2 || neighbors >3))
		return 0;
	//else keep cell's current state
	else
		return -1;
}

//calculate the next generation grid
void nextGen(const vector<vector<int>> &grid, vector<vector<int>> &next, int cols, int rows)
{
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			int neighbors = countLN(grid, i, j, cols, rows);

			int nState = rules(grid[i][j], neighbors);
			if (nState == -1)
				next[i][j] = grid[i][j];
			else
				next[i][j] = nState;
		}
	}
}
int main() {
	int cols, rows, resolution = 5;
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(600, 400), "Game of Life");
	cols = window.getSize().x/resolution;
	rows = window.getSize().y/resolution;

	//generate grid
	vector<vector<int>> grid =  makeGrid(cols, rows);
	
	//game loop
	while (window.isOpen())
	{
		sf::Event event;
		//check if close button was pressed
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//if 'R' is pressed restart simulation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			grid = makeGrid(cols, rows);

		window.clear(sf::Color::Black);
		//next Generation grid
		vector<vector<int>> next(cols, vector<int>(rows, 0));
		
		//draw cells
		for (int i = 0; i < cols; i++)
			for (int j = 0; j < rows; j++)
			{
				int x = i * resolution;
				int y = j * resolution;
				sf::RectangleShape rect(sf::Vector2f(resolution, resolution));
				rect.setPosition(sf:: Vector2f(x, y));
				if (grid[i][j] == 1)
					rect.setFillColor(sf::Color::White);
				else
					rect.setFillColor(sf::Color::Black);
				window.draw(rect);
			}
		window.display();

		//calculate the next generation grid
		nextGen(grid, next, cols, rows);
		//set nextgen grid to current grid
		grid = next;
		
	}
	return 0;
}