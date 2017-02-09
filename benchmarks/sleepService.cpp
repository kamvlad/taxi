#include <thread>
#include <sstream>
#include <chrono>
#include <fastcgi2/logger.h>
#include <fastcgi2/config.h>
#include <fastcgi2/stream.h>
#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include <iostream>

class SleepService : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
	SleepService(fastcgi::ComponentContext *context) :
					fastcgi::Component(context) {
	}
	virtual ~SleepService() {
	}

public:
	virtual void onLoad() {
		v_ = 0;
		const std::string loggerComponentName = context()->getConfig()->asString(context()->getComponentXPath() + "/logger");
		logger_ = context()->findComponent<fastcgi::Logger>(loggerComponentName);
		std::cout << "onLoad" << std::endl;
		if (!logger_) {
			throw std::runtime_error("cannot get component " + loggerComponentName);
		}
	}

	virtual void onUnload() {
	}

	virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
		fastcgi::RequestStream ss(request);

		auto start = std::chrono::system_clock::now();
		ss << start.time_since_epoch().count();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		auto end = std::chrono::system_clock::now();
		ss << " " << end.time_since_epoch().count() << std::endl;
		std::cout << (v_++) << std::endl;
		request->setStatus(200);
	}
private:
	int v_;
	fastcgi::Logger *logger_;
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("sleepService_factory", SleepService)
FCGIDAEMON_REGISTER_FACTORIES_END()