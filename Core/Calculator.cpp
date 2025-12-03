#include "Calculator.h"

/**
* Updates the stored values and percentages with the values calculated from the specified sources. Will do nothing if sources are missing.
* 
* Arguments: a map specifying the sources and amounts.
* 
* Returns: true if all required sources were provided, false otherwise.
*/
bool calculator::calculate(std::unordered_map<std::string, std::string> parameters)
{
	if (parameters.size() != weights.size()) return false;

	// calculate sum
	int total = 0;
	std::unordered_map<std::string, float> newValues;
	for (const auto& pair : weights) {
		float product = 0.0f;
		if (parameters.find(pair.first) == parameters.end()) return false; // the parameters aren't full
		if (parameters[pair.first] != "")
		{
			product = pair.second * std::stoi(parameters[pair.first]);
		}
		newValues[pair.first] = product;
		total += product;
	}
	values = newValues;
	sum = total;

	// calculate percentages
	percentages.clear();
	for (const auto& pair : values) {
		percentages[pair.first] = ((float)pair.second) / sum;
	}

	return true;
}

const std::unordered_map<std::string, float>& calculator::get_values()
{
	return values;
}

const float& calculator::get_sum()
{
	return sum;
}

const std::unordered_map<std::string, float>& calculator::get_percentages()
{
	return percentages;
}

calculator::calculator()
{
	sum = 0.0f;
}