#ifndef GAMECLOCK_H
#define GAMECLOCK_H

#include <chrono>
using namespace std::chrono_literals;

class GameClock {
  public:
	typedef std::chrono::steady_clock::time_point tp;
	typedef std::chrono::steady_clock::duration duration;

	/** Constructor of game clock **/
	GameClock();

	/** Set duration of simulation
	 * @param d Duration of simulation
	 */
	void setDuration(duration d);

	/** Is simulation running **/
	bool running();

	/** Update clock **/
	void update();

	/** Add time to simulation **/
	void addTime(duration d);

	/** Periodic calls to do **/
	uint32_t periodicCount();

	/** Execute one agent per team **/
	void setOneShot();

	/** Is oneshot ? **/
	bool oneShot();

	/** Go to normal run **/
	void setNormalRun();

	/** Is normal run ? **/
	bool normalRun();

  private:
	/** Add time to simulation (debugger version) **/
	void addTime(int d) { addTime(d * 1ms); }

	/** Duration of simulation **/
	duration m_duration = 0s;

	/** Elapsed time of simulation **/
	duration m_elapsed = 0s;

	/** Last periodic elapsed time **/
	duration m_periodic = 0s;

	/** last wall clock value **/
	tp m_lastWallclock;

	/** Is oneshot ? **/
	bool m_oneShot = false;

	/** Is normal run ? **/
	bool m_normalRun = true;
};

#endif // GAMECLOCK_H
