#include "gameclock.h"

using chr = std::chrono::steady_clock;

GameClock::GameClock(duration d) : m_duration(d), m_lastWallclock(chr::now()) {}

bool GameClock::running() { return m_elapsed < m_duration; }

void GameClock::update() {
	tp now = chr::now();
	if (now > m_lastWallclock) {
		addTime(now - m_lastWallclock);
		m_lastWallclock = now;
	}
}

void GameClock::addTime(duration d) { m_elapsed += d; }

uint32_t GameClock::periodicCount() {
	uint32_t ret = 0;
	if (m_elapsed - m_periodic > 100ms) {
		auto cnt = (m_elapsed - m_periodic).count() / (100ms).count();
		ret = static_cast<uint32_t>(cnt);
		m_periodic += ret * 100ms;
	}
	return ret;
}
