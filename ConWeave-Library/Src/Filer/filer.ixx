export module sys.filer;

import utility;

import filer.basic;
import sys.filoader;
import sys.reader;
import sys.writer;
import sys.access;

export namespace sys
{
	template <rest::character CharType>
	using filer = basic_filer <
		CharType, filoader, reader, writer
	>;

	using ::comaccess;
	using ::permission;
	using ::read_order;

	template <rest::character CharType, read_order Order = read_order::def>
	using adapt_filer = basic_adapt_filer<CharType, Order>;
}