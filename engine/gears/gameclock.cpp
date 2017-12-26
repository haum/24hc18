#include "gameclock.h"

using chr = std::chrono::steady_clock;

GameClock::GameClock() : m_lastWallclock(chr::now()) {}

void GameClock::setDuration(duration d) { m_duration = d; }

bool GameClock::running() { return m_elapsed < m_duration; }

void GameClock::update() {
	tp now = chr::now();
	if (m_normalRun) {
		addTime(now - m_lastWallclock);
		m_lastWallclock = now;
	}
}

void GameClock::addTime(duration d) { m_elapsed += d; }

uint32_t GameClock::periodicCount() {
	uint32_t ret = 0;
	if (m_elapsed - m_periodic >= 100ms) {
		auto cnt = (m_elapsed - m_periodic) / 100ms;
		ret = static_cast<uint32_t>(cnt);
		m_periodic += ret * 100ms;
	}
	return ret;
}

void GameClock::setOneShot() {
	m_oneShot = true;
	m_normalRun = false;
}

bool GameClock::oneShot() {
	bool ret = m_oneShot;
	m_oneShot = false;
	return ret;
}

void GameClock::setNormalRun() {
	m_oneShot = false;
	m_normalRun = true;
	m_lastWallclock = chr::now();
}

bool GameClock::normalRun() { return m_normalRun; }
