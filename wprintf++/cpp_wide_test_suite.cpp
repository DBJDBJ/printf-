// licence is et eof
#include "pch.h"

#define CATCH_CONFIG_MAIN
// use the 'catch' test framework
#include "catch.hpp"

#define PRINTF_USER_DEFINED_PUTCHAR
#include "wprintf++.h"

namespace wide_test {

	constexpr const inline wchar_t		WCHAR_EOS { wchar_t(0)  };
	constexpr const inline std::size_t	WCHAR_SIZE{ sizeof(wchar_t) };

	constexpr inline std::size_t 
		printf_buffer_size = 100U;
	inline std::wstring	
		printf_buffer( printf_buffer_size, WCHAR_EOS);
	inline std::wstring::iterator 
		printf_idx = printf_buffer.begin() ;

	inline auto printf_buffer_equals (const wchar_t * specimen) -> bool 
	{
		bool retval = 0 == std::wcscmp(specimen, printf_buffer.c_str());
		return retval;
	};

	inline auto printf_buffer_equals (const wchar_t * left, const wchar_t * right) -> bool
	{
		bool retval = 0 == std::wcscmp(left, right);
		return retval;
	};

	inline std::wstring::iterator
		char_to_buffer ( wchar_t next_wchar ) 
	{
		// check & put
		if (printf_idx != printf_buffer.end()) {
			*printf_idx = next_wchar;
		}
		else {
			throw __FUNCSIG__ " -- buffer overflow!";
		}
		// advance
		return (printf_idx++);
	}

	inline wchar_t * reset_buffering() {
		printf_idx = printf_buffer.begin();
		printf_buffer.clear();
		printf_buffer.resize( printf_buffer_size );
		return printf_buffer.data();
	}
}

namespace mpaland_dbjdbj::wchar {
	PRINTF_EXTERN_C inline void _putchar(wchar_t character)
	{
		wide_test::char_to_buffer(character);
	}
}

namespace wprintf_plusplus_tests {

	using mpaland_dbjdbj::wchar::printf;
	using mpaland_dbjdbj::wchar::sprintf;
	using mpaland_dbjdbj::wchar::fctprintf;
	using mpaland_dbjdbj::wchar::vsnprintf;
	using mpaland_dbjdbj::wchar::snprintf;
	
void _out_fct(wchar_t character, void* arg)
{
	(void)arg;
	wide_test::char_to_buffer(character);
}

TEST_CASE( "wide printf", "[]") {
	wide_test::reset_buffering();
	REQUIRE(
		printf(L"% d", 4232) == 5
	);
	REQUIRE(wide_test::printf_buffer[5] == wide_test::WCHAR_EOS  ); 
	// wide_test::printf_buffer[5] = wchar_t(0); // ?
	REQUIRE(wide_test::printf_buffer_equals(L" 4232"));
}


TEST_CASE( "wide fctprintf", "[]") {
	wide_test::reset_buffering();
	fctprintf(&_out_fct, nullptr, L"This is a test of %X", 0x12EFU);
	REQUIRE(wide_test::printf_buffer_equals(L"This is a test of 12EF"));
}


TEST_CASE( "wide snprintf", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	snprintf(buffer , 100U, L"%d", -1000);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"-1000"));

	snprintf(buffer, 3U, L"%d", -1000);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"-1"));
}


static void vsnprintf_builder_1(wchar_t* buffer, ...)
{
	va_list args;
	va_start(args, buffer);
	vsnprintf(buffer, 100U, L"%d", args);
	va_end(args);
}

static void vsnprintf_builder_3(wchar_t* buffer, ...)
{
	va_list args;
	va_start(args, buffer);
	vsnprintf(buffer, 100U, L"%d %d %s", args);
	va_end(args);
}

TEST_CASE( "wide vsnprintf", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	vsnprintf_builder_1(buffer, -1);
	REQUIRE(wide_test::printf_buffer_equals(buffer,L"-1"));

	vsnprintf_builder_3(buffer, 3, -1000, L"test");
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"3 -1000 test"));
}


