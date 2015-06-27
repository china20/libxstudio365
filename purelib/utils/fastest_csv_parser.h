#ifndef _FASTEST_CSV_PARSER_H_
#define _FASTEST_CSV_PARSER_H_
#include <set>
#include <map>
#include <vector>
#include <unordered_map>
#include "purelib/utils/xxfsutility.h"
#include "purelib/utils/iconvw.h"

class fastest_csv_parser
{
public:
	void build_vec2d(const char* filename)
	{
		std::string buffer = fsutil::read_file_data(filename);

		const char* endl = buffer.c_str();
		const char* cursor = nullptr;

		do {
			std::vector<std::string> record;
			cursor = endl;
			endl = csv_parse_line(cursor, [&record](const char* v_start, const char* v_end){
				
				//std::string temp = std::string(v_start, v_end);

				//int value = strtoi_s(v_start, v_end - v_start);

				//auto gbks = iconvw::u2g(temp);
				//record.push_back(gbks);
			});
			csv_vec2d.push_back(std::move(record));
		} while ((endl - buffer.c_str()) < buffer.size());
	}

	std::vector<std::vector<std::string>> csv_vec2d; // test csv parse line

	/*
	* op prototype: op(const char* v_start, const char* v_end)
	*/
	template<typename _Fty> inline
	static	const char* csv_parse_line(const char* s, _Fty op)
	{
		enum {
			normal,
			explicit_string,
		} state;

		state = normal;

		const char* _Start = s; // the start of every string
		const char* _Ptr = s;   // source string iterator
		while (*_Ptr != '\r' && *_Ptr != '\n' && *_Ptr != '\0')
		{
			switch (*_Ptr)
			{
			case '"': // skip "
				if (state == normal) {
					state = explicit_string;
				}
				else {
					state = normal;
				}
				break;
			case ',':
				if (normal == state) {
					if (_Start <= _Ptr)
					op(_Start, _Ptr);
					_Start = _Ptr + 1;
					//if (*_Start == ',') // a empty col
					//	op(_Start, _Start);
				}
				break;
			}
			++_Ptr;
		}
		if (_Start <= _Ptr) {
			op(_Start, _Ptr);
		}

		if (*++_Ptr == '\n') // skip '\n', For CRLF endl
			++_Ptr;
		return _Ptr;
	}

};

#endif

