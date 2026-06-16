export module sys.filer;

import utility;

import filer.basic;
import sys.filoader;
import sys.reader;
import sys.writer;

export import sys.access;

export namespace sys
{
	template <rest::character CharType>
	using filer = basic_filer <
		CharType, filoader, reader, writer
	>;

	template <rest::character CharType>
	using adapt_filer = basic_adapt_filer<CharType>;
}