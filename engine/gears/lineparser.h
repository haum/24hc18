#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <array>
#include <cstdint>
#include <functional>
#include <sys/types.h>

enum LineParserError : char {
	NO_ERROR,
	LINE_TOO_LONG,
	READ_ERROR,
};

class LineParserBase {
  public:
	/** Constructor
	 * @param buffer Pointer to buffer
	 * @param sz     Size of buffer (max size of line + 1)
	 */
	LineParserBase(char *buffer, size_t sz) : m_buff{buffer}, m_size{sz} {}

	/** Read next characters
	 * @param fn Function to read (parameters are buffer and max len,
	 *           should return number of read characters)
	 */
	LineParserError read(std::function<int(char *, ssize_t)> fn) {
		ssize_t len = 0;
		len = m_size - 1 - m_buff_len;
		if (len <= 0)
			return LineParserError::READ_ERROR;
		len = fn(m_buff + m_buff_len, len);
		if (len <= 0)
			return LineParserError::READ_ERROR;
		len += m_buff_len;
		return read(len);
	}

	void setExecute(std::function<void(uint8_t, const char **)> exe) {
		m_execute = exe;
	}

	void reset() { m_buff_len = 0; }

  private:
	/** Private part of read operation **/
	LineParserError read(ssize_t len);

	/** Cut line into tokens and call execute
	 * @param line Null terminated line string
	 */
	void processLine(char *line);

	/** Buffer pointer **/
	char *m_buff = nullptr;

	/** Buffer size **/
	size_t m_size = 0;

	/** Buffer usage **/
	ssize_t m_buff_len = 0;

	/** Line interpreter function pointer **/
	std::function<void(uint8_t, const char **)> m_execute = nullptr;
};

template <int N> class LineParser : public LineParserBase {
  public:
	LineParser() : LineParserBase{m_buffer.data(), m_buffer.size()} {}

  private:
	std::array<char, N + 1> m_buffer;
};

#endif // LINEPARSER_H