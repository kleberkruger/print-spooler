#pragma once

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <random>

inline std::string timeToString(std::chrono::system_clock::time_point time_point,
                                std::string format = "%Y-%m-%d %H:%M:%S") {
    auto show_milliseconds = false;
    if (auto pos = format.find("%s"); pos != std::string::npos) {
        format.erase(pos, 2);
        show_milliseconds = true;
    }

    auto time_t = std::chrono::system_clock::to_time_t(time_point);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), format.c_str());
    if (show_milliseconds) {
        auto duration = time_point.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
        ss << std::setfill('0') << std::setw(3) << milliseconds.count();
    }
    return ss.str();
}

inline int generateRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}



//#include <filesystem>
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <iostream>
//
//inline bool isPrintableFile(const std::filesystem::path &path) {
//    const std::vector<std::string> printableExtensions = {
//            // Documentos
//            ".txt", ".pdf", ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx", ".rtf", ".odt", ".ods", ".odp", ".csv",
//            ".tex", ".log",
//            // Imagens
//            ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg", ".ico",
//            // Código fonte
//            ".cpp", ".h", ".c", ".java", ".py", ".js", ".html", ".css", ".rb", ".go", ".swift", ".ts", ".php", ".sql",
//            ".sh", ".r", ".dart", ".kotlin", ".lua",
//            // Outros formatos
//            ".md", ".yaml", ".xml", ".json", ".properties", ".cfg", ".plist", ".schema", ".xaml", ".less", ".scss"
//    };
//
//    // Verifica se a extensão do arquivo está entre as extensões imprimíveis
//    return std::find(printableExtensions.begin(), printableExtensions.end(), path.extension().string()) !=
//           printableExtensions.end();
//}
//
//static inline std::filesystem::path getHomePath() {
//#ifdef _WIN32
//    const char* home = std::getenv("USERPROFILE"); // Windows
//#else
//    const char *home = std::getenv("HOME"); // Linux e macOS
//#endif
//    return home ? std::filesystem::path(home) : std::filesystem::path();
//}
//
//static inline bool isHidden(const std::filesystem::path &path) {
//#ifdef _WIN32
//    DWORD attributes = GetFileAttributes(path.c_str());
//    return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_HIDDEN);
//#else
//    return path.filename().string().starts_with(".");
//#endif
//}
//
//inline std::vector<std::string> findPrintableFiles(const std::filesystem::path &homePath, size_t maxFiles) {
//    std::vector<std::string> textFiles;
//    std::vector<std::string> imageFiles;
//    std::vector<std::string> codeFiles;
//    std::vector<std::string> otherFiles;
//
//    try {
//        for (const auto &entry: std::filesystem::recursive_directory_iterator(homePath)) {
//            if (entry.is_directory() && isHidden(entry.path())) {
//                continue; // Pula diretórios ocultos
//            }
//            if (entry.is_regular_file() && isPrintableFile(entry.path())) {
//                std::string filename = entry.path().filename().string();
//                std::string extension = entry.path().extension().string();
//
//                // Classifica os arquivos com base na extensão
//                if (extension == ".txt" || extension == ".pdf" || extension == ".doc" || extension == ".docx" ||
//                    extension == ".xls" || extension == ".xlsx" || extension == ".ppt" || extension == ".pptx" ||
//                    extension == ".rtf" || extension == ".odt" || extension == ".ods" || extension == ".csv" ||
//                    extension == ".tex" || extension == ".log") {
//                    textFiles.push_back(filename);
//                } else if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" ||
//                           extension == ".gif" || extension == ".bmp" || extension == ".svg" || extension == ".ico") {
//                    imageFiles.push_back(filename);
//                } else if (extension == ".cpp" || extension == ".h" || extension == ".c" || extension == ".java" ||
//                           extension == ".py" || extension == ".js" || extension == ".html" || extension == ".css" ||
//                           extension == ".rb" || extension == ".go" || extension == ".swift" || extension == ".ts" ||
//                           extension == ".php" || extension == ".sql" || extension == ".sh" || extension == ".r" ||
//                           extension == ".dart" || extension == ".kotlin" || extension == ".lua") {
//                    codeFiles.push_back(filename);
//                } else {
//                    otherFiles.push_back(filename);
//                }
//            }
//        }
//    } catch (const std::filesystem::filesystem_error &e) {
//        std::cerr << "Erro ao acessar diretórios: " << e.what() << std::endl;
//    }
//
//    // Seleciona 100 arquivos conforme a distribuição
//    std::vector<std::string> selectedFiles;
//    size_t totalFiles = 100;
//    size_t textCount = std::min<size_t>(textFiles.size(), totalFiles * 50 / 100);
//    size_t imageCount = std::min<size_t>(imageFiles.size(), totalFiles * 20 / 100);
//    size_t codeCount = std::min<size_t>(codeFiles.size(), totalFiles * 20 / 100);
//    size_t otherCount = std::min<size_t>(otherFiles.size(), totalFiles * 10 / 100);
//
//    // Adiciona os arquivos selecionados na ordem desejada
//    selectedFiles.insert(selectedFiles.end(), textFiles.begin(), textFiles.begin() + textCount);
//    selectedFiles.insert(selectedFiles.end(), imageFiles.begin(), imageFiles.begin() + imageCount);
//    selectedFiles.insert(selectedFiles.end(), codeFiles.begin(), codeFiles.begin() + codeCount);
//    selectedFiles.insert(selectedFiles.end(), otherFiles.begin(), otherFiles.begin() + otherCount);
//
//    return selectedFiles;
//}