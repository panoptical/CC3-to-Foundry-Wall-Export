#include "Main.h"

// generates a random string
// call this with length 16 to generate wall IDs
// from https://github.com/InversePalindrome/Blog/tree/master/RandomString
std::string random_string(std::size_t length)
{
	const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, characters.size() - 1);

	std::string random_string;

	for (std::size_t i = 0; i < length; ++i)
	{
		random_string += characters[distribution(generator)];
	}

	return random_string;
}