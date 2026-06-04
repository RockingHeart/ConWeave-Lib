export module sys.filer;

import utility;

import filer.basic;
import sys.filoader;
import sys.reader;
import sys.mapping_reader;
import sys.writer;
import sys.mapping_writer;

export import sys.access;

export namespace sys
{
	template <rest::character CharType>
	using filer = basic_filer<CharType, reader, writer>;

	template <rest::character CharType>
	using mapping_filer = basic_filer<CharType, mapping_reader, mapping_writer>;
}