#include "CAN.hpp"

#include <biorob-cpp/os/PosixCall.h>

#include <cstring>

#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/can/raw.h>
#include <unistd.h>


CANSocket::Ptr CANSocket::Open(const std::string & intf,std::vector<struct can_filter> filters) {
	int s = socket(PF_CAN,SOCK_RAW,CAN_RAW);
	if ( s < 0 ) {
		throw BCPP_SYSTEM_ERROR(errno,socket);
	}
	struct ifreq ifr;
	strcpy(ifr.ifr_name,intf.c_str());

	p_call(ioctl,s,SIOCGIFINDEX,&ifr);


	struct sockaddr_can addr;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	p_call(bind,s,(struct sockaddr *)&addr,sizeof(addr));

	// if( !filters.empty() ) {
	// 	p_call(setsockopt,
	// 	       s,
	// 	       SOL_CAN_RAW,
	// 	       CAN_RAW_FILTER,
	// 	       &(filters[0]),
	// 	       sizeof(struct can_filter) * filters.size());
	// }

	os::FileDescriptor::Ptr fd(new os::FileDescriptor(s));

	return Ptr(new CANSocket(fd));
}

void CANSocket::Receive(struct can_frame & frame) {
	size_t readB = read(d_fd->Get(), &frame, sizeof(struct can_frame) );
	if (readB < 0) {
		throw BCPP_SYSTEM_ERROR(errno,read);
	}

	if ( readB < sizeof(struct can_frame) ) {
		throw std::runtime_error("Incomplet CAN Frame received");
	}

}

void CANSocket::Send(struct can_frame & frame) {
	size_t written = write(d_fd->Get(), &frame, sizeof(struct can_frame) );
	if (written < 0) {
		throw BCPP_SYSTEM_ERROR(errno,write);
	}

	if ( written < sizeof(struct can_frame) ) {
		throw std::runtime_error("Incomplete CAN Frame sent");
	}

}

CANSocket::CANSocket(os::FileDescriptor::Ptr & fd)
	: d_fd(std::move(fd)) {
}
