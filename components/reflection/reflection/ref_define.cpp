#include "ref_define.h"

using namespace std;

namespace ref {

	template<>
	int string_to_data<int>(std::string data) {
		return data.empty()? 0 : std::stoi(data);
	}

	template<>
	std::string string_to_data<std::string>(std::string data) {
		return data == NULL_VALUE ? "" : data;
	}

	template<>
	bool string_to_data<bool>(std::string data) {
		return data == "true";
	}

	template<>
	size_t string_to_data<size_t>(std::string data) {
		if (data.empty() || data.front() == '-') return 0;
		return static_cast<size_t>(std::stoi(data));
	}

	REFLECT_TYPE(int)
	REFLECT_TYPE(size_t)
	REFLECT_TYPE(bool)
	REFLECT_TYPE(string)

}