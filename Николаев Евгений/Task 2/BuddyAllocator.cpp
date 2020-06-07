#include <iostream>
#include <list>
#include <cmath>
#include <vector>
#include <map>

using namespace std;


class Block
{
public:
    Block(void* start, size_t size)
    {
        this->size = size;
        this->start = start;
    }
    void* start;
    size_t size;
};


class Allocator
{
public:
    explicit Allocator(size_t size)
    {
        if ((size & (size - 1)) != 0)
            throw invalid_argument("size is not exp of 2");
        pool = malloc(size);
        min_size = (int)pow(2, MIN_EXP);
        int i = min_size;
        max_exp = PowOf2(size);
        while (i != size)
        {
            list<Block> test = {};
            empty_blocks.emplace_back();
            filled_blocks.emplace_back();
            i *= 2;
        }
        filled_blocks.emplace_back();
        empty_blocks.emplace_back();
        empty_blocks.back().emplace_back(Block(pool, size));
    }

    void* Alloc(size_t size)
    {
        int exp = PowOf2(size) - MIN_EXP;
        int needed_exp = exp;
        while (empty_blocks[exp].empty()) {
            if (exp > max_exp)
                throw invalid_argument("no empty blocks");
            exp += 1;
        }
        bool finded_needed_block = false;
        if (exp == needed_exp)
            finded_needed_block = true;
        int needed_size = (int)pow(2, needed_exp + MIN_EXP);
        while (!finded_needed_block)
        {
            Block block = empty_blocks[exp].back();
            empty_blocks[exp].pop_back();
            exp -= 1;
            size_t new_size = block.size / 2;
            void* new_start = (void*)((size_t)block.start + new_size);
            empty_blocks[exp].push_back(Block(new_start, new_size));
            empty_blocks[exp].push_back(Block(block.start, new_size));
            if (needed_size == new_size)
                break;
        }
        Block block_to_fill = empty_blocks[exp].back();
        empty_blocks[exp].pop_back();
        filled_blocks[exp].push_back(block_to_fill);
        blocks[block_to_fill.start] = { exp, filled_blocks[exp].size() - 1 };
        return block_to_fill.start;
    }

    void Free(void* ptr)
    {
        int exp = get<0>(blocks[ptr]);
        int id = get<1>(blocks[ptr]);
        Block empty_block = filled_blocks[exp][id];
        filled_blocks[exp].erase(filled_blocks[exp].begin() + id);
        empty_blocks[exp].push_back(empty_block);
        MergeBlocks(exp);
    }

    void Dump()
    {
        cout << "EMPTY BLOCKS" << endl;
        cout << "[size]: [count]: [starts of block]" << endl;
        for (int i = 0; i < empty_blocks.size(); i++)
        {
            cout << pow(2, i + MIN_EXP) << ": " << empty_blocks[i].size() << ": ";
            for (auto& j : empty_blocks[i])
                cout << j.start << " ";
            cout << endl;
        }
        cout << "FILLED BLOCKS" << endl;
        cout << "[size]: [count]: [starts of block]" << endl;
        for (int i = 0; i < filled_blocks.size(); i++)
        {
            cout << pow(2, i + MIN_EXP) << ": " << filled_blocks[i].size() << ": ";
            for (auto& j : filled_blocks[i])
                cout << j.start << " ";
            cout << endl;
        }
        cout << endl;
    }

private:
    void* pool;
    int min_size;
    const int MIN_EXP = 3;
    int max_exp;
    vector<vector<Block>> empty_blocks;
    vector<vector<Block>> filled_blocks;
    map<void*, tuple<int, int>> blocks;

    void MergeBlocks(int exp)
    {
        Block block = empty_blocks[exp].back();
        void* start = block.start;
        void* end = (void*)((size_t)start + block.size);
        for (int i = 0; i < empty_blocks[exp].size() - 1; i++) {
            void* start_current = empty_blocks[exp][i].start;
            void* end_current = (void*)((size_t)start_current + empty_blocks[exp][i].size);
            if (start_current == end || end_current == start)
            {
                Block new_block = Block(start_current, block.size * 2);
                if (start_current == end)
                    new_block.start = start;
                empty_blocks[exp].erase(empty_blocks[exp].end());
                empty_blocks[exp].erase(empty_blocks[exp].begin() + i + 1);
                empty_blocks[exp + 1].push_back(new_block);
                MergeBlocks(exp + 1);
                break;
            }
        }
    }

    int PowOf2(int val) {
        if (val < min_size)
            return 3;
        int i = 1;
        int j = 0;
        while (i < val)
        {
            i <<= 1;
            j += 1;
        }
        return j;
    }
};

int main() {
    Allocator alloc = Allocator(1024);
    void* g1 = alloc.Alloc(247);
    void* g2 = alloc.Alloc(31);
    void* g3 = alloc.Alloc(8);
    void* g4 = alloc.Alloc(8);
    void* g5 = alloc.Alloc(8);
    void* g6 = alloc.Alloc(8);
    void* g7 = alloc.Alloc(8);
    alloc.Free(g1);
    alloc.Free(g3);
    alloc.Free(g2);
    alloc.Dump();
    return 0;
}