TEST_CASE( "wide space flag", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"% d", 42);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L" 42"));

	sprintf(buffer, L"% d", -42);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"-42"));

	sprintf(buffer, L"% 5d", 42);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"   42"));

	sprintf(buffer, L"% 5d", -42);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"  -42"));

	sprintf(buffer, L"% 15d", 42);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"             42"));

	sprintf(buffer, L"% 15d", -42);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"            -42"));

	sprintf(buffer, L"% 15d", -42);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"            -42"));

	sprintf(buffer, L"% 15.3f", -42.987);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"        -42.987"));

	sprintf(buffer, L"% 15.3f", 42.987);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"         42.987"));

	sprintf(buffer, L"% s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"Hello testing"));

	sprintf(buffer, L"% d", 1024);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L" 1024"));

	sprintf(buffer, L"% d", -1024);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"-1024"));

	sprintf(buffer, L"% i", 1024);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L" 1024"));

	sprintf(buffer, L"% i", -1024);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"-1024"));

	sprintf(buffer, L"% u", 1024);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"1024"));

	sprintf(buffer, L"% u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"4294966272"));

	sprintf(buffer, L"% o", 511);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"777"));

	sprintf(buffer, L"% o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"37777777001"));

	sprintf(buffer, L"% x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"1234abcd"));

	sprintf(buffer, L"% x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"edcb5433"));

	sprintf(buffer, L"% X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"1234ABCD"));

	sprintf(buffer, L"% X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"EDCB5433"));

	sprintf(buffer, L"% c", 'x');
	REQUIRE(wide_test::printf_buffer_equals(buffer, L"x"));
}


TEST_CASE( "wide + flag", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%+d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"+42"));

	sprintf(buffer, L"%+d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42"));

	sprintf(buffer, L"%+5d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"  +42"));

	sprintf(buffer, L"%+5d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"  -42"));

	sprintf(buffer, L"%+15d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"            +42"));

	sprintf(buffer, L"%+15d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"            -42"));

	sprintf(buffer, L"%+s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"Hello testing"));

	sprintf(buffer, L"%+d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"+1024"));

	sprintf(buffer, L"%+d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-1024"));

	sprintf(buffer, L"%+i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"+1024"));

	sprintf(buffer, L"%+i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-1024"));

	sprintf(buffer, L"%+u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1024"));

	sprintf(buffer, L"%+u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"4294966272"));

	sprintf(buffer, L"%+o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"777"));

	sprintf(buffer, L"%+o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"37777777001"));

	sprintf(buffer, L"%+x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1234abcd"));

	sprintf(buffer, L"%+x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"edcb5433"));

	sprintf(buffer, L"%+X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1234ABCD"));

	sprintf(buffer, L"%+X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"EDCB5433"));

	sprintf(buffer, L"%+c", 'x');
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"x"));
}


TEST_CASE( "wide 0 flag", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%0d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42"));

	sprintf(buffer, L"%0ld", 42L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42"));

	sprintf(buffer, L"%0d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-42"));

	sprintf(buffer, L"%05d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00042"));

	sprintf(buffer, L"%05d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-0042"));

	sprintf(buffer, L"%015d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"000000000000042"));

	sprintf(buffer, L"%015d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-00000000000042"));

	sprintf(buffer, L"%015.2f", 42.1234);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"000000000042.12"));

	sprintf(buffer, L"%015.3f", 42.9876);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000042.988"));

	sprintf(buffer, L"%015.5f", -42.9876);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-00000042.98760"));
}


TEST_CASE( "wide - flag", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%-d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42"));

	sprintf(buffer, L"%-d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42"));

	sprintf(buffer, L"%-5d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42   "));

	sprintf(buffer, L"%-5d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42  "));

	sprintf(buffer, L"%-15d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42             "));

	sprintf(buffer, L"%-15d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42            "));

	sprintf(buffer, L"%-0d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42"));

	sprintf(buffer, L"%-0d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42"));

	sprintf(buffer, L"%-05d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42   "));

	sprintf(buffer, L"%-05d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42  "));

	sprintf(buffer, L"%-015d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42             "));

	sprintf(buffer, L"%-015d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42            "));

	sprintf(buffer, L"%0-d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42"));

	sprintf(buffer, L"%0-d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42"));

	sprintf(buffer, L"%0-5d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42   "));

	sprintf(buffer, L"%0-5d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42  "));

	sprintf(buffer, L"%0-15d", 42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"42             "));

	sprintf(buffer, L"%0-15d", -42);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-42            "));
}


