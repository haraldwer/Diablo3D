#pragma once
#include <vector>
#include "Heap.hpp"
#include <functional>

namespace CommonUtilities
{
	static const int MapWidth = 50;
	static const int MapHeight = 50;
	static const int TileCount = MapWidth * MapHeight;
	static const float Weight = 1;
	
	enum class Tile
	{
		Impassable,
		Passable
	};

	inline std::vector<int> Dijkstra(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex)
	{
		struct Vertex
		{
			int index;
			std::function<bool(const Vertex&)> compare;
			bool operator<(const Vertex& other) const
			{
				return compare(other);
			}
		};

		if (aMap.size() != TileCount)
			return {};

		if (aStartIndex < 0 || aStartIndex >= TileCount || anEndIndex < 0 || anEndIndex >= TileCount)
			return {};
		
		const float maxDist = MapWidth * MapHeight;
		
		std::vector<float> dist;
		std::vector<bool> visited;
		std::vector<int> predecessor;
		Heap<Vertex> toBeChecked;
		dist.reserve(TileCount);
		visited.reserve(TileCount);
		predecessor.reserve(TileCount);
		
		for(int i = 0; i < aMap.size(); i++)
		{
			dist.push_back(i == aStartIndex ? 0 : maxDist);
			visited.push_back(false);
			predecessor.push_back(-1);
		}
		
		Vertex v;
		v.index = aStartIndex;
		v.compare = [&, index = aStartIndex](const Vertex& other)
		{
			return dist[index] > dist[other.index];
		};
		toBeChecked.Enqueue(v);
		
		while(toBeChecked.Size() > 0)
		{
			Vertex v = toBeChecked.Dequeue();
			if (v.index == anEndIndex)
				break;
			
			visited[v.index] = true;
			std::vector<int> adjacent;
			int x = v.index;          
			int y = 0;
			while (x >= MapWidth)
			{
				y++;
				x -= MapWidth;
			}
			// right
			if (x + 1 < MapWidth && !visited[y * MapWidth + x + 1] && aMap[y * MapWidth + x + 1] == Tile::Passable)
				adjacent.push_back(y * MapWidth + x + 1);
			// left
			if (x - 1 >= 0 && !visited[y * MapWidth + x - 1] && aMap[y * MapWidth + x - 1] == Tile::Passable)
				adjacent.push_back(y * MapWidth + x - 1);
			// top
			if (y - 1 >= 0 && !visited[(y - 1) * MapWidth + x] && aMap[(y - 1) * MapWidth + x] == Tile::Passable)
				adjacent.push_back((y - 1) * MapWidth + x);
			// bottom
			if (y + 1 < MapHeight && !visited[(y + 1) * MapWidth + x] && aMap[(y + 1) * MapWidth + x] == Tile::Passable)
				adjacent.push_back((y + 1) * MapWidth + x);

			for(int u : adjacent)
			{
				if(dist[u] > dist[v.index] + Weight)
				{
					dist[u] = dist[v.index] + Weight;
					predecessor[u] = v.index;
					// Add to heap
					Vertex v;
					v.index = u;
					v.compare = [&, index = u](const Vertex& other) {
						return dist[index] > dist[other.index];
					};
					toBeChecked.Enqueue(v);
				}
			}
		}

		std::vector<int> path;
		int currentIndex = anEndIndex;
		while (predecessor[currentIndex] != -1)
		{
			path.push_back(currentIndex);
			currentIndex = predecessor[currentIndex];
		}
		std::reverse(std::begin(path), std::end(path));
		return path;
	}
}
