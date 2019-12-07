#include <syslog.h>

#define LOGGER(prio, format, msg) \
	syslog(prio, "line #%d %s(tid=0x%lx): "format, __LINE__, __func__, pthread_self() , msg);  

int main() {
	LOGGER(LOG_CRIT, "Hello %s", "kitty");
}
