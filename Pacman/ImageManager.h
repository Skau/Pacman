#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class ImageManager
{
public:
	ImageManager();
	~ImageManager()=default;


	sf::Image& getImage(int index) { return imageList[index]; }

private:
	void loadImages();
	
	std::vector<sf::Image> imageList;
};

