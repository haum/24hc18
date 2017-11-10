#ifndef AGENT_H
#define AGENT_H

#include "gameobject.h"
#include "team.h"
#include <cstring>
#include <ostream>

class Agent : public GameObject {
  public:
	/** Virutal destructor (empty) **/
	virtual ~Agent();

	/** Execute prelude instructions, that is instructions before AI is executed
	 * @param os Output stream to send data to agent
	 * @return true if AI should be executed, false if not
	 */
	virtual bool prelude(std::ostream &os) = 0;

	/** Method executed at each command from AI
	 * @param argc Argument count, including command
	 * @param argv Argument list, command at 0
	 */
	virtual void execute(uint8_t argc, const char **argv) = 0;

	/** Execute epilogue instructions, that is instructions to be done after AI
	 * is executed */
	virtual void epilogue() {}

	/** Get team reference **/
	Team &team() { return m_team; }

	/** Destroy a game object
	 * @note The version without argument destroys the agent itself
	 * @param obj Object to destroy
	 */
	void destroy(GameObject *obj);
	void destroy();

  protected:
	/** Constructor
	 * @param team Team whose agent belongs
	 */
	Agent(Team &team, GameObject_t type, double latitude, double longitude,
		  double heading);

	/** Agent's team **/
	Team &m_team;

	/** Parse integer
	 * @param str String to parse
	 * @param ok Convertion was successfull
	 * @return Converted number if successfull, undefined otherwise
	 */
	int32_t param_int(const char *str, bool &ok);

	/** Parse integer
	 * @param str String to parse
	 * @param ok Convertion was successfull
	 * @param min Minimum allowed value
	 * @param max Maximum allowed value
	 * @return Converted number if successfull, undefined otherwise
	 */
	int32_t param_int(const char *str, bool &ok, int min, int max);
};

#endif // AGENT_H
