#include <glog/logging.h>

#include "CAN.hpp"

#include <biorob-cpp/os/Poller.h>

#include <arke.h>
#include <csignal>
#include <iomanip>
#include <fstream>
struct Report {
	double Humidity;
	double Temperature[4];
};

void ReadReport(const can_frame & frame, Report & r) {
	ArkeZeusReport * report = (ArkeZeusReport*)frame.data;
	r.Humidity = (double)(report->Humidity) / 16382.0 * 100.0 ;
	r.Temperature[0] = (double)report->Temperature1 / 16382.0 * 165.0 - 40.0;
	r.Temperature[1] = (double)report->Temperature2 * 0.0625;
	r.Temperature[2] = (double)report->Temperature3 * 0.0625;
	r.Temperature[3] = (double)report->Temperature4 * 0.0625;

}

static bool shouldStop = false;

void sigint_handler(int) {
	shouldStop = true;
}

void Execute(int argc, char ** argv) {
	if (argc < 2) {
		throw std::runtime_error("I need the can interface name");
	}
	std::shared_ptr<std::ofstream> file;
	if (argc == 3 ) {
		file = std::make_shared<std::ofstream>(argv[2],std::ostream::out);
		*file << "# iteration Humidity (%) Temp1 (°C)  Temp2 (°C)  Temp3 (°C)  Temp4 (°C)" << std::endl;
		LOG(INFO) << "Outputing to " << argv[2];
	}
	auto c = CANSocket::Open(std::string(argv[1]),{});

	std::signal(SIGINT,&sigint_handler);
	unsigned int i  = 0;
	while(shouldStop == false) {
		struct can_frame f;
		try {
			c->Receive(f);
		} catch ( const std::system_error & e)  {
			if ( e.code() == std::errc::interrupted ) {
				return;
			}

		}
		Report r;
		ReadReport(f,r);
		if(file) {
			if (++i % 10 == 1 ) {
				*file << i
				      << " " << r.Humidity
				      << " " << r.Temperature[0]
				      << " " << r.Temperature[1]
				      << " " << r.Temperature[2]
				      << " " << r.Temperature[4]
				      << std::endl;
			}
		} else {
			LOG(INFO) << "Data: " << std::setw(2) << std::hex << std::setfill('0')
			          << (int)f.data[0]
			          << " " << std::setfill('0') << (int)f.data[1]
			          << " " << std::setfill('0') << (int)f.data[2]
			          << " " << std::setfill('0') << (int)f.data[3]
			          << " " << std::setfill('0') << (int)f.data[4]
			          << " " << std::setfill('0') << (int)f.data[5]
			          << " " << std::setfill('0') << (int)f.data[6]
			          << " " << std::setfill('0') << (int)f.data[7]
			          << std::endl
			          << "Humidity: " << r.Humidity << "% "
			          << "Temperature 1: " << r.Temperature[0] << "°C "
			          << "Temperature 2: " << r.Temperature[1] << "°C "
			          << "Temperature 3: " << r.Temperature[2] << "°C "
			          << "Temperature 4: " << r.Temperature[3] << "°C";
		}
	}

}

int main(int argc,char ** argv) {
	FLAGS_logtostderr = 1;
	google::InitGoogleLogging(argv[0]);
	try {
		Execute(argc,argv);
	} catch ( const std::exception & e) {
		LOG(ERROR) << "Unhandled exception: " << e.what();
		return 42;
	}

	return 0;
}
