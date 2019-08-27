/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2019 Adriano dos Santos Fernandes <adrianosf at gmail.com>
 *  and all contributors signed below.
 *
 */

#include "firebird.h"
#include "../common/SimilarToRegex.h"
#include "../common/StatusArg.h"
#include <unicode/utf8.h>

using namespace Firebird;

namespace
{
	static const unsigned FLAG_PREFER_FEWER = 0x01;
	static const unsigned FLAG_CASE_INSENSITIVE = 0x02;
	static const unsigned FLAG_GROUP_CAPTURE = 0x04;

	//// TODO: Verify usage of U8_NEXT_UNSAFE.
	class SimilarToCompiler
	{
	public:
		SimilarToCompiler(MemoryPool& pool, AutoPtr<RE2>& regexp, unsigned aFlags,
				const char* aPatternStr, unsigned aPatternLen,
				const char* escapeStr, unsigned escapeLen)
			: re2PatternStr(pool),
			  patternStr(aPatternStr),
			  patternPos(0),
			  patternLen(aPatternLen),
			  flags(aFlags),
			  useEscape(escapeStr != nullptr)
		{
			if (escapeStr)
			{
				int32_t escapePos = 0;
				U8_NEXT_UNSAFE(escapeStr, escapePos, escapeChar);

				if (escapePos != escapeLen)
					status_exception::raise(Arg::Gds(isc_escape_invalid));
			}

			if (flags & FLAG_GROUP_CAPTURE)
				re2PatternStr.append("(");

			int parseFlags;
			parseExpr(&parseFlags);

			if (flags & FLAG_GROUP_CAPTURE)
				re2PatternStr.append(")");

			// Check for proper termination.
			if (patternPos < patternLen)
				status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

			RE2::Options options;
			options.set_log_errors(false);
			options.set_dot_nl(true);
			options.set_case_sensitive(!(flags & FLAG_CASE_INSENSITIVE));

			re2::StringPiece sp((const char*) re2PatternStr.c_str(), re2PatternStr.length());
			regexp = FB_NEW_POOL(pool) RE2(sp, options);

			if (!regexp->ok())
				status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
		}

		bool hasChar()
		{
			return patternPos < patternLen;
		}

		UChar32 getChar()
		{
			fb_assert(hasChar());
			UChar32 c;
			U8_NEXT_UNSAFE(patternStr, patternPos, c);
			return c;
		}

		UChar32 peekChar()
		{
			auto savePos = patternPos;
			auto c = getChar();
			patternPos = savePos;
			return c;
		}

		bool isRep(UChar32 c) const
		{
			return c == '*' || c == '+' || c == '?' || c == '{';
		}

		bool isSpecial(UChar32 c)
		{
			switch (c)
			{
				case '^':
				case '-':
				case '_':
				case '%':
				case '[':
				case ']':
				case '(':
				case ')':
				case '{':
				case '}':
				case '|':
				case '?':
				case '+':
				case '*':
					return true;

				default:
					return false;
			}
		}

		bool isRe2Special(UChar32 c)
		{
			switch (c)
			{
				case '\\':
				case '$':
				case '.':
				case '^':
				case '-':
				case '_':
				case '[':
				case ']':
				case '(':
				case ')':
				case '{':
				case '}':
				case '|':
				case '?':
				case '+':
				case '*':
					return true;

				default:
					return false;
			}
		}

		void parseExpr(int* parseFlagOut)
		{
			while (true)
			{
				int parseFlags;
				parseTerm(&parseFlags);
				*parseFlagOut &= ~(~parseFlags & PARSE_FLAG_NOT_EMPTY);
				*parseFlagOut |= parseFlags;

				auto savePos = patternPos;
				UChar32 c;

				if (!hasChar() || (c = getChar()) != '|')
				{
					patternPos = savePos;
					break;
				}

				re2PatternStr.append("|");
			}
		}

