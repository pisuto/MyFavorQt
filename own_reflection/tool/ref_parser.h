#ifndef REF_PARSER_H
#define REF_PARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <functional>

namespace ref {

	struct type_descriptor;

	struct mapping {
		std::string value;
		int level;
		bool checked;
	};

	struct file_parser {
		using get_item_func_ptr = const void* (*)(const void*, size_t);

		file_parser(std::string name) : file_name(name), 
			is_read(false), is_wrote(false) {}
		
		virtual bool is_read_file() const { return is_read; }
		virtual bool is_write_file() const { return is_wrote; }

		virtual void read_file() = 0;
		virtual void write_file() = 0;
		virtual void write_file_finished() = 0;
		virtual void write_customize_type(type_descriptor*, const void*, int) = 0;
		virtual void write_primitive_array_type(type_descriptor*, get_item_func_ptr, int, const void*, int) = 0;
		virtual void write_next_type() {}

		virtual mapping& operator[](std::string) = 0;

		/* ���ݲ�ͬ�����ͱ仯��ֵ��������д���˸����ģ�庯�������ֻ��һһд�� */
		virtual void write_primitive_type(std::string type_name, int type_value) = 0;
		virtual void write_primitive_type(std::string type_name, std::string type_value) = 0;
		virtual void write_primitive_type(std::string type_name, bool type_value) = 0;
		virtual void write_primitive_type(std::string type_name, size_t type_value) = 0;

		std::string file_name;
		bool is_read, is_wrote;
	};

	/* .ini .txt�ļ����� */
	class txt_parser : public file_parser {
	public:
		txt_parser(std::string name) : file_parser(name) {}

		virtual void write_customize_type(type_descriptor*, const void*, int) override;

		virtual void write_primitive_array_type(type_descriptor*, get_item_func_ptr, int, const void*, int) override;

		virtual void write_primitive_type(std::string type_name, int type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, std::string type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, bool type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, size_t type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		template<typename T>
		void write_primitive_type_template(std::string type_name, T type_value) {
			out << type_name << "{" << std::boolalpha << type_value << "}";
		}

		virtual void write_next_type() override {
			out << std::endl;
		}

		/* �ֲڵؽ����ļ���û�ж��ļ���ʽ�����ж� */
		virtual void read_file() override;

		virtual void write_file() override {
			dic.clear(); /* ÿһ�ζ���ʱ����գ���Ҫ���¶��� */
			out.open(file_name, std::ios::out);
			is_wrote = true;
			is_read = false;
		}

		virtual void write_file_finished() override {
			out.close();
			is_wrote = false;
		}

		/* �����ʼ���������˳���ȡ���� */
		mapping& operator[](std::string index) {
			for (auto it = dic.begin(); it != dic.end(); it++)
			{
				if (index == it->first && !it->second.checked) {
					return it->second;
				}
			}
			return dic.back().second; /* ��Ҫ����һ��ֵ */
		}

	private:
		std::ifstream in;
		std::ofstream out;

		std::vector<std::pair<std::string, mapping>> dic;
	};

	
	/* console�����û�ж��빦�� */
	struct console_parser : file_parser {
		console_parser() : file_parser("") {}

		virtual void write_customize_type(type_descriptor*, const void*, int) override;
		
		virtual void write_primitive_array_type(type_descriptor*, get_item_func_ptr, int, const void*, int) override;

		virtual void write_primitive_type(std::string type_name, int type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, std::string type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, bool type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, size_t type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		template<typename T>
		void write_primitive_type_template(std::string type_name, T type_value) {
			std::cout << type_name << "{" << std::boolalpha << type_value << "}";
		}

		virtual void write_next_type() override {
			std::cout << std::endl;
		}

		virtual bool is_read_file() const override {
			std::cout << "ERROR: Cannot read this type, please use other parsers." << std::endl;
			return true;
		}

		/* ����̨���������� */
		virtual void read_file() override {}

		virtual void write_file() override {}

		virtual void write_file_finished() override {}

		mapping& operator[](std::string index) {
			static mapping null_map = { "",0,false };
			return null_map; /* ��Ҫ����һ��ֵ */
		}
	};

	/* xml�ṹ�壬���洢attr��ÿһ���ڵ�ֻ��Ϊ�������ӽڵ��text���򻯶�ȡ */
	enum class XML_STRU_TYPE {
		/* ������ */
		XML_ERROR_NO_RIGHT_ARROR,
		XML_ERROR_WRONG_NODE_NAME,
		XML_ERROR_UNKNOWN,

		/* �ڵ�ֵ */
		XML_NULL,		/* �սڵ� */
		XML_DOC,		/* ��ʼ�ڵ� */
		XML_HEAD,		/* ͷ�ڵ㣬'<node>' */
		XML_TAIL,		/* β�ڵ㣬'</node>' */
		XML_NODE,       /* �м�ڵ㣬'<node></node>' */
		XML_PCDATA,		/* ���ݣ�'<node>text</node>' */
		XML_CDATA,		/* <![CDATA[text]]> */
		XML_COMMENT,	/* '<!-- text -->' */
		XML_DECL,		/* '<?xml version="1.0"?>' */
	};

