#include "WindowEngine.h"

void WindowEngine::initVariables()
{
	this->window = nullptr;
}

void WindowEngine::initWindow()
{
	this->videoMode.height = 800;
	this->videoMode.width = 900;
	this->window = new sf::RenderWindow(this->videoMode, "EABComicsReader", sf::Style::Default);
	
	
}


void WindowEngine::loadComic() {
	loadComicDialog(); // Call the dialog method to load the comic
}

void WindowEngine::loadComic(const std::string& path)
{
	//clear existing pages
	pages.clear();
	currentPage = 0;

	//open a zip
	mz_zip_archive zipArchive;
	memset(&zipArchive, 0, sizeof(zipArchive));
	if (!mz_zip_reader_init_file(&zipArchive, path.c_str(), 0)) {
		std::cerr << "Failed to open zip file: " << path << std::endl;
		return;
	}

	// Get the number of files in the zip
	int fileCount = (int)mz_zip_reader_get_num_files(&zipArchive);

	for (int i = 0; i < fileCount; i++) {
		mz_zip_archive_file_stat fileStat;
		if (mz_zip_reader_file_stat(&zipArchive, i, &fileStat)) {
			// Load image data
			size_t dataSize;
			void* data = mz_zip_reader_extract_to_heap(&zipArchive, i, &dataSize, 0);
			if (data) {
				sf::Texture texture;
				if (texture.loadFromMemory(data, dataSize)) {
					pages.push_back(texture);
				}
				mz_free(data);
			}
		}
	}

	mz_zip_reader_end(&zipArchive); // Close the zip file

	/*
	fs::path comicPath(path);
	for (const auto& entry : fs::directory_iterator(comicPath)) {
		if (entry.is_regular_file()) {
			sf::Texture texture;
			if (texture.loadFromFile(entry.path().string())) {
				pages.push_back(texture);
			}
		}
	}
	*/
}

void WindowEngine::loadComicDialog()
{
	std::wstring path = FileDialog::openFile();
	if (!path.empty()) {
		loadComic(std::string(path.begin(), path.end())); // Convert wstring to string
	}
}

void WindowEngine::updateView() {
	sf::View view = window->getDefaultView();
	view.setSize(window->getSize().x, window->getSize().y);
	view.setCenter(window->getSize().x / 2.f, window->getSize().y / 2.f);
	window->setView(view);
}

void WindowEngine::drawCurrentPage()
{
	if (!pages.empty() && currentPage < pages.size()) {
		sf::Sprite sprite;
		sprite.setTexture(pages[currentPage]); // Set the current page texture

		// Get the original texture size
		sf::Vector2u textureSize = pages[currentPage].getSize();

		// Calculate scale factors
		float scaleX = window->getSize().x / static_cast<float>(textureSize.x);
		float scaleY = window->getSize().y / static_cast<float>(textureSize.y);

		// Apply scaling based on the chosen mode
		switch (scaleMode) {
		case ScaleMode::Width:
			sprite.setScale(scaleX, scaleX);
			break;
		case ScaleMode::Height:
			sprite.setScale(scaleY, scaleY);
			break;
		case ScaleMode::Both:
			float scale = std::min(scaleX, scaleY);
			sprite.setScale(scale, scale);
			break;
		}

		// Center the sprite based on the window size after scaling
		sprite.setPosition(
			(window->getSize().x - sprite.getGlobalBounds().width) / 2,
			(window->getSize().y - sprite.getGlobalBounds().height) / 2
		);

		// Draw the sprite
		window->draw(sprite);
	}
}

WindowEngine::WindowEngine()
{
	this->initVariables();
	this ->initWindow();
	this->scaleMode = ScaleMode::Both; // Default scaling mode
}

WindowEngine::~WindowEngine()
{
	delete this->window;
}

const bool WindowEngine::running() const
{
	return this->window->isOpen();
}

void WindowEngine::update()
{
	//event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type) 
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::Resized:
			updateView();
			break;
		case sf::Event::KeyPressed:
			switch (this->ev.key.code) 
			{
			case sf::Keyboard::Escape:
				this->window->close();
				break;
			case sf::Keyboard::Right:
				nextPageset(); 
				break;
			case sf::Keyboard::Left:
				lastPageset(); 
				break;
			case sf::Keyboard::W: // Scale by Width
				setScaleMode(ScaleMode::Width);
				break;
			case sf::Keyboard::H: // Scale by Height
				setScaleMode(ScaleMode::Height);
				break;
			case sf::Keyboard::B: // Scale by Both
				setScaleMode(ScaleMode::Both);
				break;
			default:
				break; // Handle other keys if needed
			}
		}
	}
}

void WindowEngine::render()
{
	/*
	*	@return void
		Renders game objects
		- clear old frame
		- render objects
		- display frame in window
	*/
	
	this->window->clear();
	drawCurrentPage();
	this->window->display();
}

void WindowEngine::nextPageset()
{
	if (currentPage < pages.size() - 1) {
		currentPage++;
	}
}

void WindowEngine::lastPageset()
{
	if (currentPage > 0) {
		currentPage--;
	}
}

void WindowEngine::setScaleMode(ScaleMode mode)
{
	scaleMode = mode;
}
