// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#include "Common/Config.hpp"
#include "GCacheCore/RecursiveDirectoryIterator.hpp"
#include "GCacheCore/MD5.hpp"
#include <cstdint>
#include <string>
#include <fstream> // std::ifstream, std::ofstream
#include <algorithm> // std::min
#include <unordered_map>
#include <cstdio> // std::printf, std::puts
#include <cstring> // std::strchr

static std::string_view Trim(std::string_view s)
{
    char const *trimChars = " \"\t\r\v\n";
    s.remove_prefix(std::min(s.find_first_not_of(trimChars), s.size()));
    s.remove_suffix(std::min(s.size() - s.find_last_not_of(trimChars) - 1, s.size()));
    return s;
}

static bool Verbose = false;

template <typename... TArgs>
static void Log(char const *format, TArgs ...args)
{
    if (!Verbose)
    {
        if (!std::strchr("!-", format[0]))
            return;
    }
    std::printf(format, args...);
    std::puts("");
}

class CacheEntry
{
public:
    int64_t Timestamp;
    std::string Hash;

private:
    std::string_view ConsumeToken(std::string_view &src, bool consumeSpaces = false)
    {
        auto space = src.find_first_of(" \t");
        if (space == std::string::npos || consumeSpaces)
            space = src.length();
        auto token = src.substr(0, space);
        src = Trim(src.substr(space));
        return token;
    }

public:
    std::filesystem::path Load(std::ifstream &fs)
    {
        // "912309182 9283109238 git/libschmoo/schmoo.h"
        std::string line = "<empty line>";
        do
        {
            if (!std::getline(fs, line))
                break;
            auto lv = Trim(std::string_view(line));
            auto tsview = ConsumeToken(lv);
            if (tsview.empty())
                break;
            if (std::sscanf(tsview.data(), "%lld", &Timestamp) != 1)
                break;
            Hash = ConsumeToken(lv);
            if (Hash.empty())
                break;
            std::filesystem::path path = ConsumeToken(lv, true);
            if (path.empty())
                break;
            return path.lexically_normal();
        }
        while (false);
        throw std::runtime_error("unrecognized cache entry: " + line);
    }

    void Save(std::ofstream &fs, std::filesystem::path path) const
    {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%lld", Timestamp);
        fs << buf << " " << std::string(Hash) << " " << path.lexically_normal() << "\n";
    }
};

class Cache
{
private:
    struct PathHasher
    {
    public:
        size_t operator()(std::filesystem::path const &p) const
        { return std::filesystem::hash_value(p); }
    };
    std::unordered_map<std::filesystem::path, CacheEntry, PathHasher> files;

public:
    static constexpr char const *FileName = ".hash_cache.txt";
    
    void Load(char const *root = ".")
    {
        Log("* loading cache");
        auto path = std::filesystem::path(root) / FileName;
        if (std::filesystem::exists(path))
        {
            std::ifstream ifs(path, std::ios::binary);
            while (ifs.peek(), ifs.good())
            {
                CacheEntry entry;
                auto path = entry.Load(ifs).relative_path();
                Log("*   " FPATH, path.c_str());
                files[path] = entry;
            }
        }
        Log("* %u files cached", uint32_t(files.size()));
    }
    
    void Update(char const *root = ".")
    {
        Log("* updating cache");
        uint32_t ignored{}, checked{}, restored{}, updated{}, new_{};
        namespace fs = std::filesystem;
        for (RecursiveDirectoryIterator rec(root); rec; ++rec)
        {
            auto path = rec.Path().relative_path().lexically_normal();
            if (path.filename().c_str()[0] == '.')
            {
                Log("*   ignoring: " FPATH, path.c_str());
                ignored++;
                if (rec.Directory())
                    rec.Skip();
                continue;
            }
            if (rec.Directory())
                continue;
            if (auto it = files.find(path); it != files.end())
            {
                Log("*   checking: " FPATH, path.c_str());
                checked++;
                auto &entry = it->second;
                // 1. compare timestamps (match => get out)
                auto ftime = fs::last_write_time(path);
                int64_t ts = ftime.time_since_epoch().count();
                if (ts == entry.Timestamp)
                    continue;
                // 2. timestamps don't match: calculate new hash                
                std::ifstream ifs(path, std::ios::binary);
                MD5 md5;
                md5.Update(ifs).Finalize();
                std::string hash = md5.Digest();
                // 3. compare new hash with the cached one
                if (hash == entry.Hash)
                {
                    // 4. match => restore timestamp
                    Log("*   restoring timestamp: " FPATH, path.c_str());
                    restored++;
                    auto newTime = fs::file_time_type(fs::file_time_type::clock::duration(entry.Timestamp));
                    fs::last_write_time(path, newTime);
                    continue;
                }
                else
                {
                    // 5. else => save new hash and timestamp
                    Log("*   updating: " FPATH, path.c_str());
                    updated++;
                    entry.Hash = hash;
                    entry.Timestamp = ts;
                    continue;
                }
            }
            else
            {
                Log("*   new file: " FPATH, path.c_str());
                new_++;
                auto &entry = files[path];
                std::ifstream ifs(path, std::ios::binary);
                MD5 md5;
                md5.Update(ifs).Finalize();
                std::string hash = md5.Digest();
                auto ftime = fs::last_write_time(path);
                int64_t ts = ftime.time_since_epoch().count();
                entry.Hash = hash;
                entry.Timestamp = ts;
                continue;
            }
        }
        Log("- update completed: ignored[%u], checked[%u], restored[%u], updated[%u], new[%u]",
            ignored, checked, restored, updated, new_);
    }
    
    void Save(char const *root = ".")
    {
        Log("* saving cache");
        std::ofstream ofs(std::filesystem::path(root) / FileName, std::ios::binary);
        for (auto const &[path, entry] : files)
            entry.Save(ofs, path);
    }
};

int main(int argc, char const **argv)
{
    switch (argc)
    {
    case 1:
        break;
    case 2:
        if (!std::strcmp(argv[1], "--verbose"))
        {
            Verbose = true;
            break;
        }
    default:
        Log("! unrecognized option (only --verbose is available)");
        return 1;
    }
    Cache cache;
    cache.Load();
    cache.Update();
    cache.Save();
    return 0;
}
