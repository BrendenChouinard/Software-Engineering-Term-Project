#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>

#ifndef PROFILES_H
#define PROFILES_H
class profile {
private:
	std::string username;
	std::string password;

public:
	const std::string& get_username();
	const std::string& get_password();
	profile(const std::string& username, const std::string& password);
};

class profiles {
private:
	std::unordered_map<std::string, profile*> profiles_list;

	void write_string(std::ofstream& stream, const std::string& string);
	void read_string(std::ifstream& stream, std::string& string);

	void save_profiles();
	void load_profiles();

public:
	 profile* create_profile(const std::string& username, const std::string& password);

	 profile* login(const std::string& username, const std::string& password);

	 profiles();

	 ~profiles();
};
#endif //PROFILES_H
