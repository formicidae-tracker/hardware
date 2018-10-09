#pragma once

typedef struct HostPacket {
	uint32_t ID;
	uint8_t length;
	uint8_t data[8];
} HostPacket;


const uint32_t STD_ID_MSK = (1L << 11 ) - 1;
const uint32_t EXT_ID_MSK =  (1L << 29 ) - 1;
const uint32_t IDEBIT_MSK = 1L << 29;
const uint32_t RTRBIT_MSK = 1L << 30;
const uint32_t CONTROLBIT_MSK = 1L << 31;


#define START_OF_FRAME_SIZE 2
#define START_OF_FRAME_BYTE 0xaa
#define HEADER_SIZE (START_OF_FRAME_SIZE + 5)
#define HOST_PACKET_SIZE(packet) (START_OF_FRAME_SIZE + 6 +  (packet).length)
#define HOST_PACKET_HEADER_SIZE HOST_PACKET_SIZE(0)


typedef enum HostPacketType_e {
	HP_RESET_REQUEST = 0x0,
	HP_FW_VERSION_REQUEST,
	HP_FW_VERSION,
	HP_STATUS_REPORT_REQUEST,
	HP_STATUS_REPORT,
} HostPacketType_e;

#define arke_compute_checksum(p,result) do {	  \
		const uint8_t * data = (const uint8_t*)(&(p)); \
		(result) = *data; \
		for(uint8_t i = 1; i < (HEADER_SIZE - START_OF_FRAME_SIZE + (p).length); ++i) { \
			(result) ^= *(data+i); \
		} \
	}while(0)


#define arke_hp_make_reset(p) do{	  \
		(p).ID = HP_RESET_REQUEST | CONTROLBIT_MSK; \
		(p).length = 0x00; \
	}while(0)

#define arke_hp_make_fw_version_request(p) do{	  \
		(p).ID = HP_oFW_VERSION_REQUEST | CONTROLBIT_MSK; \
		(p).length = 0x00; \
	}while(0)


#define arke_hp_make_fw_version(p,major,minor) do {	  \
		(p).ID = HP_FW_VERSION | CONTROLBIT_MSK;	  \
		(p).length = 6; \
		(p).data[0] = 'A'; \
		(p).data[1] = 'R'; \
		(p).data[2] = 'K'; \
		(p).data[3] = 'E'; \
		(p).data[4] = major; \
		(p).data[5] = minor; \
	}while(0)



#define arke_hp_make_status_report_request(p) do {	  \
		(p).ID = HP_STATUS_REPORT_REQUEST | CONTROLBIT_MSK; \
		(p).length = 0x0; \
	}while(0)


#define arke_hp_make_status_report(p,rxOverflow,txOverflow,invalidCS,unknownID,missedSOF,CANRxError,CANTxError) do { \
		(p).ID = HP_STATUS_REPORT_REQUEST | CONTROLBIT_MSK; \
		(p).length = 8; \
		(p).data[2] = rxOverflow; \
		(p).data[3] = txOverflow; \
		(p).data[4] = invalidCS; \
		(p).data[5] = unknownID; \
		(p).data[6] = (missedSOF) & 0xff; \
		(p).data[7] = ((missedSOF) & 0xff00) >> 8; \
		(p).data[0] = CANRxError; \
		(p).data[1] = CANTxError; \
	}while(0)
