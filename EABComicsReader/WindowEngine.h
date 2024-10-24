#pragma once

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include "miniz.h"
namespace fs = std::filesystem;

enum class ScaleMode {
	Width,
	Height,
	Both
};

class WindowEngine
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Reader objects
	std::vector<sf::Texture> pages; // To hold comic pages
	size_t currentPage; // Index of the current page
	ScaleMode scaleMode; // sets the scaling

	//Private funcs
	void initVariables();
	void initWindow();
	void drawCurrentPage(); // New function

public:

	//Const Dest

	WindowEngine();
	virtual ~WindowEngine();

	//Accessors
	const bool running() const;



	//Funcs
	void update();
	void updateView();
	void render();


	//paging
	void nextPageset();
	void lastPageset();
	void setScaleMode(ScaleMode mode);
	void loadComic(const std::string& path); // New function
};

