#include "Grid.h"
#include <thread>

#define llint long long int


// Constructor
Grid::Grid(sf::RenderWindow* window, std::int8_t grid_width, std::int8_t grid_height)
{
	start_pos = sf::Vector2<std::int8_t>(-1, -1);
	destination_pos = sf::Vector2<std::int8_t>(-1, -1);

	this->window = window;
	w_width = this->window->getSize().x;
	w_height = this->window->getSize().y;

	this->grid_width = grid_width;
	this->grid_height = grid_height;
	
	grid.resize(grid_width);
	for (int i = 0; i < grid_width; i++)
	{
		grid[i].resize(grid_height);
	}

	init_grid();
}


// init_grid() initializes the grid with new Node objects and
// calls update_grid_layout() to set the size and position of
// the Node objects
void Grid::init_grid()
{
	for (int x = 0; x < grid_width; x++)
	{
		for (int y = 0; y < grid_height; y++)
		{
			delete grid[x][y];
			grid[x][y] = NULL;
		}
	}

	grid.resize(grid_width);
	for (int i = 0; i < grid_width; i++)
	{
		grid[i].resize(grid_height);
	}


	for (int x = 0; x < grid_width; x++)
	{
		for (int y = 0; y < grid_height; y++)
		{
			grid[x][y] = new Node(sf::Vector2<std::int8_t>(x, y));
		}
	}
	start_pos = sf::Vector2<std::int8_t>(-1, -1);
	destination_pos = sf::Vector2<std::int8_t>(-1, -1);
	update_grid_layout();
}

/* This function resizes the grid while maintaining
   the information held in the old grid, that way
   information about the states of each Node don't
   disappear*/
void Grid::resize(std::int8_t new_width, std::int8_t new_height)
{
	std::int8_t prev_width = grid_width;
	std::int8_t prev_height = grid_height;

	grid_width = new_width;
	grid_height = new_height;

	// Resize the vector

	grid.resize(new_width);
	for (int x = 0; x < new_width; x++)
	{
		grid[x].resize(new_height);
	}

	/*
	Now that the grid size is changed, we determine
	whether this new grid size is larger or smaller
	than the previous grid size. If it is smaller, 
	nothing needs to be done, if it is larger, the new 
	Nodes in the grid must be initialized
	*/

	// If the new_width is larger after resize but new_height is >= prev_height
	if (new_width > prev_width && new_height <= prev_height)
	{
		for (int x = prev_width; x < new_width; x++)
		{
			for (int y = 0; y < new_height; y++)
			{
				grid[x][y] = new Node(sf::Vector2<std::int8_t>(x, y));
			}
		}
	}

	// If the new_height is larger after resize but new_width is <= prev_width
	else if (new_height > prev_height && new_width <= prev_width)
	{
		for (int x = 0; x < new_width; x++)
		{
			for (int y = prev_height; y < new_height; y++)
			{
				grid[x][y] = new Node(sf::Vector2<std::int8_t>(x, y));
			}
		}
	}

	// If the new_width is > prev_width and new_height is > prev_height
	else if (new_width > prev_width && new_height > prev_height)
	{
		// Fill in new grid space for width
		for (int x = prev_width; x < new_width; x++)
		{
			for (int y = 0; y < new_height; y++)
			{
				grid[x][y] = new Node(sf::Vector2<std::int8_t>(x, y));
			}
		}

		// Fill in new grid space for height
		for (int x = 0; x < new_width; x++)
		{
			for (int y = prev_height; y < new_height; y++)
			{
				grid[x][y] = new Node(sf::Vector2<std::int8_t>(x, y));
			}
		}
	}

	fix_dest_start_positions();
	update_grid_layout();
}


// Sets the size and position of each Node/rectangle on the grid
void Grid::update_grid_layout()
{

	for (int x = 0; x < grid_width; x++)
	{
		for (int y = 0; y < grid_height; y++)
		{
			grid[x][y]->set_rect_size(sf::Vector2f(get_rect_width(), get_rect_height()));
			grid[x][y]->set_rect_position(window->mapPixelToCoords(sf::Vector2i(get_rect_width() * x, get_rect_height() * y)));
		}
	}
}


// This function draws each rectangle in the grid to the window member variable
void Grid::draw_grid()
{
	for (int x = 0; x < grid_width; x++)
	{
		for (int y = 0; y < grid_height; y++)
		{
			window->draw(grid[x][y]->get_rectangle());
		}
	}
}

// calculate each node's s_cost, f_cost and t_cost
void Grid::calculate_all_h_cost()
{
	for (int i = 0; i < grid_width; i++)
	{
		for (int j = 0; j < grid_height; j++)
		{
			grid[i][j]->calculate_h(destination_pos);
		}
	}
}