		void parseTerm(int* parseFlagOut)
		{
			*parseFlagOut = 0;

			bool first = true;

			while (hasChar())
			{
				auto c = peekChar();

				if (c != '|' && c != ')')
				{
					int parseFlags;
					parseFactor(&parseFlags);

					*parseFlagOut |= parseFlags & PARSE_FLAG_NOT_EMPTY;

					if (first)
					{
						*parseFlagOut |= parseFlags;
						first = false;
					}
				}
				else
					break;
			}
		}

		void parseFactor(int* parseFlagOut)
		{
			int parseFlags;
			parsePrimary(&parseFlags);

			UChar32 op;

			if (!hasChar() || !isRep((op = peekChar())))
			{
				*parseFlagOut = parseFlags;
				return;
			}

			if (!(parseFlags & PARSE_FLAG_NOT_EMPTY) && op != '?')
				status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

			fb_assert(op == '*' || op == '+' || op == '?' || op == '{');

			if (op == '*')
			{
				re2PatternStr.append((flags & FLAG_PREFER_FEWER) ? "*?" : "*");
				*parseFlagOut = 0;
				++patternPos;
			}
			else if (op == '+')
			{
				re2PatternStr.append((flags & FLAG_PREFER_FEWER) ? "+?" : "+");
				*parseFlagOut = PARSE_FLAG_NOT_EMPTY;
				++patternPos;
			}
			else if (op == '?')
			{
				re2PatternStr.append((flags & FLAG_PREFER_FEWER) ? "??" : "?");
				*parseFlagOut = 0;
				++patternPos;
			}
			else if (op == '{')
			{
				const auto repeatStart = patternPos++;

				bool comma = false;
				string s1, s2;

				while (true)
				{
					if (!hasChar())
						status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

					UChar32 c = getChar();

					if (c == '}')
					{
						if (s1.isEmpty())
							status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
						break;
					}
					else if (c == ',')
					{
						if (comma)
							status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
						comma = true;
					}
					else
					{
						if (c >= '0' && c <= '9')
						{
							if (comma)
								s2 += (char) c;
							else
								s1 += (char) c;
						}
						else
							status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
					}
				}

				const int n1 = atoi(s1.c_str());
				*parseFlagOut = n1 == 0 ? 0 : PARSE_FLAG_NOT_EMPTY;

				re2PatternStr.append(patternStr + repeatStart, patternStr + patternPos);

				if (flags & FLAG_PREFER_FEWER)
					re2PatternStr.append("?");
			}

			if (hasChar() && isRep(peekChar()))
				status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
		}

