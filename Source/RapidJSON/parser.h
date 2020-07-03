#pragma once
#include <string>
#include <fstream>

inline std::string Parse(const std::string& aPath)
{
	std::string json;
	std::ifstream file(aPath);
	if (file.is_open())
	{
		std::string line;
		while (getline(file, line))
		{
			json.append(line);
		}
		file.close();
	}
	return json;
}