TEST_CASE( "wide specifier", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"Hello testing"));

	sprintf(buffer, L"%s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"Hello testing"));

	sprintf(buffer, L"%d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1024"));

	sprintf(buffer, L"%d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-1024"));

	sprintf(buffer, L"%i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1024"));

	sprintf(buffer, L"%i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-1024"));

	sprintf(buffer, L"%u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1024"));

	sprintf(buffer, L"%u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"4294966272"));

	sprintf(buffer, L"%o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"777"));

	sprintf(buffer, L"%o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"37777777001"));

	sprintf(buffer, L"%x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1234abcd"));

	sprintf(buffer, L"%x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"edcb5433"));

	sprintf(buffer, L"%X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1234ABCD"));

	sprintf(buffer, L"%X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"EDCB5433"));

	sprintf(buffer, L"%%");
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"%"));
}


TEST_CASE( "wide width", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%1s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"Hello testing"));

	sprintf(buffer, L"%1d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1024"));

	sprintf(buffer, L"%1d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-1024"));

	sprintf(buffer, L"%1i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1024"));

	sprintf(buffer, L"%1i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-1024"));

	sprintf(buffer, L"%1u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1024"));

	sprintf(buffer, L"%1u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"4294966272"));

	sprintf(buffer, L"%1o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"777"));

	sprintf(buffer, L"%1o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"37777777001"));

	sprintf(buffer, L"%1x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1234abcd"));

	sprintf(buffer, L"%1x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"edcb5433"));

	sprintf(buffer, L"%1X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1234ABCD"));

	sprintf(buffer, L"%1X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"EDCB5433"));

	sprintf(buffer, L"%1c", 'x');
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"x"));
}


TEST_CASE( "wide width 20", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%20s", L"Hello");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               Hello"));

	sprintf(buffer, L"%20d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%20d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               -1024"));

	sprintf(buffer, L"%20i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%20i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               -1024"));

	sprintf(buffer, L"%20u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%20u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"          4294966272"));

	sprintf(buffer, L"%20o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                 777"));

	sprintf(buffer, L"%20o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"         37777777001"));

	sprintf(buffer, L"%20x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234abcd"));

	sprintf(buffer, L"%20x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            edcb5433"));

	sprintf(buffer, L"%20X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234ABCD"));

	sprintf(buffer, L"%20X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            EDCB5433"));

	sprintf(buffer, L"%20c", 'x');
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                   x"));
}


TEST_CASE( "wide width *20", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%*s", 20, L"Hello");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               Hello"));

	sprintf(buffer, L"%*d", 20, 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%*d", 20, -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               -1024"));

	sprintf(buffer, L"%*i", 20, 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%*i", 20, -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               -1024"));

	sprintf(buffer, L"%*u", 20, 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%*u", 20, 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"          4294966272"));

	sprintf(buffer, L"%*o", 20, 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                 777"));

	sprintf(buffer, L"%*o", 20, 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"         37777777001"));

	sprintf(buffer, L"%*x", 20, 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234abcd"));

	sprintf(buffer, L"%*x", 20, 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            edcb5433"));

	sprintf(buffer, L"%*X", 20, 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234ABCD"));

	sprintf(buffer, L"%*X", 20, 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            EDCB5433"));

	sprintf(buffer, L"%*c", 20, 'x');
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                   x"));
}


TEST_CASE( "wide width -20", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%-20s", L"Hello");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"Hello               "));

	sprintf(buffer, L"%-20d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1024                "));

	sprintf(buffer, L"%-20d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-1024               "));

	sprintf(buffer, L"%-20i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1024                "));

	sprintf(buffer, L"%-20i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-1024               "));

	sprintf(buffer, L"%-20u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1024                "));

	sprintf(buffer, L"%-20.4f", 1024.1234);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1024.1234           "));

	sprintf(buffer, L"%-20u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"4294966272          "));

	sprintf(buffer, L"%-20o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"777                 "));

	sprintf(buffer, L"%-20o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"37777777001         "));

	sprintf(buffer, L"%-20x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1234abcd            "));

	sprintf(buffer, L"%-20x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"edcb5433            "));

	sprintf(buffer, L"%-20X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1234ABCD            "));

	sprintf(buffer, L"%-20X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"EDCB5433            "));

	sprintf(buffer, L"%-20c", 'x');
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"x                   "));

	sprintf(buffer, L"|%5d| |%-2d| |%5d|", 9, 9, 9);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"|    9| |9 | |    9|"));

	sprintf(buffer, L"|%5d| |%-2d| |%5d|", 10, 10, 10);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"|   10| |10| |   10|"));

	sprintf(buffer, L"|%5d| |%-12d| |%5d|", 9, 9, 9);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"|    9| |9           | |    9|"));

	sprintf(buffer, L"|%5d| |%-12d| |%5d|", 10, 10, 10);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"|   10| |10          | |   10|"));
}


