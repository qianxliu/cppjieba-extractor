#include <iostream>
#include <experimental/filesystem>
#include "cppjieba/Jieba.hpp"
using namespace std;
namespace fs = std::experimental::filesystem;
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdlib.h>

int main()
{
	cout << "请耐心等待加载数据……" << endl;
	const char *const DICT_PATH = "./dict/jieba.dict.utf8";
	const char *const HMM_PATH = "./dict/hmm_model.utf8";
	const char *const USER_DICT_PATH = "./dict/user.dict.utf8";
	const char *const IDF_PATH = "./dict/idf.utf8";
	const char *const STOP_WORD_PATH = "./dict/stop_words.utf8";

	cppjieba::Jieba jieba(DICT_PATH,
						  HMM_PATH,
						  USER_DICT_PATH,
						  IDF_PATH,
						  STOP_WORD_PATH);

	fs::path p1 = fs::current_path().append("cppjieba");
	fs::path p2 = fs::current_path().append("dict");
	fs::path p3 = fs::current_path().append("limonp");
	fs::path p4 = fs::current_path().append("extractor.exe");

	system("chcp 65001");
	while (true)
	{
		cout << "请输入要提取词频文件的编号：" << endl;
		vector<fs::path> entry_name;
		int i = 1;
		for (const auto &entry : fs::directory_iterator(fs::current_path()))
		{
			if (p1.compare(entry) && p2.compare(entry) && p3.compare(entry) && p4.compare(entry))
			{
				std::cout << "[ " << i << " ]" << entry.path() << std::endl;
				i++;
				entry_name.push_back(entry.path());
			}
		}
		while (cin >> i)
		{
			if (entry_name.size() < i || i < 1)
				cout << "输入有问题，请重试！" << endl;
			else
				break;
		}
		fs::path &filename = entry_name[i - 1];
		cout << "选择的文件为" << filename << endl;
		string str;
		std::ifstream myfile{filename};
		if (myfile)
		{
			ostringstream ss;
			ss << myfile.rdbuf(); // reading data
			str = ss.str();
			str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
			myfile.close();
			cout << str;
		}
		cout << "文档如下：" << endl;
		cout << str << endl;

		while (true)
		{
			cout << "此方案为关键词抽取(根据权重和词频计算总值)：" << endl;
			int n;
			cout << "请输入要排名的前n位？按0键退出。" << endl;
			cin >> n;
			if (n == 0)
				break;
			while (n < 0)
			{
				cout << "不可为负数！" << endl;
				cin >> n;
			}
			const size_t topk = n;
			vector<cppjieba::KeywordExtractor::Word> keywordres;
			jieba.extractor.Extract(str, keywordres, topk);
			for (cppjieba::KeywordExtractor::Word keyword : keywordres)
				cout << keyword << endl;
		}
	}
	return 0;
}
