#ifndef SCENARIOBASE_H
#define SCENARIOBASE_H

#include "../nest.h"
#include "lineparser.h"
#include "snitch.h"
#include <chrono>
#include <memory>
#include <vector>

/** Class managing scenario execution (gears part) **/
class ScenarioBase {
  public:
	/** Constructor **/
	ScenarioBase();

	/** Set list of teams
	 * @param teams List of teams
	 */
	void set_teams(std::vector<Team *> teams);

	/** Set snitch object
	 * @param snitch Snitch object
	 */
	void set_snitch(Snitch *snitch);

	/** Load scenario
	 * @param scenario_name  Scenario name
	 */
	void load(const char *scenario_name);

	/** Run scenario **/
	void run();

	/** Process one line of scenario **/
	virtual void processLine(uint8_t argc, const char **argv) = 0;

	/** Create and add game object **/
	template <typename T, typename... Args> T *addGameObject(Args &&... args) {
		static_assert(std::is_base_of<GameObject, T>::value,
					  "addGameObject can only construct GameObject");
		auto u = std::make_unique<T>(std::forward<Args>(args)...);
		auto *ret = u.get();
		addExistingGameObject(std::move(u));
		return ret;
	}

	/** Add game object **/
	void addExistingGameObject(std::unique_ptr<GameObject> obj);

	/** Remove game object **/
	void rmGameObject(GameObject *obj);

	/** List game objects
	 * @param fct Functor bool(std::shared_ptr<GameObject>) called at each
	 *            object until the end of collection or until it returns false
	 * @note Objects cannot be directly nor indirectly destroyed during the loop
	 *
	 * Example usage:
	 * scenario.listObjects([](auto sgo) {
	 *     std::cout << sgo->category()->name() << std::endl;
	 *     return true;
	 * });
	 */
	template <typename Fct> void listObjects(Fct fct) {
		for (auto sgo : m_gameObjectsStorage) {
			if (!fct(sgo))
				break;
		}
	}

  protected:
	/** File parser **/
	LineParser<100> m_parser;

	/** Snitch to export data **/
	Snitch *m_snitch{nullptr};

	/** Copy of team list **/
	std::vector<Team *> m_teams;

	/** List of created nests **/
	std::vector<std::shared_ptr<GameObject>> m_gameObjectsStorage;

	/** Duration **/
	std::chrono::seconds m_duration;
};

#endif // SCENARIOBASE_H