TEST_CASE( "wide width 0-20", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%0-20s", L"Hello");
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"Hello               "));

	sprintf(buffer, L"%0-20d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1024                "));

	sprintf(buffer, L"%0-20d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-1024               "));

	sprintf(buffer, L"%0-20i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1024                "));

	sprintf(buffer, L"%0-20i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-1024               "));

	sprintf(buffer, L"%0-20u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1024                "));

	sprintf(buffer, L"%0-20u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"4294966272          "));

	sprintf(buffer, L"%0-20o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"777                 "));

	sprintf(buffer, L"%0-20o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"37777777001         "));

	sprintf(buffer, L"%0-20x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1234abcd            "));

	sprintf(buffer, L"%0-20x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"edcb5433            "));

	sprintf(buffer, L"%0-20X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"1234ABCD            "));

	sprintf(buffer, L"%0-20X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"EDCB5433            "));

	sprintf(buffer, L"%0-20c", 'x');
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"x                   "));
}


TEST_CASE( "wide padding 20", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%020d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"00000000000000001024"));

	sprintf(buffer, L"%020d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-0000000000000001024"));

	sprintf(buffer, L"%020i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"00000000000000001024"));

	sprintf(buffer, L"%020i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"-0000000000000001024"));

	sprintf(buffer, L"%020u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"00000000000000001024"));

	sprintf(buffer, L"%020u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"00000000004294966272"));

	sprintf(buffer, L"%020o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"00000000000000000777"));

	sprintf(buffer, L"%020o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"00000000037777777001"));

	sprintf(buffer, L"%020x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"0000000000001234abcd"));

	sprintf(buffer, L"%020x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"000000000000edcb5433"));

	sprintf(buffer, L"%020X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"0000000000001234ABCD"));

	sprintf(buffer, L"%020X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"000000000000EDCB5433"));
}


TEST_CASE( "wide padding .20", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%.20d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000001024"));

	sprintf(buffer, L"%.20d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-00000000000000001024"));

	sprintf(buffer, L"%.20i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000001024"));

	sprintf(buffer, L"%.20i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-00000000000000001024"));

	sprintf(buffer, L"%.20u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000001024"));

	sprintf(buffer, L"%.20u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000004294966272"));

	sprintf(buffer, L"%.20o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000000777"));

	sprintf(buffer, L"%.20o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000037777777001"));

	sprintf(buffer, L"%.20x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0000000000001234abcd"));

	sprintf(buffer, L"%.20x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"000000000000edcb5433"));

	sprintf(buffer, L"%.20X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0000000000001234ABCD"));

	sprintf(buffer, L"%.20X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"000000000000EDCB5433"));
}


TEST_CASE( "wide padding #020", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%#020d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000001024"));

	sprintf(buffer, L"%#020d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-0000000000000001024"));

	sprintf(buffer, L"%#020i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000001024"));

	sprintf(buffer, L"%#020i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-0000000000000001024"));

	sprintf(buffer, L"%#020u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000001024"));

	sprintf(buffer, L"%#020u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000004294966272"));

	sprintf(buffer, L"%#020o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000000000000777"));

	sprintf(buffer, L"%#020o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000037777777001"));

	sprintf(buffer, L"%#020x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0x00000000001234abcd"));

	sprintf(buffer, L"%#020x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0x0000000000edcb5433"));

	sprintf(buffer, L"%#020X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0X00000000001234ABCD"));

	sprintf(buffer, L"%#020X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0X0000000000EDCB5433"));
}


