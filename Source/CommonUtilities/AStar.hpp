#pragma once
#include <vector>
#include "Heap.hpp"
#include <functional>

namespace CommonUtilities
{
	static const int MapWidth = 20;
	static const int MapHeight = 20;
	static const int TileCount = MapWidth * MapHeight;
	static const float Weight = 1;
	static const float Heuristic = 1.0f;
	
	enum class Tile
	{
		Impassable,
		Passable
	};
	
	inline float Manhattan(const int aFrom, const int aTo)
	{
		const float startX = aFrom % MapWidth;
		const float startY = aFrom / MapWidth;
		const float endX = aTo % MapWidth;
		const float endY = aTo / MapWidth;
		return abs(startX - endX) + abs(startY - endY);
	}

	inline float Euclidean(const int aFrom, const int aTo)
	{
		const float startX = aFrom % MapWidth;
		const float startY = aFrom / MapWidth;
		const float endX = aTo % MapWidth;
		const float endY = aTo / MapWidth;
		const float rX = startX - endX;
		const float rY = startY - endY;
		return pow(rX * rX + rY * rY, 0.5f);
	}
	
	inline std::vector<int> AStarInternal(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex, std::vector<float>& aDistMap, std::vector<bool>& aVisitedMap)
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
		
		std::vector<int> predecessor;
		Heap<Vertex> toBeChecked;
		aVisitedMap.clear();
		aDistMap.clear();
		aDistMap.reserve(TileCount);
		aVisitedMap.reserve(TileCount);
		predecessor.reserve(TileCount);

		for (int i = 0; i < aMap.size(); i++)
		{
			aDistMap.push_back(i == aStartIndex ? 0 : maxDist);
			aVisitedMap.push_back(false);
			predecessor.push_back(-1);
		}

		Vertex v;
		v.index = aStartIndex;
		v.compare = [&, index = aStartIndex](const Vertex& other)
		{
			const float thisDist = aDistMap[index] + Manhattan(index, anEndIndex) * Heuristic;
			const float otherDist = aDistMap[other.index] + Manhattan(other.index, anEndIndex) * Heuristic;
			return thisDist > otherDist;
		};
		toBeChecked.Enqueue(v);

		while (toBeChecked.Size() > 0)
		{
			Vertex v = toBeChecked.Dequeue();
			if (v.index == anEndIndex)
				break;

			aVisitedMap[v.index] = true;
			std::vector<int> adjacent;
			int x = v.index % MapWidth;
			int y = v.index / MapWidth;
			
			// right
			if (x + 1 < MapWidth && !aVisitedMap[y * MapWidth + x + 1] && aMap[y * MapWidth + x + 1] == Tile::Passable)
				adjacent.push_back(y * MapWidth + x + 1);
			// left
			if (x - 1 >= 0 && !aVisitedMap[y * MapWidth + x - 1] && aMap[y * MapWidth + x - 1] == Tile::Passable)
				adjacent.push_back(y * MapWidth + x - 1);
			// top
			if (y - 1 >= 0 && !aVisitedMap[(y - 1) * MapWidth + x] && aMap[(y - 1) * MapWidth + x] == Tile::Passable)
				adjacent.push_back((y - 1) * MapWidth + x);
			// bottom
			if (y + 1 < MapHeight && !aVisitedMap[(y + 1) * MapWidth + x] && aMap[(y + 1) * MapWidth + x] == Tile::Passable)
				adjacent.push_back((y + 1) * MapWidth + x);

			for (int u : adjacent)
			{
				if (aDistMap[u] > aDistMap[v.index] + Weight)
				{
					aDistMap[u] = aDistMap[v.index] + Weight;
					predecessor[u] = v.index;
					// Add to heap
					Vertex v;
					v.index = u;
					v.compare = [&, index = u](const Vertex& other) {
						const float thisDist = aDistMap[index] + Euclidean(index, anEndIndex) * Heuristic;
						const float otherDist = aDistMap[other.index] + Euclidean(other.index, anEndIndex) * Heuristic;
						return thisDist > otherDist;
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


	inline std::vector<int> AStar(const std::vector<Tile>& aMap, const int aStartIndex, const int anEndIndex)
	{
		std::vector<float> dist;
		std::vector<bool> visited;
		return AStarInternal(aMap, aStartIndex, anEndIndex, dist, visited);
	}
}
