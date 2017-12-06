#ifndef SNITCH_H
#define SNITCH_H

#include <stdint.h>

class Scenario; // Forward declaration

class Snitch {
  public:
	/** Constructor **/
	Snitch(const char *const host, uint16_t port, Scenario &scenario);

	/** Gets event file descriptor
	 * @return File descriptor
	 */
	int eventFd() const;

	/** Process read events **/
	void eventProcessRead();

  private:
	int m_fd{-1};

	/** Scenario in use **/
	Scenario *m_scenario;
};

#endif // SNITCH_H