TEST_CASE( "wide padding #20", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%#20d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"                1024"));

	sprintf(buffer, L"%#20d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"               -1024"));

	sprintf(buffer, L"%#20i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"                1024"));

	sprintf(buffer, L"%#20i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"               -1024"));

	sprintf(buffer, L"%#20u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"                1024"));

	sprintf(buffer, L"%#20u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"          4294966272"));

	sprintf(buffer, L"%#20o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"                0777"));

	sprintf(buffer, L"%#20o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"        037777777001"));

	sprintf(buffer, L"%#20x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"          0x1234abcd"));

	sprintf(buffer, L"%#20x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"          0xedcb5433"));

	sprintf(buffer, L"%#20X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"          0X1234ABCD"));

	sprintf(buffer, L"%#20X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer,  L"          0XEDCB5433"));
}


TEST_CASE( "wide padding 20.5", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%20.5d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               01024"));

	sprintf(buffer, L"%20.5d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"              -01024"));

	sprintf(buffer, L"%20.5i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               01024"));

	sprintf(buffer, L"%20.5i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"              -01024"));

	sprintf(buffer, L"%20.5u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               01024"));

	sprintf(buffer, L"%20.5u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"          4294966272"));

	sprintf(buffer, L"%20.5o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               00777"));

	sprintf(buffer, L"%20.5o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"         37777777001"));

	sprintf(buffer, L"%20.5x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234abcd"));

	sprintf(buffer, L"%20.10x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"          00edcb5433"));

	sprintf(buffer, L"%20.5X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234ABCD"));

	sprintf(buffer, L"%20.10X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"          00EDCB5433"));
}


TEST_CASE( "wide length", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%.0s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L""));

	sprintf(buffer, L"%20.0s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));

	sprintf(buffer, L"%.s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L""));

	sprintf(buffer, L"%20.s", L"Hello testing");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));

	sprintf(buffer, L"%20.0d", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%20.0d", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               -1024"));

	sprintf(buffer, L"%20.d", 0);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));

	sprintf(buffer, L"%20.0i", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%20.i", -1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"               -1024"));

	sprintf(buffer, L"%20.i", 0);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));

	sprintf(buffer, L"%20.u", 1024);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                1024"));

	sprintf(buffer, L"%20.0u", 4294966272U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"          4294966272"));

	sprintf(buffer, L"%20.u", 0U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));

	sprintf(buffer, L"%20.o", 511);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                 777"));

	sprintf(buffer, L"%20.0o", 4294966785U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"         37777777001"));

	sprintf(buffer, L"%20.o", 0U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));

	sprintf(buffer, L"%20.x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234abcd"));

	sprintf(buffer, L"%50.x", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                                          1234abcd"));

	sprintf(buffer, L"%50.x%10.u", 305441741, 12345);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                                          1234abcd     12345"));

	sprintf(buffer, L"%20.0x", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            edcb5433"));

	sprintf(buffer, L"%20.x", 0U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));

	sprintf(buffer, L"%20.X", 305441741);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            1234ABCD"));

	sprintf(buffer, L"%20.0X", 3989525555U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"            EDCB5433"));

	sprintf(buffer, L"%20.X", 0U);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"                    "));
}


