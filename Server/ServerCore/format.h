#ifndef FORMAT_H
#define FORMAT_H
#include <initializer_list>

namespace Utils {
	namespace {
		static void ReplaceString(string& input, uint32 i, const string& val)
		{
			string target = "{" + to_string(i) + "}";
			uint64 pos = input.find(target);

			while (true)
			{
				if (pos == string::npos)
					break;

				input.erase(pos, target.length());
				input.insert(pos, val);
				pos = input.find(target, pos + val.length());
			}
		}

		static void ReplacewString(wstring& input, uint32 i, const wstring& val)
		{
			wstring target = L"{" + to_wstring(i) + L"}";
			uint64 pos = input.find(target);

			while (true)
			{
				if (pos == wstring::npos)
					break;

				input.erase(pos, target.length());
				input.insert(pos, val);
				pos = input.find(target, pos + val.length());
			}
		}
	}
	static string format(const string& input, const initializer_list<string>& args) {
		string result = input;
		uint32 i = 0;

		for (auto& str : args)
		{
			ReplaceString(result, i, str);
			i++;
		}
		return result;
	}
	static wstring wformat(const wstring& input, const initializer_list<wstring>& args) {
		wstring result = input;
		uint32 i = 0;

		for (auto& str : args)
		{
			ReplacewString(result, i, str);
			i++;
		}
		return result;
	}
}
#endif