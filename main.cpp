#include "main.hpp"

int main(int argc, char* argv[]) {
  try {
    map<fs::path, future<bool>> results;
    for (int i = 1; i < argc; ++i) {
	    fs::path Path = argv[i];
      auto task = packaged_task<bool()>{ bind(print_info, Path) };
	    auto result = task.get_future();
	    thread task_td(move(task));
      task_td.join();
	    results.emplace(Path, move(result));
    }

    for(auto & p : results) {
      auto& result = p.second;
      auto is_ok = result.get();
      if (!is_ok) {
        cerr << "ERROR: FINDING INFORMATION OF DIRECTORY: " << p.first << endl;
      }
    }
  }
  catch(const exception& e) {
    cout << e.what() << endl;
  }
}


