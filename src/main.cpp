#include "Drawing.h"
#include "UI.h"
#include "GameData.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <iostream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::uintptr_t hexStringToAddress(const std::string& str) {
    std::uintptr_t addr = 0;
    // std::stoull корректно обрабатывает префикс 0x
    try {
        addr = std::stoull(str, nullptr, 0); // 0 значит auto-detect base
    } catch (...) {
        addr = 0;
    }
    return addr;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    CURL* curl = curl_easy_init();
    if (!curl) return 1;
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    std::string readBuffer;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Authorization: JXIOJ92DNDU92HDKLDNHXWIUD");

    curl_easy_setopt(curl, CURLOPT_URL, "https://loader-backend.vercel.app/sc-data");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    try {
        auto json = nlohmann::json::parse(readBuffer);

        Globals::chamsAddr = hexStringToAddress(json["chams"]);
        Globals::wallhackAddr = hexStringToAddress(json["wallhack"]);
        Chams::Init(Globals::baseAddress, Globals::chamsAddr);
        Wallhack::Init(Globals::baseAddress, Globals::wallhackAddr);

    } catch (...) {
        OutputDebugStringA("Failed to parse JSON\n"); 
    }

    Drawing::init();
    UI::Render();

    return 0;
}