// check if the destination_pos and start_pos are within the bounds of the grid
// and if they aren't set the position to -1, -1
void Grid::fix_dest_start_positions()
{
	if (destination_pos.x > grid_width || destination_pos.y > grid_height)
		destination_pos = sf::Vector2<std::int8_t>(-1, -1);
	
	if (start_pos.x > grid_width || start_pos.y > grid_height)
		start_pos = sf::Vector2<std::int8_t>(-1, -1);
}

// A* Algorithm for finding the fastest path between the start_pos and destination_pos
void Grid::run_a_star()
{
	// Data Sets
	std::vector<Node*> open;
	std::vector<Node*> closed;
	std::stack<sf::Vector2<std::int8_t>> path;

	Node* start_node = new Node(start_pos);
	Node* destination_node = new Node(destination_pos);
	Node* current_node;

	running = true; // it is necessary to set the running flag to true so that user input is 
					// not accepted while the algorithm is being run

	// Check if the function is able to be run
	if (destination_pos.x == -1 || start_pos.x == -1)
	{
		std::cout << "Please input the start and destination position before attempting to run the a* path finder\n";
		running = false;
		return; // exit out of function
	}

	// Necesssary so that all of the costs can be compared between nodes
	calculate_all_h_cost();

	open.push_back(start_node);

	while (!open.empty())
	{
		current_node = open[0];
		if(slow_solve)
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		int current_index = 0, index = 0;
		for (Node* node : open)
		{
			if (node->f < current_node->f)
			{
				current_node = node;
				current_index = index;
			}
			index++;
		}

		open.erase(open.begin() + current_index);

		bool skip_push_back = false;
		for (Node* closed_node : closed)
		{
			if (current_node->get_pos() == closed_node->get_pos())
			{
				skip_push_back = true;
				break;
			}

		}

		if (!skip_push_back)
		{
			grid[current_node->get_pos().x][current_node->get_pos().y]->set_state(NodeState::visited);
			closed.push_back(current_node);
		}

		// Found the destination
		if (current_node->get_pos() == destination_node->get_pos())
		{
			while (current_node != NULL)
			{
				path.push(current_node->get_pos());
				current_node = current_node->parent;
			}
			draw_path(path);
			for (int i = open.size() - 1; i >= 0; i--)
			{
				delete open[i];
				open[i] = NULL;
			}
			for (int i = closed.size() - 1; i >= 0; i--)
			{
				delete closed[i];
				closed[i] = NULL;
			}
			running = false;
			return;
		}

		std::vector<Node*> children;
		std::int16_t curr_x = current_node->get_pos().x;
		std::int16_t curr_y = current_node->get_pos().y;

		// Verticals
		if (curr_y + 1 < grid_height && grid[curr_x][curr_y + 1]->get_state() != NodeState::wall)
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x, curr_y + 1), current_node));
		}
		if (curr_y - 1 >= 0 && grid[curr_x][curr_y - 1]->get_state() != NodeState::wall)
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x, curr_y - 1),current_node));
		}

		// Horizontals
		if (curr_x - 1 >= 0 && grid[curr_x - 1][curr_y]->get_state() != NodeState::wall)
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x - 1, curr_y), current_node));
		}
		if (curr_x + 1 < grid_width && grid[curr_x + 1][curr_y]->get_state() != NodeState::wall)
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x + 1, curr_y), current_node));
		}

		// Diagonals
		if (
			curr_x + 1 < grid_width && curr_y + 1 < grid_height &&
			grid[curr_x + 1][curr_y + 1]->get_state() != NodeState::wall
			)
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x + 1, curr_y + 1), current_node));
		}
		if (
			curr_x - 1 >= 0 && curr_y - 1 >= 0 &&
			grid[curr_x - 1][curr_y - 1]->get_state() != NodeState::wall
			) 
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x - 1, curr_y - 1), current_node));
		}
		if (curr_y - 1 >= 0 &&
			curr_x + 1 < grid_width &&
			grid[curr_x + 1][curr_y - 1]->get_state() != NodeState::wall
			)
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x + 1, curr_y - 1), current_node));
		}
		if (
			curr_x - 1 >= 0 &&
			curr_y + 1 < grid_height &&
			grid[curr_x - 1][curr_y + 1]->get_state() != NodeState::wall
			)
		{
			children.push_back(new Node(sf::Vector2<std::int8_t>(curr_x - 1, curr_y + 1), current_node));
		}

		// Loop through children
		for (Node* child : children)
		{
			bool skip_child = false;
			for (Node* closed_child : closed)
			{
				if (child->get_pos() == closed_child->get_pos())
				{
					delete child;
					skip_child = true;
					break;
				}
			}
			if (skip_child) continue;

			child->g = child->parent->g + 1;
			child->calculate_f();

			for (Node* open_node : open)
			{
				if (open_node->get_pos() == child->get_pos())
				{
					delete child;
					skip_child = true;
					break;
				}
			}

			if (skip_child) continue;

			grid[child->get_pos().x][child->get_pos().y]->set_state(NodeState::unvisited);
			open.push_back(child);
		}
	}
	for (int i = open.size() - 1; i >= 0; i--)
	{
		delete open[i];
		open[i] = NULL;
	}
	for (int i = closed.size() - 1; i >= 0; i--)
	{
		delete closed[i];
		closed[i] = NULL;
	}
	running = false;
}

