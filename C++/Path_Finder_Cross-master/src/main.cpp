#include <SFML/Graphics.hpp>
#include "Grid.h"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <thread>
#include <SFML/Audio.hpp>
#include <string>


int main()
{
	std::srand(std::time(0));
	NodeState brushState = NodeState::open;
	sf::RenderWindow window(sf::VideoMode(500, 500), "Path Finder", sf::Style::Close);
    
	sf::Vector2<std::int8_t> mouse_pos = sf::Vector2<std::int8_t>(0, 0);
	sf::Vector2<std::int8_t> last_pos = sf::Vector2<std::int8_t>(0,0);

    std::string file_name = "../../../res/startsound.wav";
    
     sf::Music music;

	 sf::SoundBuffer buffer;
	 buffer.loadFromFile(file_name);

	 sf::Sound sound;

	 sound.setBuffer(buffer);
     
     if (!music.openFromFile("../../../res/RSL_120_synth_bass_ukg_organ_filter_Gm.wav"))
     {
        std::cout << "Error with the music file!" << std::endl;
     }
     
    music.play();
    music.setLoop(true);
    
	Grid grid(&window, 20, 20);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	std::thread t1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Event for if the window is closed
			if(event.type == sf::Event::Closed)
				window.close();

			// Event for if the window is resized
			if(event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}

			// Event for KeyPressed
			if(event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{

				case(sf::Keyboard::X): // X: Open

					brushState = NodeState::open;
					break;

				case(sf::Keyboard::W): // W: Wall

					brushState = NodeState::wall;
					break;

				case(sf::Keyboard::S): // S: Start

					brushState = NodeState::start;
					break;

				case(sf::Keyboard::D): // D: Destination

					brushState = NodeState::destination;
					break;
				
				case(sf::Keyboard::Tab):
					grid.toggle_slow_solve();
					break;

				case(sf::Keyboard::Down): // Down arrow: Make grid larger vertically (limit at 100)

					// Upper Bounderies for y component
					if (grid.get_height() + 1 <= 100)
					{
						grid.resize(grid.get_width(), grid.get_height() + 1);
						window.setSize(sf::Vector2u(window.getSize().x, window.getSize().y + grid.get_rect_height()));
					}
					break;

				case(sf::Keyboard::Up): // Up arrow: Make grid smaller vertically (limit at 5)

					// Lower Bounderies for y component
					if (grid.get_height() - 1 >= 5)
					{
						grid.resize(grid.get_width(), grid.get_height() - 1);
						window.setSize(sf::Vector2u(window.getSize().x, window.getSize().y - grid.get_rect_height()));
					}

					break;
				case(sf::Keyboard::Right): // Right arrow: Make grid larger horizontally (limit at 100)

					// Upper Bounderies for x component
					if (grid.get_width() + 1 <= 100)
					{
						grid.resize(grid.get_width() + 1, grid.get_height());
						window.setSize(sf::Vector2u(window.getSize().x + grid.get_rect_width(), window.getSize().y));
					}

					break;
				case(sf::Keyboard::Left): // Left arrow: Make grid smaller horizontally (limit at 5)

					// Lower Bounderies for x component
					if (grid.get_width() - 1 >= 5)
					{
						grid.resize(grid.get_width() - 1, grid.get_height());
						window.setSize(sf::Vector2u(window.getSize().x - grid.get_rect_width(), window.getSize().y));
					}
					break;

				case(sf::Keyboard::R):
					if (!grid.is_running())
					{
						grid.random_grid_setup();
						grid.set_running(false);
					}
					break;

				case(sf::Keyboard::T):
					grid.set_running(false);
					grid.run_qa_test();
					break;

				case(sf::Keyboard::Enter): // Enter/Return: start the program
					if (!grid.is_running())
					{

						t1 = std::thread(&Grid::run_a_star, &grid);
						t1.detach();

					}
					break;

				case(sf::Keyboard::C):
					grid.set_running(false);
					grid.clear_grid();
					break;

				case(sf::Keyboard::I):
					grid.set_running(false);
					grid.init_grid();
					break;
				}
			}
			if (!grid.is_running())
			{
				sf::Vector2i mouse_pos_window = sf::Mouse::getPosition(window);
				int x = mouse_pos_window.x;
				int y = mouse_pos_window.y;
				if (x >= window.getSize().x)
				{
					x = window.getSize().x - 1;
				}
				if (y >= window.getSize().y)
				{
					y = window.getSize().y-1;
				}
				mouse_pos = grid.get_mouse_pos_in_grid(sf::Vector2i(x, y));
				grid.set_node_color(mouse_pos, sf::Color::Yellow);

				if (last_pos.x >= grid.get_width() || last_pos.y >= grid.get_height())
				{
					last_pos = mouse_pos;
				}

				if (last_pos != mouse_pos)
				{
					grid.normalize_node_color(last_pos);
				}

				last_pos = mouse_pos;
			}

			// Set up
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !grid.is_running())
			{

				sf::Vector2i mouse_pos_window = sf::Mouse::getPosition(window);
				int x = mouse_pos_window.x;
				int y = mouse_pos_window.y;
				if (x >= window.getSize().x)
				{
					x = window.getSize().x - 1;
				}
				if (y >= window.getSize().y)
				{
					y = window.getSize().y - 1;
				}
				mouse_pos = grid.get_mouse_pos_in_grid(sf::Vector2i(x, y));
				grid.set_node_state(mouse_pos, brushState);
                if(brushState == NodeState::wall) sound.play();

			}

			
		}

		window.clear();
		grid.draw_grid();
		window.display();
	}

	return 0;

}
