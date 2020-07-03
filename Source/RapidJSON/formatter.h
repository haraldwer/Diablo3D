#pragma once
#include <string>

inline std::string Format(std::string aJson, bool aDoTabs = true)
{
	std::string formatted;
	int offset = 0;
	int totalIndent = 0;
	bool inString = false;
	for (int i = 0; i < aJson.size(); i++)
	{
		if (aJson[i] == '\"')
		{
			if (inString)
			{
				int j = i - 1;
				while (j >= 0 && aJson[j] == '\\')
				{
					--j;
				}
				if (((i - 1) - j) % 2 == 0)
				{
					inString = false;
				}
			}
			else
			{
				inString = true;
			}
		}
		if (!inString)
		{
			if (aJson[i] == ',' || aJson[i] == '{' || aJson[i] == '}') // add new line
			{
				formatted.append(aJson.substr(offset, i - offset));

				if (aJson[i] == '}')
				{
					totalIndent--;
					formatted.append("\n");
					for (int j = 0; j < totalIndent; j++)
					{
						formatted.append("\t");
					}
				}

				formatted.append({ aJson[i] });
				offset = i + 1;

				if (aJson[i] == '{')
				{
					totalIndent++;
				}

				if (aJson[i] != '}')
				{
					formatted.append("\n");
					for (int j = 0; j < totalIndent; j++)
					{
						if (aDoTabs)
							formatted.append("\t");
					}
				}
			}
		}
	}
	return formatted;
}