TEST_CASE( "wide float", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%.4f", 3.1415354);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"3.1415"));

	sprintf(buffer, L"%.3f", 30343.1415354);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"30343.142"));

	sprintf(buffer, L"%.0f", 34.1415354);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"34"));

	sprintf(buffer, L"%.2f", 42.8952);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42.90"));

	sprintf(buffer, L"%.9f", 42.8952);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42.895200000"));

	sprintf(buffer, L"%.10f", 42.895223);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42.8952230000"));

	// this testcase checks, that the precision is truncated to 9 digits.
	// a perfect working float should return the whole number
	sprintf(buffer, L"%.12f", 42.89522312345678);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42.895223123000"));

	// this testcase checks, that the precision is truncated AND rounded to 9 digits.
	// a perfect working float should return the whole number
	sprintf(buffer, L"%.12f", 42.89522387654321);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42.895223877000"));

	sprintf(buffer, L"%6.2f", 42.8952);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L" 42.90"));

	sprintf(buffer, L"%+6.2f", 42.8952);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"+42.90"));

	sprintf(buffer, L"%+5.1f", 42.9252);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"+42.9"));

	sprintf(buffer, L"%f", 42.5);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42.500000"));

	sprintf(buffer, L"%.1f", 42.5);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42.5"));

	sprintf(buffer, L"%f", 42167.0);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"42167.000000"));

	sprintf(buffer, L"%.9f", -12345.987654321);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-12345.987654321"));

	sprintf(buffer, L"%.1f", 3.999);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"4.0"));

	sprintf(buffer, L"%.0f", 3.5);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"4"));

	sprintf(buffer, L"%.0f", 3.49);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"3"));

	sprintf(buffer, L"%.1f", 3.49);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"3.5"));

	// out of range in the moment, need to be fixed by someone
	sprintf(buffer, L"%.1f", 1E20);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L""));
}


TEST_CASE( "wide types", "[]") {
	
	//wchar_t buffer[wide_test::printf_buffer_size]{};
	wchar_t * buffer = wide_test::reset_buffering();


	sprintf(buffer, L"%i", 0);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0"));

	sprintf(buffer, L"%i", 1234);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1234"));

	sprintf(buffer, L"%i", 32767);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"32767"));

	sprintf(buffer, L"%i", -32767);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-32767"));

	sprintf(buffer, L"%li", 30L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"30"));

	sprintf(buffer, L"%li", -2147483647L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-2147483647"));

	sprintf(buffer, L"%li", 2147483647L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"2147483647"));

	sprintf(buffer, L"%lli", 30LL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"30"));

	sprintf(buffer, L"%lli", -9223372036854775807LL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"-9223372036854775807"));

	sprintf(buffer, L"%lli", 9223372036854775807LL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"9223372036854775807"));

	sprintf(buffer, L"%lu", 100000L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"100000"));

	sprintf(buffer, L"%lu", 0xFFFFFFFFL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"4294967295"));

	sprintf(buffer, L"%llu", 281474976710656LLU);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"281474976710656"));

	sprintf(buffer, L"%llu", 18446744073709551615LLU);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"18446744073709551615"));

	sprintf(buffer, L"%zu", 2147483647UL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"2147483647"));

	sprintf(buffer, L"%zd", 2147483647UL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"2147483647"));

	if (sizeof(std::size_t) == sizeof(long)) {
		sprintf(buffer, L"%zi", -2147483647L);
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"-2147483647"));
	}
	else {
		sprintf(buffer, L"%zi", -2147483647LL);
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"-2147483647"));
	}

	sprintf(buffer, L"%b", 60000);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1110101001100000"));

	sprintf(buffer, L"%lb", 12345678L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"101111000110000101001110"));

	sprintf(buffer, L"%o", 60000);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"165140"));

	sprintf(buffer, L"%lo", 12345678L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"57060516"));

	sprintf(buffer, L"%lx", 0x12345678L);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"12345678"));

	sprintf(buffer, L"%llx", 0x1234567891234567LLU);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"1234567891234567"));

	sprintf(buffer, L"%lx", 0xabcdefabL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"abcdefab"));

	sprintf(buffer, L"%lX", 0xabcdefabL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"ABCDEFAB"));

	sprintf(buffer, L"%c", 'v');
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"v"));

	sprintf(buffer, L"%cv", 'w');
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"wv"));

	sprintf(buffer, L"%s", L"A Test");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"A Test"));

	sprintf(buffer, L"%hhu", 0xFFFFUL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"65535"));

	sprintf(buffer, L"%hu", 0x123456UL);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"13398"));

	sprintf(buffer, L"%s%hhi %hu", L"Test", 10000, 0xFFFFFFFF);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"Test16 65535"));

	sprintf(buffer, L"%tx", &buffer[10] - &buffer[0]);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"a"));

	// TBD
	if (sizeof(intmax_t) == sizeof(long)) {
		sprintf(buffer, L"%ji", -2147483647L);
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"-2147483647"));
	}
	else {
		sprintf(buffer, L"%ji", -2147483647LL);
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"-2147483647"));
	}

}


