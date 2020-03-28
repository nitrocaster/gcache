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

static std::string_view Trim(std::string_view s)
{
    char const *trimChars = " \"\t\r\v\n";
    s.remove_prefix(std::min(s.find_first_not_of(trimChars), s.size()));
    s.remove_suffix(std::min(s.size() - s.find_last_not_of(trimChars) - 1, s.size()));
    return s;
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
        auto path = std::filesystem::path(root) / FileName;
        if (std::filesystem::exists(path))
        {
            std::ifstream ifs(path, std::ios::binary);
            while (ifs.peek(), ifs.good())
            {
                CacheEntry entry;
                auto path = entry.Load(ifs).relative_path();
                files[path] = entry;
            }
        }
    }
    
    void Update(char const *root = ".")
    {
        for (RecursiveDirectoryIterator rec(root); rec; ++rec)
        {
            auto path = rec.Path().relative_path().lexically_normal();
            if (path.filename().c_str()[0] == '.')
            {
                if (rec.Directory())
                    rec.Skip();
                continue;
            }
            if (rec.Directory())
                continue;
            // XXX: process file referenced by 'path'
        }
    }
    
    void Save(char const *root = ".")
    {
        std::ofstream ofs(std::filesystem::path(root) / FileName, std::ios::binary);
        for (auto const &[path, entry] : files)
            entry.Save(ofs, path);
    }
};

int main()
{
    Cache cache;
    cache.Load();
    cache.Update();
    cache.Save();
    return 0;
}
