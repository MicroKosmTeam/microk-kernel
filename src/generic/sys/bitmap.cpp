#include <sys/bitmap.hpp>

bool Bitmap::operator[](u64 index) {
        return Get(index);
}

bool Bitmap::Get(u64 index) {
        if (index > Size * 8) return false;
        u64 byteIndex = index / 8;
        u8 bitIndex = index % 8;
        u8 bitIndexer = 0b10000000 >> bitIndex;

        if ((Buffer[byteIndex] & bitIndexer) > 0) {
                return true;
        }

        return false;
}

bool Bitmap::Set(u64 index, bool value) {
        if (index > Size * 8) return false;
        u64 byteIndex = index / 8;
        u8 bitIndex = index % 8;
        u8 bitIndexer = 0b10000000 >> bitIndex;

        Buffer[byteIndex] &= ~bitIndexer;
        if (value) {
                Buffer[byteIndex] |= bitIndexer;
        }

        return true;
}