		void parsePrimary(int* parseFlagOut)
		{
			*parseFlagOut = 0;

			fb_assert(hasChar());
			auto savePos = patternPos;
			auto op = getChar();

			if (op == '_')
			{
				*parseFlagOut |= PARSE_FLAG_NOT_EMPTY;
				re2PatternStr.append(".");
				return;
			}
			else if (op == '%')
			{
				re2PatternStr.append((flags & FLAG_PREFER_FEWER) ? ".*?" : ".*");
				return;
			}
			else if (op == '[')
			{
				struct
				{
					const char* similarClass;
					const char* re2ClassInclude;
					const char* re2ClassExclude;
				} static const classes[] =
					{
						{"alnum", "[:alnum:]", "[:^alnum:]"},
						{"alpha", "[:alpha:]", "[:^alpha:]"},
						{"digit", "[:digit:]", "[:^digit:]"},
						{"lower", "[:lower:]", "[:^lower:]"},
						{"space", " ", "\\x00-\\x1F\\x21-\\x{10FFFF}"},
						{"upper", "[:upper:]", "[:^upper:]"},
						{"whitespace", "[:space:]", "[:^space:]"}
					};

				struct Item
				{
					int clazz;
					unsigned firstStart, firstEnd, lastStart, lastEnd;
				};
				Array<Item> items;
				unsigned includeCount = 0;
				bool exclude = false;

				do
				{
					if (!hasChar())
						status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

					unsigned charSavePos = patternPos;
					UChar32 c = getChar();
					bool range = false;
					bool charClass = false;

					if (useEscape && c == escapeChar)
					{
						if (!hasChar())
							status_exception::raise(Arg::Gds(isc_escape_invalid));

						charSavePos = patternPos;
						c = getChar();

						if (!(c == escapeChar || isSpecial(c)))
							status_exception::raise(Arg::Gds(isc_escape_invalid));
					}
					else
					{
						if (c == '[')
							charClass = true;
						else if (c == '^')
						{
							if (exclude)
								status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

							exclude = true;
							continue;
						}
					}

					Item item;

					if (!exclude)
						++includeCount;

					if (charClass)
					{
						if (!hasChar() || getChar() != ':')
							status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

						charSavePos = patternPos;

						while (hasChar() && getChar() != ':')
							;

						const SLONG len = patternPos - charSavePos - 1;

						if (!hasChar() || getChar() != ']')
							status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

						for (item.clazz = 0; item.clazz < FB_NELEM(classes); ++item.clazz)
						{
							if (fb_utils::strnicmp(patternStr + charSavePos,
									classes[item.clazz].similarClass, len) == 0)
							{
								break;
							}
						}

						if (item.clazz >= FB_NELEM(classes))
							status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
					}
					else
					{
						item.clazz = -1;

						item.firstStart = item.lastStart = charSavePos;
						item.firstEnd = item.lastEnd = patternPos;

						if (hasChar() && peekChar() == '-')
						{
							getChar();

							charSavePos = patternPos;
							c = getChar();

							if (useEscape && c == escapeChar)
							{
								if (!hasChar())
									status_exception::raise(Arg::Gds(isc_escape_invalid));

								charSavePos = patternPos;
								c = getChar();

								if (!(c == escapeChar || isSpecial(c)))
									status_exception::raise(Arg::Gds(isc_escape_invalid));
							}

							item.lastStart = charSavePos;
							item.lastEnd = patternPos;
						}
					}

					items.add(item);

					if (!hasChar())
						status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
				} while (peekChar() != ']');

				auto appendItem = [&](const Item& item, bool negated) {
					if (item.clazz != -1)
					{
						re2PatternStr.append(negated ?
							classes[item.clazz].re2ClassExclude :
							classes[item.clazz].re2ClassInclude);
					}
					else
					{
						if (negated)
						{
							UChar32 c;
							char hex[20];

							int32_t cPos = item.firstStart;
							U8_NEXT_UNSAFE(patternStr, cPos, c);

							if (c > 0)
							{
								re2PatternStr.append("\\x00");
								re2PatternStr.append("-");

								sprintf(hex, "\\x{%X}", (int) c - 1);
								re2PatternStr.append(hex);
							}

							cPos = item.lastStart;
							U8_NEXT_UNSAFE(patternStr, cPos, c);

							if (c < 0x10FFFF)
							{
								sprintf(hex, "\\x{%X}", (int) c + 1);
								re2PatternStr.append(hex);
								re2PatternStr.append("-");
								re2PatternStr.append("\\x{10FFFF}");
							}
						}
						else
						{
							if (isRe2Special(patternStr[item.firstStart]))
								re2PatternStr.append("\\");

							re2PatternStr.append(patternStr + item.firstStart, patternStr + item.firstEnd);

							if (item.lastStart != item.firstStart)
							{
								re2PatternStr.append("-");

								if (isRe2Special(patternStr[item.lastStart]))
									re2PatternStr.append("\\");

								re2PatternStr.append(patternStr + item.lastStart, patternStr + item.lastEnd);
							}
						}
					}
				};

				if (exclude && includeCount > 1)
				{
					re2PatternStr.append("(?:");

					for (unsigned i = 0; i < includeCount; ++i)
					{
						if (i != 0)
							re2PatternStr.append("|");

						re2PatternStr.append("[");
						re2PatternStr.append("^");
						appendItem(items[i], true);

						for (unsigned j = includeCount; j < items.getCount(); ++j)
							appendItem(items[j], false);

						re2PatternStr.append("]");
					}

					re2PatternStr.append(")");
				}
				else
				{
					re2PatternStr.append("[");

					if (exclude)
						re2PatternStr.append("^");

					for (unsigned i = 0; i < items.getCount(); ++i)
						appendItem(items[i], exclude && i < includeCount);

					re2PatternStr.append("]");
				}

				getChar();
				*parseFlagOut |= PARSE_FLAG_NOT_EMPTY;
			}
			else if (op == '(')
			{
				re2PatternStr.append(flags & FLAG_GROUP_CAPTURE ? "(" : "(?:");

				int parseFlags;
				parseExpr(&parseFlags);

				if (!hasChar() || getChar() != ')')
					status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

				re2PatternStr.append(")");

				*parseFlagOut |= parseFlags & PARSE_FLAG_NOT_EMPTY;
			}
			else
			{
				patternPos = savePos;

				bool controlChar = false;

				do
				{
					auto charSavePos = patternPos;
					op = getChar();

					if (useEscape && op == escapeChar)
					{
						charSavePos = patternPos;
						op = getChar();

						if (!isSpecial(op) && op != escapeChar)
							status_exception::raise(Arg::Gds(isc_escape_invalid));
					}
					else
					{
						if (isSpecial(op))
						{
							controlChar = true;
							patternPos = charSavePos;
						}
					}

					if (!controlChar)
					{
						if (isRe2Special(op))
							re2PatternStr.append("\\");

						re2PatternStr.append(patternStr + charSavePos, patternStr + patternPos);
					}
				} while (!controlChar && hasChar());

				if (patternPos == savePos)
					status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

				*parseFlagOut |= PARSE_FLAG_NOT_EMPTY;
			}
		}

