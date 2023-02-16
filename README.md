/* ================================================================================= */

1) **Make service for run**
<pre>
#include "service.h"

class TestService: public daemonizer::Service {
  public:
    TestService() {}
    virtual TestService() {}
    
    bool run() override {
       // Do something here for your service
    }
};
</pre>

/* ================================================================================= */

2) **Make Daemoniser**

<pre>
#include "daemon.h"

class TestDaemon : public daemonizer::Daemonizer {
 public:
  	TestDaemon(const int argc, const char **argv):Daemonizer(argc, argv) {}
  	~TestDaemon() {}
  private:
  	bool initialize() noexcept override {
  		// init all params for your daemon from config here
  	}
};
</pre>

/* ================================================================================= */

3) **Start your daemon in main.cc**

<pre>
#include "test_daemon.h"

int main(const int argc, const char **argv ) { 
	return TestDaemon(argc, argv).run();
}
</pre>

/* ================================================================================= */
