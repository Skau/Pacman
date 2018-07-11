#include "ImageManager.h"
#include <iostream>


ImageManager::ImageManager()
{
	loadImages();
}

void ImageManager::loadImages()
{
	sf::Image image;

	if (!image.loadFromFile("images/yellowImage.png"))
	{
		std::cout << "Failed to load yellow image!" << std::endl;
		return;
	}
	imageList.push_back(image);
	if (!image.loadFromFile("images/orangeImage.png"))
	{
		std::cout << "Failed to load orange image!" << std::endl;
		return;
	}
	imageList.push_back(image);
	if (!image.loadFromFile("images/purpleImage.png"))
	{
		std::cout << "Failed to load purple image!" << std::endl;
		return;
	}
	imageList.push_back(image);
	if (!image.loadFromFile("images/redImage.png"))
	{
		std::cout << "Failed to load red image!" << std::endl;
		return;
	}
	imageList.push_back(image);
	if (!image.loadFromFile("images/pinkImage.png"))
	{
		std::cout << "Failed to load pink image!" << std::endl;
		return;
	}
	imageList.push_back(image);
	if (!image.loadFromFile("images/whiteImage.png"))
	{
		std::cout << "Failed to load white image!" << std::endl;
		return;
	}
	imageList.push_back(image);
	if (!image.loadFromFile("images/lightBlueImage.png"))
	{
		std::cout << "Failed to load light blue image!" << std::endl;
		return;
	}
	imageList.push_back(image);
}

