#include <iostream>
#include <Windows.h>
#include <filesystem>
#include  <direct.h>

std::string path;
bool recursive;
int depth = 0;

std::string pad() {
    std::string ret;

    for (int i = 0; i < depth; i++)
        ret.append("  ");

    return ret;
}

int get_dir_count(const std::filesystem::directory_entry& entry) {
    int ret = 0;
    for (const auto& fn : std::filesystem::directory_iterator(entry.path().string()))
        ret++;

    return ret;
}

std::string get_text(const std::filesystem::directory_entry& entry) {
    std::string ret = pad().append(entry.path().string());

    if (entry.is_directory())
        ret = ret.append("\\ [").append(std::to_string(get_dir_count(entry))).append("]");

    return ret;
}

void loop(std::string path) {
    for (const auto& fn : std::filesystem::directory_iterator(path)) {
        try {
            std::cout << get_text(fn) << std::endl;

            if (recursive && fn.is_directory())
                depth++, loop(fn.path().string()), depth--;
        }
        catch (const std::exception& ex) {
            // probably access denied
            continue;
        }
    }
}

int main(int argc, char** argv) {
    path = _getcwd(nullptr, NULL);
    recursive = false;

    for (int i = 0; i < argc; i++) {
        if (strstr(argv[i], "-p")) path = argv[i + 1];
        if (strstr(argv[i], "-r")) recursive = true;
    }

    loop(path);
}