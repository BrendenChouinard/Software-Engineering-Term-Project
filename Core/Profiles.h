#ifndef PROFILES_H
#define PROFILES_H

#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>

class profile {
private:
	std::string username;
	std::string password;

	std::unordered_map<std::string, std::string> user_data;

	void write_string(std::ofstream& stream, const std::string& string);
	void read_string(std::ifstream& stream, std::string& string);
	
	void save_to_file(std::ofstream& stream);
	void load_from_file(std::ifstream& stream);

	profile();
	profile(const std::string& username, const std::string& password);

public:
	const std::string& get_username();
	const std::string& get_password();
	bool update_carbon_source(const std::string& source, const std::string& value);
	std::string get_carbon_from_source(const std::string& source);

	friend class profile_manager;
};






class profile_manager {
private:
	std::unordered_map<std::string, profile*> profiles_list;

	void save_profiles();
	void load_profiles();

public:
	 bool create_profile(const std::string& username, const std::string& password);

	 profile* login(const std::string& username, const std::string& password);

	 profile_manager();

	 ~profile_manager();
};

#endif PROFILES_H