TEST_CASE( "wide pointer", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%p", (void*)0x1234U);
	if (sizeof(void*) == 4U) {
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"00001234"));
	}
	else {
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"0000000000001234"));
	}

	sprintf(buffer, L"%p", (void*)0x12345678U);
	if (sizeof(void*) == 4U) {
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"12345678"));
	}
	else {
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"0000000012345678"));
	}

	sprintf(buffer, L"%p-%p", (void*)0x12345678U, (void*)0x7EDCBA98U);
	if (sizeof(void*) == 4U) {
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"12345678-7EDCBA98"));
	}
	else {
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"0000000012345678-000000007EDCBA98"));
	}

	if (sizeof(uintptr_t) == sizeof(uint64_t)) {
		sprintf(buffer, L"%p", (void*)(uintptr_t)0xFFFFFFFFU);
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"00000000FFFFFFFF"));
	}
	else {
		sprintf(buffer, L"%p", (void*)(uintptr_t)0xFFFFFFFFU);
		REQUIRE(wide_test::printf_buffer_equals( buffer, L"FFFFFFFF"));
	}
}


TEST_CASE( "wide unknown flag", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%kmarco", 42, 37);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"kmarco"));
}


TEST_CASE( "wide buffer length", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};
	int ret;

	ret = snprintf(nullptr, 10, L"%s", L"Test");
	REQUIRE(ret == 4);
	ret = snprintf(nullptr, 0, L"%s", L"Test");
	REQUIRE(ret == 4);

	buffer[0] = (wchar_t)0xA5;
	ret = snprintf(buffer, 0, L"%s", L"Test");
	REQUIRE(buffer[0] == (wchar_t)0xA5);
	REQUIRE(ret == 4);

	buffer[0] = (wchar_t)0xCC;
	snprintf(buffer, 1, L"%s", L"Test");
	REQUIRE(buffer[0] == '\0');

	snprintf(buffer, 2, L"%s", L"Hello");
	REQUIRE(wide_test::printf_buffer_equals(buffer,L"H"));
}


TEST_CASE( "wide ret value", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};
	int ret;

	ret = snprintf(buffer, 6, L"0%s", L"1234");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"01234"));
	REQUIRE(ret == 5);

	ret = snprintf(buffer, 6, L"0%s", L"12345");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"01234"));
	REQUIRE(ret == 6);  // '5' is truncated

	ret = snprintf(buffer, 6, L"0%s", L"1234567");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"01234"));
	REQUIRE(ret == 8);  // '567' are truncated

	ret = snprintf(buffer, 10, L"hello, world");
	REQUIRE(ret == 12);

	ret = snprintf(buffer, 3, L"%d", 10000);
	REQUIRE(ret == 5);
	REQUIRE(wcslen(buffer) == 2U);
	REQUIRE(buffer[0] == L'1');
	REQUIRE(buffer[1] == L'0');
	REQUIRE(buffer[2] == L'\0');
}


TEST_CASE( "wide misc", "[]") {
	wchar_t buffer[wide_test::printf_buffer_size]{};

	sprintf(buffer, L"%u%u%ctest%d %s", 5, 3000, L'a', -20, L"bit");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"53000atest-20 bit"));

	sprintf(buffer, L"%.*f", 2, 0.33333333);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"0.33"));

	sprintf(buffer, L"%.3s", L"foobar");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"foo"));

	sprintf(buffer, L"%10.5d", 4);
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"     00004"));

	sprintf(buffer, L"%*sx", -3, L"hi");
	REQUIRE(wide_test::printf_buffer_equals( buffer, L"hi x"));
}


} // namespace

///////////////////////////////////////////////////////////////////////////////
// \author (c) C VERSION -- Marco Paland (info@paland.com)
//             2014-2018, PALANDesign Hannover, Germany
//         (c) CPP HEADER ONLY VERSION -- Dusan B. Jovanovic ( dbj@dbj.org )
//             2018, dbj.systems, London, UK 
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the L"Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED L"AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////
