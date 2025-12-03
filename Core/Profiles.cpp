#include "Profiles.h"

#include <fstream>
#include <iostream>


/**
profile class
*/
void profile::write_string(std::ofstream& stream, const std::string& string)
{
	size_t string_len = string.size() + 1; // need to make room for 1 extra character (the null terminator)
	stream.write(reinterpret_cast<const char*>(&string_len), sizeof(string_len));

	stream.write(string.c_str(), string_len);
}

void profile::read_string(std::ifstream& stream, std::string& string)
{
	size_t string_len;
	stream.read(reinterpret_cast<char*>(&string_len), sizeof(string_len));

	char* c_string = new char[string_len];
	stream.read(c_string, string_len);
	string = std::string(c_string);
	delete[] c_string;
}

void profile::save_to_file(std::ofstream& stream)
{
	write_string(stream, this->username);
	write_string(stream, this->password);

	size_t length = user_data.size();
	stream.write(reinterpret_cast<const char*>(&length), sizeof(length));
	for (const auto& pair : user_data) {
		write_string(stream, pair.first);
		stream.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
	}
}

void profile::load_from_file(std::ifstream& stream)
{
	read_string(stream, this->username);
	read_string(stream, this->password);

	size_t length;
	stream.read(reinterpret_cast<char*>(&length), sizeof(length));
	for (decltype(length) i = 0; i < length; i++) {
		std::string carbon_source;
		float value;
		read_string(stream, carbon_source);
		stream.read(reinterpret_cast<char*>(&value), sizeof(value));

		add_carbon_source(carbon_source, value);
	}
}

const std::string& profile::get_username()
{
	return this->username;
}

const std::string& profile::get_password()
{
	return this->password;
}

/** 
* Defines a new source of carbon for this user.
* 
* Arguments: name of the carbon source and an amount.
*/
void profile::add_carbon_source(const std::string& source, const float& amount)
{
	user_data.emplace(source, amount);
}

/**
* Gets the amount of carbon this user produces from a given source.
*
* Arguments: name of the carbon source.
* 
* Returns: the amount of carbon produced from this source (defaults to 0).
*/
float profile::get_carbon_from_source(const std::string& source)
{
	if (user_data.contains(source)) return user_data.at(source);

	return 0.0f;
}

profile::profile()
{
}

profile::profile(const std::string& username, const std::string& password)
{
	this->username = username;
	this->password = password;
}





// profile_manager class


void profile_manager::save_profiles()
{
	std::ofstream output;
	output.open("carbon_profiles.dat", std::ios::binary);

	if (output.is_open()) {
		size_t length = profiles_list.size();
		output.write(reinterpret_cast<const char*>(&length), sizeof(length));
		for (const auto& pair : profiles_list) {
			pair.second->save_to_file(output);
		}
		output.close();
	}
	else {
		std::cerr << "Error saving profiles: Could not open data file!" << std::endl;
	}
}

void profile_manager::load_profiles()
{
	std::ifstream input;
	input.open("carbon_profiles.dat", std::ios::binary);
	
	if (input.is_open()) {
		size_t length;
		input.read(reinterpret_cast<char*>(&length), sizeof(length));
		for (decltype(length) i = 0; i < length; i++) {
			profile* newProfile = new profile();
			newProfile->load_from_file(input);
			profiles_list.emplace(newProfile->username, newProfile);
		}
	}
	else {
		std::cerr << "Could not read from data file! Either this was the first time the program is run, or there is an issue!" << std::endl;
	}
}

/**
* Creates a new user profile.
*
* Arguments: profile username and profile password.
*
* Returns: returns true if the profile was created, false if it wasn't (profile already existed).
*/
bool profile_manager::create_profile(const std::string& username, const std::string& password)
{
	if (profiles_list.contains(username)) return false; // this profile name is already taken, so we will not create a new one

	profiles_list.emplace(username, new profile(username, password));
	return true;
}

/**
* Attempts to log in to a user profile using the given username and password.
*
* Arguments: profile username and profile password.
*
* Returns: a pointer to the created profile, or nullptr if no profile with this username and password exists.
*/
profile* profile_manager::login(const std::string& username, const std::string& password)
{
	if (profiles_list.contains(username)) {
		profile* profile = profiles_list.at(username);
		if (profile->get_password() == password) return profile;
	}

	return nullptr;
}

profile_manager::profile_manager()
{
	load_profiles();
}

profile_manager::~profile_manager()
{
	save_profiles();

	// free all memory
	for (const auto& pair : profiles_list) {
		delete pair.second;
	}
	profiles_list.clear();
}

