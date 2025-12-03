#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <unordered_map>
#include <string>

class calculator {
private:
	const std::unordered_map<std::string, float> weights = {
		{"GasUsageInput", 19.8f},
		{"WalkingInput", 0.3747858f},
		{"BusInput", 0.395f},
		{"BikeInput", 0.07f},
		{"FlyingInput", 53.0f},

		{"HousePowerInput", 0.81f},
		{"NaturalGasInput", 0.12f},

		{"BottledWaterInput", 0.49604f},
		{"TapWaterInput", 0.02204623f},
		{"ShowerCountInput", 5.67f},

		{"BeefInput", 39.0f},
		{"PorkInput", 7.7f},
		{"ChickenInput", 11.0f},
		{"MuttonInput", 50.0f},
		{"BreadInput", 2.2f},
		{"RiceInput", 4.545f},
		{"FruitInput", 1.1089252f},
		{"VegetableInput", 0.563f}
	};;

	std::unordered_map<std::string, float> values;
	float sum = 0.0f;
	std::unordered_map<std::string, float> percentages;


public:
	calculator();

	bool calculate(std::unordered_map<std::string, std::string> parameters);

	const std::unordered_map<std::string, float>& get_values();
	const float& get_sum();
	const std::unordered_map<std::string, float>& get_percentages();
};

#endif CALCULATOR_H