void Grid::draw_path(std::stack<sf::Vector2<std::int8_t>> path)
{

	while (!path.empty())
	{
		grid[path.top().x][path.top().y]->set_state(NodeState::path);
		path.pop();
	}
}



void Grid::clear_grid()
{
	for (int i = 0; i < grid_width; i++)
	{
		for (int j = 0; j < grid_height; j++)
		{
			if(grid[i][j]->get_state() == NodeState::unvisited ||
				grid[i][j]->get_state() == NodeState::visited ||
				grid[i][j]->get_state() == NodeState::path)
				grid[i][j]->set_state(NodeState::open);
		}
	}
}

void Grid::normalize_node_color(sf::Vector2<std::int8_t> pos)
{
	grid[pos.x][pos.y]->set_state(grid[pos.x][pos.y]->get_state());
}

void Grid::draw_window()
{
	window->clear();
	draw_grid();
	window->display();
}

// Setters
void Grid::set_node_state(sf::Vector2<std::int8_t> pos, NodeState state)
{
	grid[pos.x][pos.y]->set_state(state);
	if (state == NodeState::start) {

		// sets the old start position as NodeState::open instead of NodeState::start
		if ((start_pos.x != -1 || start_pos.y != -1) && pos != start_pos)
			grid[start_pos.x][start_pos.y]->set_state(NodeState::open);

		start_pos = pos;
	}
	else if (state == NodeState::destination) {

		// sets the old destination position as NodeState::open instead of NodeState::destination
		if ((destination_pos.x != -1 || destination_pos.y != -1 ) && pos != destination_pos)
			grid[destination_pos.x][destination_pos.y]->set_state(NodeState::open);

		destination_pos = pos;
	}
}

void Grid::set_node_color(sf::Vector2<std::int8_t> pos,sf::Color color)
{
	grid[pos.x][pos.y]->set_rect_color(color);
}

void Grid::set_running(bool running)
{
	this->running = running;
}

void Grid::toggle_slow_solve()
{
	slow_solve = !slow_solve;
}



// Getters
const bool Grid::is_running()
{
	return running;
}

const int Grid::get_width()
{
	return grid_width;
}


const NodeState Grid::get_node_state(sf::Vector2<std::int8_t> pos)
{
    return grid[pos.x][pos.y]->get_state();
}

const int Grid::get_height()
{
	return grid_height;
}

const int Grid::get_rect_width()
{
	return 25;
}

const int Grid::get_rect_height()
{
	return 25;
}

const sf::Vector2<std::int8_t> Grid::get_mouse_pos_in_grid(sf::Vector2i mouse_pos)
{
	int grid_x = mouse_pos.x / get_rect_width();
	int grid_y = mouse_pos.y / get_rect_height();

	return sf::Vector2<std::int8_t>(grid_x, grid_y);
}


// Test Functions

void Grid::random_grid_setup()
{
	if (running) return;
	init_grid();

	for (int x = 0; x < grid_width; x++)
	{
		for (int y = 0; y < grid_height; y++)
		{

			if (!(std::rand() % 3))
			{
				set_node_state(sf::Vector2<int8_t>(x, y), NodeState::wall);
			}
			
		}
	}

	sf::Vector2<int8_t> new_start_pos(std::rand() % grid_width, std::rand() % grid_height);
	set_node_state(new_start_pos, NodeState::start);

	sf::Vector2<int8_t> new_destination_pos(std::rand() % grid_width, std::rand() % grid_height);
	while (new_destination_pos == new_start_pos) new_destination_pos = sf::Vector2<int8_t>(std::rand() % grid_width, std::rand() % grid_height);
	set_node_state(new_destination_pos, NodeState::destination);

	update_grid_layout();
}


void Grid::run_qa_test()
{
	for (int i = 0; i < 1000; i++)
	{
		random_grid_setup();
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
		run_a_star();
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
		draw_window();
		running = false;
	}
}

