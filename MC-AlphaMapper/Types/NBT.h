enum tagType {
	TAG_End = 0x00,
	TAG_Byte = 0x01,
	TAG_Short = 0x02,
	TAG_Int = 0x03,
	TAG_Long = 0x04,
	TAG_Float = 0x05,
	TAG_Double = 0x06,
	TAG_Byte_Array = 0x07,
	TAG_String = 0x08,
	TAG_List = 0x09,
	TAG_Compound = 0x0A
};

/**
 * @brief A class representing each NBT tag
 */
class tag {
public:
	// the type of the tag
	tagType Type;
	// the name of the tag
	std::string Name;
	// the length of the tag's name
	BYTE NameLength;
};

class ByteTag : public tag {
	// the tag is byte type
	tagType Type = TAG_Byte;
	// the byte value contained within the tag
	BYTE value = 0x00;
};

class LongTag : public tag {
	// the tag is long type
	tagType Type = TAG_Long;


	// we are using long long here because normal long is 32-bit but Java longs are 64-bit

	// the long value contained within the tag 
	long long value = 0x00i64;
};