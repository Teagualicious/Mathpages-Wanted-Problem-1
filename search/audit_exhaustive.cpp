#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Record {
    std::uint64_t product;
    std::uint32_t value;
};

static std::uint32_t reverse_bits(std::uint32_t value, int length) {
    std::uint32_t reversed = 0;
    for (int i = 0; i < length; ++i) {
        reversed = static_cast<std::uint32_t>((reversed << 1U) | (value & 1U));
        value >>= 1U;
    }
    return reversed;
}

static std::vector<int> distance_signature(std::uint32_t value, int length) {
    std::vector<int> signature;
    signature.reserve(static_cast<std::size_t>(length - 1));
    for (int distance = 1; distance < length; ++distance) {
        signature.push_back(std::popcount(value & (value >> distance)));
    }
    return signature;
}

static std::string bits(std::uint32_t value, int length) {
    std::string out;
    out.reserve(static_cast<std::size_t>(length));
    for (int i = length - 1; i >= 0; --i) {
        out.push_back(((value >> i) & 1U) != 0U ? '1' : '0');
    }
    return out;
}

static void print_record(std::uint32_t value, int length) {
    const auto signature = distance_signature(value, length);
    std::cout << "n=" << value
              << " pop=" << std::popcount(value)
              << " bits=" << bits(value, length)
              << " rev=" << reverse_bits(value, length)
              << " counts=";
    for (std::size_t i = 0; i < signature.size(); ++i) {
        if (i != 0) std::cout << ',';
        std::cout << signature[i];
    }
    std::cout << '\n';
}

int main(int argc, char** argv) {
    const int length = argc > 1 ? std::atoi(argv[1]) : 29;
    if (length < 2 || length > 31) {
        std::cerr << "length must be in [2, 31]\n";
        return 2;
    }

    const std::uint64_t record_count = std::uint64_t{1} << (length - 2);
    const std::uint32_t first = (std::uint32_t{1} << (length - 1)) | 1U;
    const std::uint32_t end = std::uint32_t{1} << length;

    std::vector<Record> records;
    records.reserve(static_cast<std::size_t>(record_count));
    for (std::uint32_t n = first; n < end; n += 2U) {
        const std::uint32_t reversed = reverse_bits(n, length);
        records.push_back({static_cast<std::uint64_t>(n) * reversed, n});
    }

    std::sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
        if (a.product != b.product) return a.product < b.product;
        return a.value < b.value;
    });

    std::uint64_t collision_groups = 0;
    for (std::size_t begin = 0; begin < records.size();) {
        std::size_t end_group = begin + 1;
        while (end_group < records.size() && records[end_group].product == records[begin].product) {
            ++end_group;
        }
        if (end_group - begin >= 2) {
            ++collision_groups;
            for (std::size_t i = begin; i < end_group; ++i) {
                for (std::size_t j = i + 1; j < end_group; ++j) {
                    const std::uint32_t a = records[i].value;
                    const std::uint32_t b = records[j].value;
                    if (b == reverse_bits(a, length)) continue;
                    if (distance_signature(a, length) != distance_signature(b, length)) {
                        std::cout << "COUNTEREXAMPLE L=" << length
                                  << " product=" << records[begin].product << '\n';
                        for (std::size_t k = begin; k < end_group; ++k) {
                            print_record(records[k].value, length);
                        }
                        return 0;
                    }
                }
            }
        }
        begin = end_group;
    }

    std::cout << "NONE L=" << length
              << " records=" << record_count
              << " collision_groups=" << collision_groups << '\n';
    return 0;
}
