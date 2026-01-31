#pragma once

#include "types.hpp"
#include <shlobj.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

namespace krash {

class Cleaner {
private:
    static int cleanDir(const std::string& path, bool recursive = true) {
        WIN32_FIND_DATAA fd;
        std::string searchPath = path + "\\*";
        HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
        int deleted = 0;

        if (hFind == INVALID_HANDLE_VALUE) return 0;

        do {
            if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) continue;
            
            std::string fullPath = path + "\\" + fd.cFileName;

            if (fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) continue;

            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (recursive) {
                    deleted += cleanDir(fullPath, true);
                    RemoveDirectoryA(fullPath.c_str());
                }
            } else {
                SetFileAttributesA(fullPath.c_str(), FILE_ATTRIBUTE_NORMAL);
                if (DeleteFileA(fullPath.c_str())) {
                    deleted++;
                }
            }
        } while (FindNextFileA(hFind, &fd));

        FindClose(hFind);
        return deleted;
    }

public:
    static CleanResult cleanTemp() {
        CleanResult r = {0, 0};
        char path[MAX_PATH];
        GetTempPathA(MAX_PATH, path);
        r.filesDeleted = cleanDir(path);
        
        char userTemp[MAX_PATH];
        ExpandEnvironmentStringsA("%USERPROFILE%\\AppData\\Local\\Temp", userTemp, MAX_PATH);
        r.filesDeleted += cleanDir(userTemp);
        
        return r;
    }

    static CleanResult cleanPrefetch() {
        CleanResult r = {0, 0};
        r.filesDeleted = cleanDir("C:\\Windows\\Prefetch");
        return r;
    }

    static CleanResult cleanBrowsers() {
        CleanResult r = {0, 0};
        char local[MAX_PATH], roaming[MAX_PATH];

        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local))) {
            r.filesDeleted += cleanDir(std::string(local) + "\\Google\\Chrome\\User Data\\Default\\Cache");
            r.filesDeleted += cleanDir(std::string(local) + "\\Google\\Chrome\\User Data\\Default\\Code Cache");
            r.filesDeleted += cleanDir(std::string(local) + "\\Google\\Chrome\\User Data\\Default\\GPUCache");
            r.filesDeleted += cleanDir(std::string(local) + "\\Microsoft\\Edge\\User Data\\Default\\Cache");
            r.filesDeleted += cleanDir(std::string(local) + "\\Microsoft\\Edge\\User Data\\Default\\Code Cache");
            r.filesDeleted += cleanDir(std::string(local) + "\\Opera Software\\Opera Stable\\Cache");
            r.filesDeleted += cleanDir(std::string(local) + "\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Cache");
            r.filesDeleted += cleanDir(std::string(local) + "\\Yandex\\YandexBrowser\\User Data\\Default\\Cache");
        }

        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, roaming))) {
            r.filesDeleted += cleanDir(std::string(roaming) + "\\Mozilla\\Firefox\\Profiles");
        }

        return r;
    }

    static CleanResult cleanWindows() {
        CleanResult r = {0, 0};
        
        char local[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local))) {
            r.filesDeleted += cleanDir(std::string(local) + "\\CrashDumps");
            r.filesDeleted += cleanDir(std::string(local) + "\\Microsoft\\Windows\\INetCache");
            r.filesDeleted += cleanDir(std::string(local) + "\\Microsoft\\Windows\\WER");
        }
        
        return r;
    }

    static CleanResult cleanRecycleBin() {
        CleanResult r = {0, 0};
        if (SUCCEEDED(SHEmptyRecycleBinA(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND)))
            r.filesDeleted = 1;
        return r;
    }

    static CleanResult cleanThumbnails() {
        CleanResult r = {0, 0};
        char local[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local))) {
            r.filesDeleted += cleanDir(std::string(local) + "\\Microsoft\\Windows\\Explorer\\thumbcache_*.db", false);
        }
        return r;
    }

    static CleanResult cleanNvidia() {
        CleanResult r = {0, 0};
        char local[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local))) {
            r.filesDeleted += cleanDir(std::string(local) + "\\NVIDIA\\DXCache");
            r.filesDeleted += cleanDir(std::string(local) + "\\NVIDIA\\GLCache");
        }
        r.filesDeleted += cleanDir("C:\\ProgramData\\NVIDIA Corporation\\NV_Cache");
        return r;
    }

    static CleanResult cleanAMD() {
        CleanResult r = {0, 0};
        char local[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local))) {
            r.filesDeleted += cleanDir(std::string(local) + "\\AMD\\DxCache");
            r.filesDeleted += cleanDir(std::string(local) + "\\AMD\\GLCache");
        }
        return r;
    }

    static CleanResult cleanAll() {
        CleanResult total = {0, 0};
        CleanResult r;

        r = cleanTemp(); total.filesDeleted += r.filesDeleted;
        r = cleanBrowsers(); total.filesDeleted += r.filesDeleted;
        r = cleanPrefetch(); total.filesDeleted += r.filesDeleted;
        r = cleanWindows(); total.filesDeleted += r.filesDeleted;
        r = cleanNvidia(); total.filesDeleted += r.filesDeleted;
        r = cleanAMD(); total.filesDeleted += r.filesDeleted;

        return total;
    }

    static void runDiskCleanup() {
        system("cleanmgr /sagerun:1");
    }
};

}