	/* level��xml������ײ㿪ʼ���� */
	struct xml_text {
		xml_text() : name(""), value{ "", 0, false } {}
		std::string name;
		mapping value;
	};

	struct xml_node {
		xml_text text;
		std::list<xml_node> xml_nodes;

		/* ���ںͶ����null_node���бȽ� */
		bool operator==(const xml_node& rhs) {
			if (text.name == rhs.text.name &&
				text.value.value == rhs.text.value.value &&
				text.value.level == rhs.text.value.level &&
				text.value.checked == rhs.text.value.checked &&
				xml_nodes.size() == rhs.xml_nodes.size()) {
				return true;
			}
			return false;
		}

		bool operator!=(const xml_node& rhs) {
			return !(*this == rhs);
		}
	};

	struct xml_doc {
		std::list<xml_node> xml_nodes;
	};

	struct xml_tiny_tool {

		void parser_xml(std::string& line, xml_doc& doc) {
			auto type = get_xml_type(line, doc);
			switch (type)
			{
			case ref::XML_STRU_TYPE::XML_ERROR_NO_RIGHT_ARROR:
				std::cout << "ERROR: .xml file lacks right arrow!" << std::endl;
				break;
			case ref::XML_STRU_TYPE::XML_ERROR_WRONG_NODE_NAME:
				std::cout << "ERROR: .xml file has error tag!" << std::endl;
				break;
			case ref::XML_STRU_TYPE::XML_ERROR_UNKNOWN:
				std::cout << "ERROR: .xml file encounters unknown error!" << std::endl;
				break;
			case ref::XML_STRU_TYPE::XML_NULL:
			case ref::XML_STRU_TYPE::XML_DOC:
			case ref::XML_STRU_TYPE::XML_HEAD:
			case ref::XML_STRU_TYPE::XML_TAIL:
			case ref::XML_STRU_TYPE::XML_NODE:
			case ref::XML_STRU_TYPE::XML_PCDATA:
			case ref::XML_STRU_TYPE::XML_CDATA:
			case ref::XML_STRU_TYPE::XML_COMMENT:
			case ref::XML_STRU_TYPE::XML_DECL:
			default:
				break;
			}
		}

	private:

		XML_STRU_TYPE get_xml_type(std::string& line, xml_doc& doc);

		std::stack<xml_node> xml_store_nodes;
	};

	/* xml�ļ����� */
	class xml_parser : public file_parser {
	public:

		struct show_condition {
			std::string var_name;
			bool is_array;
		};

		xml_parser(std::string name) : file_parser(name), condition{"", false} {
			null_node = {};
			null_node.text.value.level = -1;
		}

		virtual void write_customize_type(type_descriptor*, const void*, int) override;
		
		virtual void write_primitive_array_type(type_descriptor*, get_item_func_ptr, int, const void*, int) override;

		virtual void write_primitive_type(std::string type_name, int type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, std::string type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, bool type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		virtual void write_primitive_type(std::string type_name, size_t type_value) {
			write_primitive_type_template(type_name, type_value);
		}

		template<typename T>
		void write_primitive_type_template(std::string type_name, T type_value) {
			out << std::boolalpha << type_value;
		}

		virtual void read_file() override {
			in.open(file_name, std::ios::in);
			std::string line;
			xml_tiny_tool tool;
			while (std::getline(in, line)) {
				tool.parser_xml(line, xml_file);
			}
			in.close();
			is_read = true;
			return;
		}

		virtual void write_file() override {
			out.open(file_name, std::ios::out);
			out << "<?xml version=\"1.0\"?>" << std::endl << "<reflection>" << std::endl;
			is_wrote = true;
			is_read = false;
		}

		virtual void write_file_finished() override {
			out << "</reflection>" << std::endl;
			out.close();
			is_wrote = false;
			if (!check_if_valid_xml_finished()) {
				std::cout << "ERROR: Output .xml file has formal error!" << std::endl;
			}
		}

		mapping& operator[](std::string index) {
			for (auto& node : xml_file.xml_nodes) {
				auto& result = check_if_unchecked_node(index, node);
				if (result != null_node) {
					return result.text.value;
				}
			}
			return null_node.text.value;
		}

	private:

		bool check_if_valid_xml_finished() {
			return end_arr_names.empty();
		}

		/* ʹ�õݹ�ķ������б��� */
		xml_node& check_if_unchecked_node(std::string& index, xml_node& node) {
			if (!node.text.value.checked && !node.text.value.value.empty() && index == node.text.name) {
				return node;
			}
			for (auto& sub : node.xml_nodes) {
				auto& result = check_if_unchecked_node(index, sub);
				if (result != null_node) {
					return result;
				}
			}
			return null_node;
		}

		std::ifstream in;
		std::ofstream out;
		xml_doc xml_file;

		/* ����.xmlʱ���ڴ洢��������ջ */
		std::stack<std::string> end_arr_names;

		/* ���ڹر���ʾ�ڵ����ƣ��������ͳ��� */
		show_condition condition;

		xml_node null_node;
	};
}


#endif
