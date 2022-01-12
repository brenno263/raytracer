#pragma once

class TileCounter
{
public:
	int num_tiles;
	int current_tile;

	TileCounter(int num_tiles)
	{
		this->num_tiles = num_tiles;
		current_tile = 1;
	}

	void increment()
	{
		std::cerr << "finished tile " << current_tile << "/" << num_tiles << std::endl;
		current_tile++;
	}
};
