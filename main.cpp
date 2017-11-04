#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <future>


using namespace std;
namespace fs = boost::filesystem;

string EXPLORER_LOG_PATH = "log.txt";

//Подсчет размера директории
double getDirSize(string dirAddress, double size) {
    fs::path dirPath = dirAddress;
    fs::path textDir = dirPath.filename();
    cout << "Finding size of " << textDir << ". Please, wait...\n";
    if(fs::exists(dirPath)){
      fs::directory_iterator end_itr;
      for(fs::directory_iterator dirIte(dirPath); dirIte != end_itr; ++dirIte) {
        fs::path filePath(dirIte->path());
        try {
          if(!fs::is_directory(dirIte->status()))
          {
            size = size + fs::file_size(filePath);
          }
          else {
            getDirSize(filePath.string(), size);
          }
        }
        catch(exception& e){
          cout << e.what() << endl;
        }
        //Проверка на то, является ли файл, размер которого мы хотим узнать, директорией
      }
    }
    return size;
}

string getDirName(string dirAddress) {
    cout << "Finding name of file or directory. Please, wait...\n" << endl;
    fs::path dirPath = dirAddress;
    fs::path textDir = dirPath.filename();
    return textDir.string();
}

string getDirData(string dirAddress) {
    cout << "Finding latest modification data. Please, wait...\n" << endl;
    fs::path dirPath = dirAddress;
    std::time_t ftime = fs::last_write_time(dirPath);
    return asctime(gmtime(&ftime));
}

int putIntoFile(string logPath, string dirName, string dirData, double dirSize) {
    ofstream stream;
    stream.open(logPath, ofstream::out | ofstream::app);
    stream << "Directory name: " << dirName << endl;
    stream << "Latest modification: " << dirData << endl;
    stream << "Directory size: " << dirSize / 1000 << " Kb" << endl;
    stream << "---------------------------------" << endl;
    stream.close();
    return 0;
}

void displayDir(string dirAddress) {
    //Проверка на существование директории
    if(!fs::exists(dirAddress)) {
      throw runtime_error("ERROR: We don't have such directory");
    }
    //Проверка на то, является ли файл директорией
    if(!fs::is_directory(dirAddress)) {
      throw runtime_error("ERROR: Type of file - not directory");
    }

    std::packaged_task<string(string)> taskName(getDirName);
    std::packaged_task<string(string)> taskData(getDirData);
    std::packaged_task<double(string, double)> taskSize(getDirSize);
    std::packaged_task<int(string, string, string, double)> taskFile(putIntoFile);

    std::future<string> dirName = taskName.get_future();
    std::future<string> dirData = taskData.get_future();
    std::future<double> dirSize = taskSize.get_future();
    std::future<int> putInto = taskFile.get_future();

    taskName(dirAddress);
    taskData(dirAddress);
    taskSize(dirAddress, 0);
    taskFile(EXPLORER_LOG_PATH, dirName.get(), dirData.get(), dirSize.get());

}


int main(int argc, char* argv[])
{
  string ad;

  try {
    for(int i = 1; i < argc; i++){
      ad = argv[i];
      displayDir(argv[i]);
    }
  }
  catch(const exception& e) {
    cout << e.what() << endl;
  }
}
