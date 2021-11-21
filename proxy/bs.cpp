/* 
    constexpr static uint32_t getMaskForIndex(size_t bitIndex) {
		assert(bitIndex < containerBitsCount && "Cannot mask outside the bits of the container type!");
		return uint32_t(1) << bitIndex;
	}

    struct IndexingPair {
        size_t containerIndex;
        size_t bitIndex;

        uint32_t getMask() const { return getMaskForIndex(bitIndex); }
		uint32_t getMaskInverted() const { return ~getMask(); }
    };
    
    constexpr static IndexingPair getIndexingPairForBitIndex(int bitIndex) {
		return IndexingPair{ bitIndex / containerBitsCount , bitIndex % containerBitsCount };
	} */