#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <future>
#include <map>
#include <utility>
#include <thread>

using namespace std;
namespace fs = boost::filesystem;


double getDirSize(const fs::path& p, double size) {
    fs::path dirPath = p;
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
      }
    }
    return size;
}

string getDirName(const fs::path& p) {
    cout << "Finding name of file or directory. Please, wait...\n" << endl;
    fs::path dirPath = p;
    fs::path textDir = dirPath.filename();
    return textDir.string();
}

string getDirData(const fs::path& p) {
    cout << "Finding latest modification data. Please, wait...\n" << endl;
    fs::path dirPath = p;
    std::time_t ftime = fs::last_write_time(dirPath);
    return asctime(gmtime(&ftime));
}

bool print_name(const fs::path& p) {
  fs::path dirPath = p;
  ofstream stream;
  stream.open(getenv("EXPLORER_LOG_PATH"), ofstream::out | ofstream::app);
  stream << "Directory name: " << getDirName(dirPath) << endl;
  stream.close();
  return true;
}

bool print_data(const fs::path& p) {
  fs::path dirPath = p;
  ofstream stream;
  stream.open(getenv("EXPLORER_LOG_PATH"), ofstream::out | ofstream::app);
  stream << "Directory data: " << getDirData(dirPath) << endl;
  stream.close();
  return true;
}

bool print_size(const fs::path& p) {
  fs::path dirPath = p;
  ofstream stream;
  stream.open(getenv("EXPLORER_LOG_PATH"), ofstream::out | ofstream::app);
  stream << "Directory size: " << getDirSize(dirPath, 0) / 1000 << " Kb" << endl;
  stream << "---------------------------------" << endl;
  stream.close();
  return true;
}


bool print_info(const fs::path& p) {
  bool is_ok = true;
  is_ok &= print_name(p);
  is_ok &= print_data(p);
  is_ok &= print_size(p);
  return is_ok;
}
