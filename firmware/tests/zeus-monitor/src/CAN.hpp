#pragma once


#include <memory>
#include <vector>
#include <biorob-cpp/os/FileDescriptor.h>

#include <sys/socket.h>
#include <linux/can.h>

class CANSocket {
public:
	typedef std::unique_ptr<CANSocket> Ptr;

	static Ptr Open(const std::string & intf,std::vector<struct can_filter> filters);


	void Receive(struct can_frame & );
	void Send(struct can_frame & );

	inline os::FileDescriptor::NativeHandle Get() const {
		return d_fd->Get();
	}


private :
	CANSocket(os::FileDescriptor::Ptr & fd);
	os::FileDescriptor::Ptr d_fd;
};
