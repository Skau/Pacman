#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
class ImageManager
{
public:
	ImageManager();
	~ImageManager();

	void addImage(sf::Image& image) { imageList.push_back(image); }
	sf::Image& getImage(int index) { return imageList[index]; }
private:
	std::vector<sf::Image> imageList;


};

