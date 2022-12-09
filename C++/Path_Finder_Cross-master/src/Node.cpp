#pragma once
#include <iostream>
#include <cmath>
#include "Node.h"

// Constructor
Node::Node(sf::Vector2<std::int8_t> pos, Node* parent)
{
	this->parent = parent;
	this->pos = pos;
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(1.0f);
	set_state(NodeState::open);
	g = 0;
	h = 0;
	f = 0;
}


// Calculators
void Node::calculate_h(sf::Vector2<std::int8_t> destination)
{
	// the deltas are the distances between the respective coordinate
	float delta_x = destination.x - pos.x;
	float delta_y = destination.y - pos.y;

	h = std::sqrt((delta_x * delta_x) + (delta_y * delta_y));
}

void Node::calculate_f()
{
	f = g + h;
}


void Node::set_pos(sf::Vector2<std::int8_t> pos)
{
	this->pos = pos;
}

// Setters
void Node::set_state(NodeState state)
{
	this->state = state;
	switch (state)
	{
	case NodeState::open:
		rect.setFillColor(sf::Color(190, 190, 190));
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1.0f);
		break;
	case NodeState::wall:
		rect.setFillColor(sf::Color(10, 10, 10));
		rect.setOutlineColor(sf::Color(10,10,10));
		rect.setOutlineThickness(1.0f);
		break;
	case NodeState::start:
		rect.setFillColor(sf::Color::Blue);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1.0f);
		break;
	case NodeState::destination:
		rect.setFillColor(sf::Color::Magenta);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1.0f);
		break;
	case NodeState::path:
		rect.setFillColor(sf::Color::Blue);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1.0f);
		break;
	case NodeState::visited:
		rect.setFillColor(sf::Color(190, 0, 0));
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1.0f);
		break;
	case NodeState::unvisited:
		rect.setFillColor(sf::Color(0, 190, 0));
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1.0f);
		break;
	}
}

void Node::set_rect_color(sf::Color color)
{
	rect.setFillColor(color);
}

void Node::set_rect_position(sf::Vector2f position)
{
	rect.setPosition(position);
}

void Node::set_rect_size(sf::Vector2f size)
{
	rect.setSize(size);
}



// Getters
const sf::Vector2<std::int8_t> Node::get_pos()
{
	return pos;
}

const NodeState Node::get_state()
{
	return state;
}

const sf::Color Node::get_rect_color()
{
	return rect.getFillColor();
}

const sf::Vector2f Node::get_rect_position()
{
	return rect.getPosition();
}

const sf::Vector2f Node::get_rect_size()
{
	return rect.getSize();
}

const sf::RectangleShape Node::get_rectangle()
{
	return rect;
}
