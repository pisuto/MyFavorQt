#include "../tool/ref_parser.h"
#include "../reflection/ref_define.h"

namespace ref {

	bool check_if_native_type(const TYPE_CLASSIFY& type_class) {
		return type_class == TYPE_CLASSIFY::TYPE_PRIMITIVE ||
			type_class == TYPE_CLASSIFY::TYPE_STL;
	}

	bool check_if_customize_type(const TYPE_CLASSIFY& type_class) {
		return type_class == TYPE_CLASSIFY::TYPE_CUSTOMIZE;
	}

	void txt_parser::write_customize_type(type_descriptor* type_desc, const void* obj, int level)
	{
		out << type_desc->full_name() << " {" << std::endl;
		std::vector<type_member> members;
		type_desc->type_members(members);
		for (const type_member& member : members) {
			out << std::string(4 * (level + 1), ' ') << member.var_name << " = ";
			member.type_desc->serialize(this, (char*)obj + member.offset, level + 1);
			out << std::endl;
		}
		out << std::string(4 * level, ' ') << "}";
	}

	void txt_parser::write_primitive_array_type(type_descriptor* type_desc, get_item_func_ptr get_item_func, int size, const void* obj, int level)
	{
		out << type_desc->full_name();
		if (size == 0) {
			out << "[0] {}";
		}
		else {
			out << "[" << size << "]" << " {" << std::endl;
			for (size_t index = 0; index < size; ++index) {
				out << std::string(4 * (level + 1), ' ');
				type_desc->serialize(this, get_item_func(obj, index), level + 1);
				out << std::endl;
			}
			out << std::string(4 * level, ' ') << "}";
		}
	}

	void console_parser::write_customize_type(type_descriptor* type_desc, const void* obj, int level)
	{
		std::cout << type_desc->full_name() << " {" << std::endl;
		std::vector<type_member> members;
		type_desc->type_members(members);
		for (const type_member& member : members) {
			std::cout << std::string(4 * (level + 1), ' ') << member.var_name << " = ";
			member.type_desc->serialize(this, (char*)obj + member.offset, level + 1);
			std::cout << std::endl;
		}
		std::cout << std::string(4 * level, ' ') << "}";
	}

	void console_parser::write_primitive_array_type(type_descriptor* type_desc, get_item_func_ptr get_item_func, int size, const void* obj, int level)
	{
		std::cout << type_desc->full_name();
		if (size == 0) {
			std::cout << "[0] {}";
			return;
		}
		else {
			std::cout << "[" << size << "]" << " {" << std::endl;
			for (size_t index = 0; index < size; ++index) {
				std::cout << std::string(4 * (level + 1), ' ');
				type_desc->serialize(this, get_item_func(obj, index), level + 1);
				std::cout << std::endl;
			}
			std::cout << std::string(4 * level, ' ') << "}";
		}
	}

	void xml_parser::write_customize_type(type_descriptor*  type_desc, const void* obj, int level)
	{
        bool var_exist = true;
		/* ��var_name��ʾ���ǽṹ�ĳ�Ա����������ʾ�������ʹ���ļ�̫�����ӣ�ͬʱ�迼���������� */
        if (condition.end_var_names.empty() || condition.is_array) {
			out << std::string(4 * level, ' ') << "<" << type_desc->full_name() << ">" << std::endl;
            var_exist = false;
            condition.is_array = false;
		}
		std::vector<type_member> members;
		type_desc->type_members(members);
		for (const type_member& member : members) {
			out << std::string(4 * (level + 1), ' ') << "<" << member.var_name << ">";
			/* ��������Ԫ��Ϊ0ʱ������������ʾ����һ�� */
			if (check_if_customize_type(member.type_desc->type_class)) {
				out << std::endl;
			}
            condition.end_var_names.push(member.var_name);
			member.type_desc->serialize(this, (char*)obj + member.offset, level + 1);
            condition.end_var_names.pop();
			if (check_if_customize_type(member.type_desc->type_class)) {
				out << std::string(4 * (level + 1), ' ');
			}
			out << "</" << member.var_name << ">" << std::endl;
		}
        if (!var_exist) {
			out << std::string(4 * level, ' ') << "</" << type_desc->full_name() << ">" << std::endl;
		}

	}

	void xml_parser::write_primitive_array_type(type_descriptor* type_desc, get_item_func_ptr get_item_func, int size, const void* obj, int level)
	{
		if (size == 0) {
			return;
		}
		bool first_time = false; /* ȥ�����з���Ӱ�� */
		out << std::endl;
		for (size_t index = 0; index < size; ++index) {
			/* ��������Ԫ�ص�����������ʾ��ʽ���Զ�������һ�� */
			if (check_if_native_type(type_desc->type_class)) {
				if (first_time) {
					out << std::endl;
				}
				out << std::string(4 * (level + 1), ' ')
					<< "<" << type_desc->full_name() << ">";
				first_time = true;
			}
            condition.is_array = true;
			type_desc->serialize(this, get_item_func(obj, index), level + 1);
			if (check_if_native_type(type_desc->type_class)) {
				out << "</" << type_desc->full_name() << ">";
			}
		}
		if (check_if_native_type(type_desc->type_class)) {
			out << std::endl;
		}
		out << std::string(4 * (level), ' ');
		condition.is_array = false;
	}

