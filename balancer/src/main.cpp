#include <drogon/drogon.h>
#include <filesystem>
using namespace drogon;
namespace fs = std::filesystem;

int main() {
    app()
        .setLogPath(fs::absolute(std::getenv("DROGON_CONFIG")).parent_path())
        .loadConfigFile(std::getenv("DROGON_CONFIG"))
        .run();
    return 0;
}
