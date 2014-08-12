enum FILE_PACKET_STATE {
	START_PACKET,
	START_ACK,
	DATA_PACKET,
	DATA_ACK,
	EOF_PACKET,
	EOF_ACK,
	FILE_PACKET_UNKNOWN
	};

struct __attribute__((packed)) file_property_t
{
	unsigned int size;
	unsigned short data_packet_size;
	unsigned char file_name[64];
	
}file_property;