		const string& getRe2PatternStr() const
		{
			return re2PatternStr;
		}

	private:
		static const int PARSE_FLAG_NOT_EMPTY	= 1;	// known never to match empty string

		string re2PatternStr;
		const char* patternStr;
		int32_t patternPos;
		int32_t patternLen;
		UChar32 escapeChar;
		unsigned flags;
		bool useEscape;
	};

	class SubstringSimilarCompiler
	{
	public:
		SubstringSimilarCompiler(MemoryPool& pool, AutoPtr<RE2>& regexp, unsigned flags,
				const char* aPatternStr, unsigned aPatternLen,
				const char* escapeStr, unsigned escapeLen)
			: patternStr(aPatternStr),
			  patternPos(0),
			  patternLen(aPatternLen)
		{
			int32_t escapePos = 0;
			U8_NEXT_UNSAFE(escapeStr, escapePos, escapeChar);

			if (escapePos != escapeLen)
				status_exception::raise(Arg::Gds(isc_escape_invalid));

			unsigned positions[2];
			unsigned part = 0;

			while (hasChar())
			{
				auto c = getChar();

				if (c != escapeChar)
					continue;

				if (!hasChar())
					status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

				c = getChar();

				if (c == '"')
				{
					if (part >= 2)
						status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

					positions[part++] = patternPos;
				}
			}

			if (part != 2)
				status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));

			AutoPtr<RE2> regexp1, regexp2, regexp3;

			SimilarToCompiler compiler1(pool, regexp1, FLAG_PREFER_FEWER,
				aPatternStr, positions[0] - escapeLen - 1, escapeStr, escapeLen);

			SimilarToCompiler compiler2(pool, regexp2, 0,
				aPatternStr + positions[0], positions[1] - positions[0] - escapeLen - 1, escapeStr, escapeLen);

			SimilarToCompiler compiler3(pool, regexp3, FLAG_PREFER_FEWER,
				aPatternStr + positions[1], patternLen - positions[1], escapeStr, escapeLen);

			string finalRe2Pattern;
			finalRe2Pattern.reserve(
				1 + 	// (
				compiler1.getRe2PatternStr().length() +
				2 +		// )(
				compiler2.getRe2PatternStr().length() +
				2 +		// )(
				compiler3.getRe2PatternStr().length() +
				1		// )
			);

