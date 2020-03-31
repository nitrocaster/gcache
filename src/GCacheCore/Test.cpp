#include "Common/Config.hpp"
#include "GCacheCore.hpp"
#include "MD5.hpp"
#include "RecursiveDirectoryIterator.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace GCache
{

TEST_CASE("MD5::DigestType to string")
{
    MD5::DigestType digest
    {{
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
    }};
    CHECK(std::string(digest) == "0001020304050607f8f9fafbfcfdfeff");
}

TEST_CASE("MD5 Init & Update & Finalize")
{
    auto md5 = [](char const *s)
    { return std::string(MD5().Update(s, std::strlen(s)).Finalize().Digest()); };
    SUBCASE("empty string")
    {
        CHECK(md5("") == "d41d8cd98f00b204e9800998ecf8427e");
    }
    SUBCASE("lazy dog")
    {
        CHECK(md5("The quick brown fox jumps over the lazy dog") == "9e107d9d372bb6826bd81d3542a419d6");
    }
}

namespace fs = std::filesystem;

TEST_CASE("RecursiveDirectoryIterator")
{
    struct PathHasher
    {
    public:
        size_t operator()(fs::path const &p) const
        { return fs::hash_value(p); }
    };
    struct PathFlags
    {
        bool Visited = false;
        bool MustVisit;
        bool Invalid = true;

        PathFlags(bool mustVisit = false) :
            MustVisit(mustVisit), Invalid(false)
        {}
    };
    auto touch = [](fs::path const &path)
    { std::ofstream ofs(path); };
    auto normalize = [](fs::path const &path)
    { return path.relative_path().lexically_normal(); };
    fs::path root = "test_hierarchy";
    fs::remove_all(root);
    std::unordered_map<fs::path, PathFlags, PathHasher> paths;
    std::vector<std::pair<fs::path, PathFlags>> directories
    {
        {root, false},
        {root / "foo", true},
        {root / "foo/zed", true},
        {root / "bar", true},
        {root / ".hidden", true}
    };
    std::vector<std::pair<fs::path, PathFlags>> files
    {
        {root / "foo/f1.txt", true},
        {root / "foo/zed/z1.txt", true},
        {root / "foo/zed/z2.txt", true},
        {root / "bar/b1.txt", true},
        {root / ".hidden/hidden.txt", false},
        {root / "r1.txt", true},
    };
    for (auto const &[dir, flags] : directories)
    {
        fs::create_directory(dir);
        paths[normalize(dir)] = flags;
    }
    for (auto const &[file, flags] : files)
    {
        touch(file);
        paths[normalize(file)] = flags;
    }
    for (RecursiveDirectoryIterator rec(root); rec; ++rec)
    {
        auto path = normalize(rec.Path());
        if (path.filename() == ".hidden")
            rec.Skip();
        auto &flags = paths[path];
        CHECK_MESSAGE(!flags.Invalid, path);
        CHECK_MESSAGE(!flags.Visited, path);
        flags.Visited = true;
    }
    for (auto const &[path, flags] : paths)
    {
        auto npath = path;
        CHECK_MESSAGE(flags.MustVisit == flags.Visited, npath);
    }
    fs::remove_all(root);
}
} // namespace GCache
