#ifndef AGENT_H
#define AGENT_H

#include "team.h"
#include <cstring>
#include <ostream>

class Agent {
  public:
	/** Constructor
	 * @param team Team whose agent belongs
	 */
	Agent(Team &team);

	/** Virutal destructor (empty) **/
	virtual ~Agent();

	/** Run agent logic **/
	void run();

  protected:
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

  private:
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

	/** Execute epilogue instructions, that is instructions after AI is executed
	 * @return true if AI should be executed, false if not
	 */
	virtual void epilogue() {}
};

#endif // AGENT_H