			finalRe2Pattern.append("(");
			finalRe2Pattern.append(compiler1.getRe2PatternStr());
			finalRe2Pattern.append(")(");
			finalRe2Pattern.append(compiler2.getRe2PatternStr());
			finalRe2Pattern.append(")(");
			finalRe2Pattern.append(compiler3.getRe2PatternStr());
			finalRe2Pattern.append(")");

			RE2::Options options;
			options.set_log_errors(false);
			options.set_dot_nl(true);
			options.set_case_sensitive(!(flags & FLAG_CASE_INSENSITIVE));

			re2::StringPiece sp((const char*) finalRe2Pattern.c_str(), finalRe2Pattern.length());
			regexp = FB_NEW_POOL(pool) RE2(sp, options);

			if (!regexp->ok())
				status_exception::raise(Arg::Gds(isc_invalid_similar_pattern));
		}

		bool hasChar()
		{
			return patternPos < patternLen;
		}

		UChar32 getChar()
		{
			fb_assert(hasChar());
			UChar32 c;
			U8_NEXT_UNSAFE(patternStr, patternPos, c);
			return c;
		}

		UChar32 peekChar()
		{
			auto savePos = patternPos;
			auto c = getChar();
			patternPos = savePos;
			return c;
		}

	private:
		const char* patternStr;
		int32_t patternPos;
		int32_t patternLen;
		UChar32 escapeChar;
	};
}	// namespace

namespace Firebird {


SimilarToRegex::SimilarToRegex(MemoryPool& pool, bool caseInsensitive,
		const char* patternStr, unsigned patternLen, const char* escapeStr, unsigned escapeLen)
	: PermanentStorage(pool)
{
	SimilarToCompiler compiler(pool, regexp,
		FLAG_GROUP_CAPTURE | FLAG_PREFER_FEWER | (caseInsensitive ? FLAG_CASE_INSENSITIVE : 0),
		patternStr, patternLen, escapeStr, escapeLen);
}

bool SimilarToRegex::matches(const char* buffer, unsigned bufferLen, Array<MatchPos>* matchPosArray)
{
	re2::StringPiece sp(buffer, bufferLen);

	if (matchPosArray)
	{
		const int argsCount = regexp->NumberOfCapturingGroups();

		Array<re2::StringPiece> resSps(argsCount);
		resSps.resize(argsCount);

		Array<RE2::Arg> args(argsCount);
		args.resize(argsCount);

		Array<RE2::Arg*> argsPtr(argsCount);

		{	// scope
			auto resSp = resSps.begin();

			for (auto& arg : args)
			{
				arg = resSp++;
				argsPtr.push(&arg);
			}
		}

		if (RE2::FullMatchN(sp, *regexp.get(), argsPtr.begin(), argsCount))
		{
			matchPosArray->clear();

			for (const auto resSp : resSps)
			{
				matchPosArray->push(MatchPos{
					static_cast<unsigned>(resSp.data() - sp.begin()),
					static_cast<unsigned>(resSp.length())
				});
			}

			return true;
		}
		else
			return false;
	}
	else
		return RE2::FullMatch(sp, *regexp.get());
}

//---------------------

SubstringSimilarRegex::SubstringSimilarRegex(MemoryPool& pool, bool caseInsensitive,
		const char* patternStr, unsigned patternLen, const char* escapeStr, unsigned escapeLen)
	: PermanentStorage(pool)
{
	SubstringSimilarCompiler compiler(pool, regexp,
		(caseInsensitive ? FLAG_CASE_INSENSITIVE : 0),
		patternStr, patternLen, escapeStr, escapeLen);
}

bool SubstringSimilarRegex::matches(const char* buffer, unsigned bufferLen,
	unsigned* resultStart, unsigned* resultLength)
{
	re2::StringPiece sp(buffer, bufferLen);

	re2::StringPiece spResult;

	if (RE2::FullMatch(sp, *regexp.get(), nullptr, &spResult, nullptr))
	{
		*resultStart = spResult.begin() - buffer;
		*resultLength = spResult.length();
		return true;
	}
	else
		return false;
}


}	// namespace Firebird
