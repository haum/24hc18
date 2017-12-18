#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../engine/gears/lineparser.h"

#include <sstream>
#include <vector>

using namespace testing;

class GearsLineParser : public testing::Test {
  public:
	void SetUp() override {
		lp.setExecute([this](uint8_t argc, const char **argv) {
			for (uint8_t i = 0; i < argc; ++i)
				out << argv[i] << ((i < argc - 1) ? '/' : '@');
		});
	}

	void reset() { read_ptr = 0; }

	ssize_t read(char *out, size_t sz) {
		size_t inputlen = input.length();
		if (read_ptr + sz > inputlen)
			sz = inputlen - read_ptr;
		if (sz <= 0)
			return 0;
		strncpy(out, &(input.c_str()[read_ptr]), sz);
		read_ptr += sz;
		return static_cast<ssize_t>(sz);
	}

	bool eof() { return read_ptr >= input.length(); }

	std::string output() { return out.str(); }

  protected:
	std::string input;
	std::ostringstream out;
	LineParser<100> lp;
	std::vector<LineParserError> err;
	size_t read_ptr{0};
};

#define READ()                                                                 \
	do {                                                                       \
		err.push_back(                                                         \
		    lp.read([this](char *out, size_t sz) { return read(out, sz); }));  \
	} while (!eof())

TEST_F(GearsLineParser, EmptyInput) {
	input = "";
	READ();
	for (auto e : err)
		EXPECT_EQ(e,
		          LineParserError::READ_ERROR); // End of file before new line
	EXPECT_EQ(output(), "");
}

TEST_F(GearsLineParser, EmptyLine) {
	input = "\n";
	READ();
	for (auto e : err)
		EXPECT_EQ(e, LineParserError::NO_ERROR);
	EXPECT_EQ(output(), "");
}

TEST_F(GearsLineParser, TooLong) {
	input = "12345678901234567890123456789012345678901234567890123456789"
	        "012345678901234567890123456789012345678901234567890123456789"
	        "012345678901234567890123456789012345678901234567890123456789\n";
	READ();
	EXPECT_EQ(err.size(), 2);
	EXPECT_EQ(err[0], LineParserError::LINE_TOO_LONG);
	EXPECT_EQ(err[1], LineParserError::NO_ERROR);
	EXPECT_EQ(output(), "");
}

TEST_F(GearsLineParser, TooLongWithOk) {
	input = "ABC 1 2 34\n"
	        "12345678901234567890123456789012345678901234567890123456789"
	        "012345678901234567890123456789012345678901234567890123456789"
	        "012345678901234567890123456789012345678901234567890123456789\n"
	        "DEF 12 3 4\n";
	std::string expected_out = "ABC/1/2/34@"
	                           "DEF/12/3/4@";
	READ();
	EXPECT_EQ(err.size(), 3);
	EXPECT_EQ(err[0], LineParserError::NO_ERROR);
	EXPECT_EQ(err[1], LineParserError::LINE_TOO_LONG);
	EXPECT_EQ(err[2], LineParserError::NO_ERROR);
	EXPECT_EQ(output(), expected_out);
}

TEST_F(GearsLineParser, DoubleSpace) {
	input = "COUCOU 123  456 789\n";
	std::string expected_out = "COUCOU/123/456/789@";
	READ();
	for (auto e : err)
		EXPECT_EQ(e, LineParserError::NO_ERROR);
	EXPECT_EQ(output(), expected_out);
}

TEST_F(GearsLineParser, TwentyPlusArgs) {
	input = "TWENTY 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21\n";
	std::string expected_out =
	    "TWENTY/1/2/3/4/5/6/7/8/9/10/11/12/13/14/15/16/17/18/19 20 21@";
	READ();
	for (auto e : err)
		EXPECT_EQ(e, LineParserError::NO_ERROR);
	EXPECT_EQ(output(), expected_out);
}

TEST_F(GearsLineParser, SimpleMultiline) {
	input = "COUCOU 123 456 789\n"
	        "TEST 22 STARS 42 HH vqN\n"
	        "TSAR AA\n";
	std::string expected_out = "COUCOU/123/456/789@"
	                           "TEST/22/STARS/42/HH/vqN@"
	                           "TSAR/AA@";
	READ();
	for (auto e : err)
		EXPECT_EQ(e, LineParserError::NO_ERROR);
	EXPECT_EQ(output(), expected_out);
}

TEST_F(GearsLineParser, LongMultiline) {
	input = "COUCOU 123 456 789\n"
	        "TEST 22 STARS 42 HH vqN\n"
	        "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18\n"
	        "012 345 678901 234 5678 90 1 23456 789 01 23 456 7890123 4567\n"
	        "TSAR AA\n";
	std::string expected_out =
	    "COUCOU/123/456/789@"
	    "TEST/22/STARS/42/HH/vqN@"
	    "1/2/3/4/5/6/7/8/9/10/11/12/13/14/15/16/17/18@"
	    "012/345/678901/234/5678/90/1/23456/789/01/23/456/7890123/4567@"
	    "TSAR/AA@";
	READ();
	for (auto e : err)
		EXPECT_EQ(e, LineParserError::NO_ERROR);
	EXPECT_EQ(output(), expected_out);
}

TEST_F(GearsLineParser, SplitedReadLine) {
	input = "COUCOU 123 45";
	READ();
	reset();
	input = "6 789\n";
	READ();
	std::string expected_out = "COUCOU/123/456/789@";
	for (auto e : err)
		EXPECT_EQ(e, LineParserError::NO_ERROR);
	EXPECT_EQ(output(), expected_out);
}

TEST_F(GearsLineParser, TripleSplitedReadLine) {
	input = "COUCOU 1";
	READ();
	reset();
	input = "23 45";
	READ();
	reset();
	input = "6 789\n";
	READ();
	std::string expected_out = "COUCOU/123/456/789@";
	for (auto e : err)
		EXPECT_EQ(e, LineParserError::NO_ERROR);
	EXPECT_EQ(output(), expected_out);
}
