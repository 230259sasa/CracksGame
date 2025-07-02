#include<string>
#include<corecrt_wstring.h>
#include <nlohmann/json.hpp>
#include<filesystem>
#include<fstream>

namespace JsonReader {
	template<typename T>
	T Get(std::string _key, std::string _nestedKey, T& _value)
	{
		std::string fileName = "Data\\config.json";
		std::ifstream ifs(fileName.c_str());
		if (ifs.good()) {
			nlohmann::json j;
			ifs >> j;
			//errorMessage
			if (!j.contains(_key) || !j[_key].contains(_nestedKey)) {
				std::string message;
				if (!j.contains(_key)) {
					message += _key + "キーが存在しません";
				}
				else {
					message += _key + "の" + _nestedKey + "キーが存在しません";
				}
				MessageBox(NULL, message.c_str(), "エラー", MB_OK);
			}
			_value = j[_key][_nestedKey].get<T>();
		}
		return _value;
	}
	template<typename T>
	T Get(std::string _key, std::string _nestedKey) {
		T v;
		return Get(_key, _nestedKey, v);
	}
}