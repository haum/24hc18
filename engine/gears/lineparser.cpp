#include "lineparser.h"
#include <cstring>

LineParserError LineParserBase::read(ssize_t len) {
	if (len <= 0) {
		return LineParserError::READ_ERROR;
	} else if (!strstr(m_buff, "\n")) {
		return LineParserError::LINE_TOO_LONG;
	} else {
		m_buff[len] = 0;
		char *ntok;
		char *narg = strtok_r(m_buff, "\n", &ntok);
		while (narg != nullptr) {
			bool uncut = strstr(ntok, "\n");
			processLine(narg);
			if (uncut) {
				narg = strtok_r(nullptr, "\n", &ntok);
			} else {
				strcpy(m_buff, ntok);
				m_buff_len = static_cast<ssize_t>(strlen(m_buff));
				narg = nullptr;
			}
		}
	}
	return LineParserError::NO_ERROR;
}

void LineParserBase::processLine(char *line) {
	char *stok;
	char *sarg = strtok_r(line, " ", &stok);
	uint8_t argc = 0;
	constexpr size_t argslen = 20;
	const char *args[20];
	while (sarg != nullptr && argc < argslen - 1) {
		args[argc] = sarg;
		++argc;
		if (argc < argslen - 1) {
			sarg = strtok_r(nullptr, " ", &stok);
		} else {
			args[argc] = stok;
			++argc;
		}
	}
	if (argc > 0 && m_execute)
		m_execute(argc, args);
}
