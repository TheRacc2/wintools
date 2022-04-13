#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <direct.h>
#include <fstream>

std::string word;
std::string path;
bool recursive;

void search(std::string file) {
    std::ifstream stream(file);

    int i = 0;
    for (std::string line; getline(stream, line); i++) {
        if (line.contains(word.c_str()))
            std::cout << "[" << file << "] " <<
            "[" << line.find(word) << ", " << i << "] " <<
            line <<
            std::endl;
    }
}

void loop(std::string path) {
    for (const auto& fn : std::filesystem::directory_iterator(path)) {
        try {
            if (fn.is_directory()) {
                if (recursive)
                    loop(fn.path().string());
            }
            else
                search(fn.path().string()); // Yeah lets not create fstream to a folder
        }
        catch (const std::exception& ex) {
            // probably access denied
            continue;
        }
    }
}

int main(int argc, char** argv) {
    path = _getcwd(nullptr, NULL);
    word = argv[1]; // Don't ever call grep without a key
    recursive = false;

    for (int i = 0; i < argc; i++) {
        if (strstr(argv[i], "-p")) path = argv[i + 1];
        if (strstr(argv[i], "-r")) recursive = true;
    }

    loop(path);
}