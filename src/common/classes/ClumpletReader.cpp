#include "firebird.h"

#include "../common/classes/ClumpletReader.h"
#include "fb_exception.h"

namespace Firebird {

ClumpletReader::ClumpletReader(bool isTagged, const UCHAR* buffer, size_t buffLen) :
	cur_offset(isTagged ? 1 : 0), mIsTagged(isTagged), static_buffer(buffer), 
	  static_buffer_end(buffer + buffLen) 
{
}

void ClumpletReader::usage_mistake(const char* what) {
	fatal_exception::raiseFmt(
		"Internal error when using clumplet API: %s", what);
}

void ClumpletReader::invalid_structure() {
	fatal_exception::raise("Invalid clumplet buffer structure");
}

UCHAR ClumpletReader::getBufferTag() {
	if (!mIsTagged) {
		usage_mistake("buffer is not tagged");
		return 0;
	}

	const UCHAR* buffer_end = getBufferEnd();
	const UCHAR* buffer_start = getBuffer();
	if (buffer_end - buffer_start == 0) {
		invalid_structure();
		return 0;
	}
	return buffer_start[0];
}

void ClumpletReader::moveNext() {
	const UCHAR* clumplet = getBuffer() + cur_offset;
	const UCHAR* buffer_end = getBufferEnd();

	// Check for EOF
	if (clumplet >= buffer_end) {
		usage_mistake("read past EOF");
		return;
	}

	// It appears we didn't receive length component for clumplet
	if (buffer_end - clumplet < 2) {
		invalid_structure();
		return;
	}

	size_t length = clumplet[1];
	cur_offset += length + 2;
}

// Methods which work with currently selected clumplet
UCHAR ClumpletReader::getClumpTag() {
	const UCHAR* clumplet = getBuffer() + cur_offset;
	const UCHAR* buffer_end = getBufferEnd();

	// Check for EOF
	if (clumplet >= buffer_end) {
		usage_mistake("read past EOF");
		return 0;
	}

	return clumplet[0];
}

size_t ClumpletReader::getClumpLength() {
	const UCHAR* clumplet = getBuffer() + cur_offset;
	const UCHAR* buffer_end = getBufferEnd();

	// Check for EOF
	if (clumplet >= buffer_end) {
		usage_mistake("read past EOF");
		return 0;
	}

	// See if we have length byte available
	if (buffer_end - clumplet < 2) {
		invalid_structure();
		return 0;
	}

	// See if length seems kosher
	size_t length = clumplet[1];
	if (buffer_end - clumplet < length + 1) {
		invalid_structure();
		return buffer_end - clumplet - 1;
	}

	return length;
}

SLONG ClumpletReader::getInt() {
	const UCHAR* clumplet = getBuffer() + cur_offset;
	size_t length = getClumpLength();

	if (length > 4)
		invalid_structure();

	const UCHAR* ptr = clumplet + 2;

	// This code is taken from gds__vax_integer
	SLONG value = 0;
	int shift = 0;
	while (length > 0) {
		--length;
		value += ((SLONG) *ptr++) << shift;
		shift += 8;
	}

	return value;
}

SINT64 ClumpletReader::getBigInt() {
	const UCHAR* clumplet = getBuffer() + cur_offset;
	size_t length = getClumpLength();

	if (length > 8)
		invalid_structure();

	const UCHAR* ptr = clumplet + 2;

	// This code is taken from isc_portable_integer
	SINT64 value = 0;
	int shift = 0;
	while (length > 0) {
		--length;
		value += ((SINT64) *ptr++) << shift;
		shift += 8;
	}

	return value;
}

string& ClumpletReader::getString(string& str) {
	const UCHAR* clumplet = getBuffer() + cur_offset;
	size_t length = getClumpLength();
	str.assign(reinterpret_cast<const char*>(clumplet + 2), length);
	return str;
}

PathName& ClumpletReader::getPath(PathName& str) 
{
	const UCHAR* clumplet = getBuffer() + cur_offset;
	size_t length = getClumpLength();
	str.assign(reinterpret_cast<const char*>(clumplet + 2), length);
	return str;
}

bool ClumpletReader::getBoolean()
{
	const UCHAR* clumplet = getBuffer() + cur_offset;
	size_t length = getClumpLength();
	return length && clumplet[2];
}

}
