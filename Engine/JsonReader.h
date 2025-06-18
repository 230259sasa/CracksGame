#include<string>
#include <nlohmann/json.hpp>
#include<filesystem>
#include<fstream>

namespace JsonReader {
	template<typename T>
	T Get(std::string _key, std::string _nestedKey,T& _value);
	template<typename T>
	T Get(std::string _key, std::string _nestedKey, T& _value)
	{
		std::string fileName = "Data\\config.json";
		std::ifstream ifs(fileName.c_str());
		if (ifs.good()) {
			nlohmann::json j;
			ifs >> j;
			_value = j[_key][_nestedKey];
		}
		float n = _value;
		return _value;
	}
}