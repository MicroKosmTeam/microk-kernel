#include <sys/bitmap.hpp>

bool Bitmap::operator[](u64 index) {
        return Get(index);
}

bool Bitmap::Get(u64 index) {
        if (index > size * 8) return false;
        u64 byteIndex = index / 8;
        u8 bitIndex = index % 8;
        u8 bitIndexer = 0b10000000 >> bitIndex;

        if ((buffer[byteIndex] & bitIndexer) > 0) {
                return true;
        }

        return false;
}

bool Bitmap::Set(u64 index, bool value) {
        if (index > size * 8) return false;
        u64 byteIndex = index / 8;
        u8 bitIndex = index % 8;
        u8 bitIndexer = 0b10000000 >> bitIndex;

        buffer[byteIndex] &= ~bitIndexer;
        if (value) {
                buffer[byteIndex] |= bitIndexer;
        }

        return true;
}