	void txt_parser::read_file()
	{
		in.open(file_name, std::ios::in);
		std::string line;
		int level = 0;
		while (std::getline(in, line))
		{
			std::string name;
			{
				auto pos1 = line.find_first_not_of(" ");
				auto pos2 = line.find_first_of(" ", pos1);
				name = line.substr(pos1, pos2 - pos1);
			}

			std::string value;
			{
				auto pos1 = line.find_first_of("{");
				auto pos2 = line.find_first_of("}");
				if (pos1 != std::string::npos && pos2 != std::string::npos)
				{
					value = line.substr(pos1 + 1, pos2 - pos1 - 1);
					auto pos1 = value.find_first_not_of(" ");
					auto pos2 = value.find_last_not_of(" ");
					if (pos1 == std::string::npos && pos2 == std::string::npos)
					{
						/* ͨ��[]��ȡ�������͵Ĵ�С */
						pos1 = line.find_first_of("[");
						pos2 = line.find_first_of("]");
						if (pos1 != std::string::npos && pos2 != std::string::npos)
						{
							value = line.substr(pos1 + 1, pos2 - pos1 - 1);
							dic.emplace_back(std::make_pair(name, mapping{ value, level, false }));
						}
						else
						{
							dic.emplace_back(std::make_pair(name, mapping{ "", level, false }));
						}
					}
					else
					{
						value = value.substr(pos1, pos2 - pos1 + 1);
						dic.emplace_back(std::make_pair(name, mapping{ value, level, false }));
					}
				}
				else if (pos2 == std::string::npos)
				{
					pos1 = line.find_first_of("[");
					pos2 = line.find_first_of("]");
					if (pos1 != std::string::npos && pos2 != std::string::npos)
					{
						value = line.substr(pos1 + 1, pos2 - pos1 - 1);
						dic.emplace_back(std::make_pair(name, mapping{ value, level, false }));
					}
					else
					{
						dic.emplace_back(std::make_pair(name, mapping{ "", level, false }));
					}
					++level;
				}
			}
		}
		/* ��ֹȡ��������û�з���ֵ����˼���һ��null����һ���ò��� */
		dic.emplace_back(std::make_pair("null", mapping{ "", 0, false }));
		in.close();
		is_read = true;
	}

	XML_STRU_TYPE xml_tiny_tool::get_xml_type(std::string& line, xml_doc& doc)
	{
		if (line.empty()) {
			return XML_STRU_TYPE::XML_NULL;
		}
		xml_node node;
		auto las /* left arrow start */ = line.find_first_of('<');
		if (las != std::string::npos) {
			if (las + 1 < line.length() && line[las + 1] == '?') {
				return XML_STRU_TYPE::XML_DECL;
			}
			auto lae /* left arrow end */ = line.find_first_of('>', las);
			if (lae != std::string::npos) {
				if (line[las + 1] != '/') {
					node.text.name = line.substr(las + 1, lae - las - 1);
                    auto ras /* right arrow start */ = line.find_first_of("<", lae);
                    if (ras != std::string::npos && line[ras + 1] == '/') {
						auto rae /* right arrow end */ = line.find_first_of('>', ras);
						if (rae != std::string::npos) {
							/* ��ȡ��<>...</>�ڵ�ʱ����ѹ����һ�����ڵ� */
							auto node_name = line.substr(ras + 2, rae - ras - 2);
							if (node_name == node.text.name) {
								std::string value = lae + 1 != ras ? line.substr(lae + 1, ras - lae - 1) : NULL_VALUE;
								node.text.value = { value, 0, false };
								xml_store_nodes.top().xml_nodes.push_back(node);
								return XML_STRU_TYPE::XML_NODE;
							}
							return XML_STRU_TYPE::XML_ERROR_WRONG_NODE_NAME;
						}
					}
					else {
						/* ������<>��ǩʱ�����µĽڵ�ѹ��ջ */
						xml_store_nodes.push(node);
						return XML_STRU_TYPE::XML_HEAD;
					}
				}
				else {
					/* ����</>��ǩʱ�����һ���ڵ㵯����ѹ�븸�ڵ� */
					if (xml_store_nodes.empty()) {
						return XML_STRU_TYPE::XML_ERROR_UNKNOWN;
					}
					auto temp = xml_store_nodes.top();
					temp.text.value.value = std::to_string(temp.xml_nodes.size());
					node.text.name = line.substr(las + 2, lae - las - 2);
					node.text.value.level = xml_store_nodes.size() - 1;
					if (temp.text.name == node.text.name) {
						if (xml_store_nodes.size() == 1) {
							doc.xml_nodes.push_back(xml_store_nodes.top());
						}
						xml_store_nodes.pop();
						if (!xml_store_nodes.empty()) {
							auto& top = xml_store_nodes.top();
							top.xml_nodes.push_back(temp);
							top.text.value.value = std::to_string(top.xml_nodes.size());
							top.text.value.level = temp.text.value.level + 1;
						}
						return XML_STRU_TYPE::XML_TAIL;
					}
					return XML_STRU_TYPE::XML_ERROR_WRONG_NODE_NAME;
				}
			}
			else {
				return XML_STRU_TYPE::XML_ERROR_NO_RIGHT_ARROR;
			}
		}
		return XML_STRU_TYPE::XML_ERROR_UNKNOWN;
	}

}

