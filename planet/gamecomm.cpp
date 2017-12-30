#include "gamecomm.h"
#include <QColor>
#include <QTcpSocket>
#include <QTimer>
#include <Qt3DExtras/QPhongMaterial>
#include <math.h>
#include <string.h>

namespace {
struct TeamMaterial : public Qt3DExtras::QPhongMaterial {
	TeamMaterial(const QColor c) {
		setAmbient(c);
		setShareable(true);
	}
};
TeamMaterial *teamMaterials[] = {
    new TeamMaterial{Qt::yellow}, new TeamMaterial{Qt::cyan},
    new TeamMaterial{Qt::green}, new TeamMaterial{Qt::red},
    new TeamMaterial{Qt::blue}};
constexpr int teamMaterialsSz = sizeof(teamMaterials) / sizeof(*teamMaterials);
} // namespace

GameComm::GameComm(QObject *parent) : QObject(parent) {
	onDisconnected();
	QObject::connect(&m_server, &QTcpServer::newConnection, this, [this] {
		m_client = m_server.nextPendingConnection();
		for (auto &t : m_teamsData) {
			for (auto &o : t) {
				for (auto &e : o.entities) {
					e->setVisible(false);
				}
				o.it = o.entities.begin();
			}
		}
		m_teams.clear();
		QObject::connect(m_client, &QTcpSocket::readyRead, this,
		                 &GameComm::onReadyRead);
		QObject::connect(m_client, &QTcpSocket::disconnected, this,
		                 &GameComm::onDisconnected);
		m_server.close();
		m_client->write("\n", 1);
		emit connectedChanged();
	});
}

void GameComm::setRefreshRate(qreal percent) {
	m_refreshRate = static_cast<int>(100 + (2000 - 100) * (1 - percent));
	emit refreshRateChanged();
}

void GameComm::onReadyRead() {
	for (;;) {
		QByteArray data;
		if (m_cut_buffer.size() != 0) {
			data = m_cut_buffer;
			data += m_client->read(SZ - m_cut_buffer.size());
			m_cut_buffer.clear();
		} else {
			data = m_client->read(SZ);
		}
		if (data.size() == SZ) {
			uint32_t category, team;
			float longitude, latitude, heading;
			::memcpy(&category, &data.data()[0], sizeof(uint32_t));
			::memcpy(&longitude, &data.data()[4], sizeof(float));
			::memcpy(&latitude, &data.data()[8], sizeof(float));
			::memcpy(&heading, &data.data()[12], sizeof(float));
			::memcpy(&team, &data.data()[16], sizeof(uint32_t));
			longitude *= 180 / M_PI;
			latitude *= 180 / M_PI;
			heading *= 180 / M_PI;

			if (category == UINT32_MAX) {
				for (auto &t : m_teamsData) {
					for (auto &o : t) {
						while (o.it != o.entities.end()) {
							(*o.it)->setVisible(false);
							++o.it;
						}
						o.it = o.entities.begin();
					}
				}
				QTimer::singleShot(m_refreshRate, this, &GameComm::onTimeout);
				continue;
			}

			auto itfind =
			    std::find(std::begin(m_teams), std::end(m_teams), team);
			if (itfind == m_teams.end()) {
				m_teams.push_back(team);
				team = static_cast<uint32_t>(m_teams.size() - 1);
			} else {
				team = static_cast<uint32_t>(
				    std::distance(m_teams.begin(), itfind));
			}

			if (category < 4) {
				if (m_teamsData.size() <= team) {
					m_teamsData.push_back({});
					m_teamsData[team][category].it =
					    m_teamsData[team][category].entities.begin();
				}
				auto &o = m_teamsData[team][category];
				if (o.it == o.entities.end()) {
					if (category == 0)
						o.entities.push_back(
						    new Ant(m_rootEntity, latitude, longitude, heading,
						            teamMaterials[team % teamMaterialsSz]));
					else if (category == 1)
						o.entities.push_back(
						    new Food(m_rootEntity, latitude, longitude));
					else if (category == 2)
						o.entities.push_back(
						    new Nest(m_rootEntity, latitude, longitude,
						             teamMaterials[team % teamMaterialsSz]));
					else if (category == 3)
						o.entities.push_back(new Pheromone(
						    m_rootEntity, latitude, longitude,
						    teamMaterials[team % teamMaterialsSz]));
					o.it = o.entities.end();
				} else {
					(*o.it)->setVisible(true);
					(*o.it)->setPosition(latitude, longitude, heading);
					++o.it;
				}
			}
		} else {
			m_cut_buffer = data;
			break;
		}
	}
}

void GameComm::onDisconnected() {
	m_server.listen(QHostAddress::Any, 2080);
	m_client->deleteLater();
	m_client = nullptr;
	emit connectedChanged();
}

void GameComm::onTimeout() {
	if (m_client)
		m_client->write("\n", 1);
}

void GameComm::setRootEntity(Qt3DCore::QEntity *rootEntity) {
	m_rootEntity = rootEntity;
}
