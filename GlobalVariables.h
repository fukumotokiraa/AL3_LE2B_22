#pragma once
#include"Calculation.h"

#include<variant>
#include<map>
#include<string>

class GlobalVariables {
public:
	static GlobalVariables* GetInstance();

	void Update();

	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	void SetValue(const std::string& groupName, const std::string& key, float value);

	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);


private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;
	struct Item {
		std::variant<int32_t, float, Vector3> value;
	};
	struct Group {
		std::map<std::string, Item> items;
	};
	std::map<std::string, Group> datas_;

};


