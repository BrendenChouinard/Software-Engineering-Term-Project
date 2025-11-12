#include "Profiles.h"

#include <fstream>
#include <iostream>

void profiles::write_string(std::ofstream& stream, const std::string& string)
{
	size_t string_len = string.size() + 1; // need to make room for 1 extra character (the null terminator)
	stream.write(reinterpret_cast<const char*>(&string_len), sizeof(string_len));

	stream.write(string.c_str(), string_len);
}

void profiles::read_string(std::ifstream& stream, std::string& string)
{
	size_t string_len;
	stream.read(reinterpret_cast<char*>(&string_len), sizeof(string_len));

	char* c_string = new char[string_len];
	stream.read(c_string, string_len);
	string = std::string(c_string);
	delete[] c_string;
}

void profiles::save_profiles()
{
	std::ofstream output;
	output.open("carbon_profiles.dat", std::ios::binary);

	if (output.is_open()) {
		size_t length = profiles_list.size();
		output.write(reinterpret_cast<const char*>(&length), sizeof(length));
		for (const auto& pair : profiles_list) {
			write_string(output, pair.second->get_username());
			write_string(output, pair.second->get_password());
		}
		output.close();
	}
	else {
		std::cerr << "Could not open data file! Either this was the first time the program is run, or there is an issue!" << std::endl;
	}
}

void profiles::load_profiles()
{
	std::ifstream input;
	input.open("carbon_profiles.dat", std::ios::binary);
	
	if (input.is_open()) {
		size_t length;
		input.read(reinterpret_cast<char*>(&length), sizeof(length));
		for (int i = 0; i < length; i++) {
			std::string username;
			std::string password;
			read_string(input, username);
			read_string(input, password);

			create_profile(username, password);
		}
	}
	else {
		std::cerr << "Error loading profiles: could not open data file!" << std::endl;
	}
}

/**
* Creates a new user profile.
*
* Arguments: profile username and profile password.
*
* Returns: a pointer to the created profile, or nullptr if a profile with this name already existed.
*/
profile* profiles::create_profile(const std::string& username, const std::string& password)
{
	if (profiles_list.contains(username)) return nullptr; // this profile name is already taken, so we will not create a new one

	profile* newProfile = new profile(username, password);
	profiles_list.emplace(username, newProfile);
	return newProfile;
}

/**
* Attempts to log in to a user profile using the given username and password.
*
* Arguments: profile username and profile password.
*
* Returns: a pointer to the created profile, or nullptr if no profile with this username and password exists.
*/
profile* profiles::login(const std::string& username, const std::string& password)
{
	if (profiles_list.contains(username)) {
		profile* profile = profiles_list.at(username);
		if (profile->get_password() == password) return profile;
	}

	return nullptr;
}

profiles::profiles()
{
	load_profiles();
}

profiles::~profiles()
{
	save_profiles();

	// free all memory
	for (const auto& pair : profiles_list) {
		delete pair.second;
	}
	profiles_list.clear();
}

const std::string& profile::get_username()
{
	return this->username;
}

const std::string& profile::get_password()
{
	return this->password;
}

profile::profile(const std::string& username, const std::string& password)
{
	this->username = username;
	this->password = password;
}
