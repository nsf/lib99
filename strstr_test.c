#include "strstr.h"
#include <check.h>

//-------------------------------------------------------------------------------
// STR
//-------------------------------------------------------------------------------

#define CHECK_STR(str, _cap, _len, _data)					\
do {										\
	fail_unless((str)->cap _cap,						\
		    #_cap " capacity expected, got: %d", (str)->cap);		\
	fail_unless((str)->len _len,						\
		    #_len " length expected, got: %d", (str)->len);		\
	fail_unless(strcmp((str)->data, _data) == 0,				\
		    "\"" _data "\" string data expected, got: \"%s\"",		\
		    (str)->data);						\
} while (0)

START_TEST(test_str_new)
{
	fail_unless(sizeof(struct str) == 8,
		    "struct size is not correct for this compiler/architecture");

	struct str *str = str_new(10);
	CHECK_STR(str, == 10, == 0, "");
	str_free(str);

	// use STR_DEFAULT_CAPACITY
	str = str_new(0);
	CHECK_STR(str, == STR_DEFAULT_CAPACITY, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_str_clear)
{
	struct str *str = str_from_cstr("nsf");
	str_clear(str);
	CHECK_STR(str, >= 3, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_str_from_cstr)
{
	// simple string
	struct str *str = str_from_cstr("Hello, nsf");
	CHECK_STR(str,
		  == strlen("Hello, nsf"),
		  == strlen("Hello, nsf"),
		  "Hello, nsf");
	str_free(str);

	// empty string
	str = str_from_cstr("");
	CHECK_STR(str, == STR_DEFAULT_CAPACITY, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_str_from_cstr_len)
{
	const char *cstr = "12345";

	struct str *str = str_from_cstr_len(cstr, 5);
	CHECK_STR(str, == 5, == 5, "12345");
	str_free(str);

	str = str_from_cstr_len(cstr, 4);
	CHECK_STR(str, == 4, == 4, "1234");
	str_free(str);
}
END_TEST

START_TEST(test_str_ensure_cap)
{
	// zero capacity
	struct str *str = str_new(0);
	str_ensure_cap(&str, 10);
	CHECK_STR(str, >= 10, == 0, "");
	str_free(str);

	// non-zero length and capacity
	str = str_from_cstr("hello");
	str_ensure_cap(&str, 1);
	CHECK_STR(str, >= 6, == 5, "hello");
	str_free(str);

	// rare case, requested capacity is more than 2x bigger than the
	// previous capacity, just for the sake of coverage
	str = str_from_cstr("hello");
	str_ensure_cap(&str, 50);
	CHECK_STR(str, >= 50, == 5, "hello");
	str_free(str);
}
END_TEST

START_TEST(test_str_printf)
{
	// here I'm not checking all the printf stuff, because I use snprintf,
	// all that needs to be checked is correct cap/len calculation.
	struct str *str = str_printf("preved: %d", 31337);
	CHECK_STR(str, == 13, == 13, "preved: 31337");
	str_free(str);
}
END_TEST

START_TEST(test_str_dup)
{
	struct str *str1 = str_from_cstr("hello123");
	struct str *str2 = str_dup(str1);
	CHECK_STR(str2, == 8, == 8, "hello123");
	str_free(str1);
	str_free(str2);
}
END_TEST

START_TEST(test_str_from_file)
{
	struct str *str = str_from_file("testdata/file.txt");
	fail_unless(str != 0, "testdata/file.txt should be loaded successfully");
	CHECK_STR(str, == 10, == 10, "123456789\n");
	str_free(str);

	str = str_from_file("non-existent file");
	fail_unless(str == 0, "zero value expected");
}
END_TEST

START_TEST(test_str_add_str)
{
	struct str *str1 = str_from_cstr("123");
	struct str *str2 = str_from_cstr("456");
	str_add_str(&str1, str2);
	CHECK_STR(str1, >= 6, == 6, "123456");
	str_free(str1);
	str_free(str2);

	str1 = str_new(0);
	str2 = str_from_cstr("preved");
	str_add_str(&str1, str2);
	CHECK_STR(str1, >= 6, == 6, "preved");
	str_free(str2);

	str2 = str_from_cstr("medved");
	str_add_str(&str1, str2);
	CHECK_STR(str1, >= 12, == 12, "prevedmedved");
	str_free(str2);

	str2 = str_from_cstr("");
	str_add_str(&str1, str2);
	CHECK_STR(str1, >= 12, == 12, "prevedmedved");
	str_free(str2);

	str_free(str1);
}
END_TEST

START_TEST(test_str_add_cstr)
{
	struct str *str = str_from_cstr("123");
	str_add_cstr(&str, "456");
	CHECK_STR(str, >= 6, == 6, "123456");
	str_free(str);

	str = str_new(0);
	str_add_cstr(&str, "preved");
	CHECK_STR(str, >= 6, == 6, "preved");

	str_add_cstr(&str, "medved");
	CHECK_STR(str, >= 12, == 12, "prevedmedved");

	str_add_cstr(&str, "");
	CHECK_STR(str, >= 12, == 12, "prevedmedved");

	str_free(str);
}
END_TEST

START_TEST(test_str_add_printf)
{
	struct str *str = str_from_cstr("123");
	str_add_printf(&str, "%d", 456);
	CHECK_STR(str, >= 6, == 6, "123456");
	str_free(str);

	str = str_new(0);
	str_add_printf(&str, "%d", 31337);
	str_add_printf(&str, "%s", "31337");
	CHECK_STR(str, >= 10, == 10, "3133731337");
	str_free(str);
}
END_TEST

START_TEST(test_str_add_file)
{
	struct str *str = str_from_cstr("abc");
	str_add_file(&str, "testdata/file.txt");
	CHECK_STR(str, >= 13, == 13, "abc123456789\n");
	str_free(str);

	str = str_new(0);
	str_add_file(&str, "testdata/file.txt");
	CHECK_STR(str, >= 10, == 10, "123456789\n");
	str_free(str);

	str = str_new(0);
	str_add_file(&str, "non-existent file");
	CHECK_STR(str, == STR_DEFAULT_CAPACITY, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_str_rtrim)
{
	struct str *str = str_from_cstr("123\r\n  ");
	str_rtrim(str);
	CHECK_STR(str, >= 3, == 3, "123");
	str_free(str);

	str = str_new(0);
	str_rtrim(str);
	CHECK_STR(str, == STR_DEFAULT_CAPACITY, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_str_ltrim)
{
	struct str *str = str_from_cstr(" 	\n\r123");
	str_ltrim(str);
	CHECK_STR(str, >= 3, == 3, "123");
	str_free(str);

	str = str_new(0);
	str_ltrim(str);
	CHECK_STR(str, == STR_DEFAULT_CAPACITY, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_str_trim)
{
	struct str *str = str_from_cstr(" 	\n\r123  \n");
	str_trim(str);
	CHECK_STR(str, >= 3, == 3, "123");
	str_free(str);

	str = str_new(0);
	str_trim(str);
	CHECK_STR(str, == STR_DEFAULT_CAPACITY, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_str_split_path)
{
	struct str *str = str_from_cstr("test/path");
	struct str *half1, *half2;
	half1 = str_split_path(str, &half2);
	CHECK_STR(half1, >= 4, == 4, "test");
	CHECK_STR(half2, >= 4, == 4, "path");
	str_free(str);
	str_free(half1);
	str_free(half2);

	str = str_from_cstr("test/my/path");
	half1 = str_split_path(str, &half2);
	CHECK_STR(half1, >= 7, == 7, "test/my");
	CHECK_STR(half2, >= 4, == 4, "path");
	str_free(str);
	str_free(half1);
	str_free(half2);

	str = str_from_cstr("testpath");
	half1 = str_split_path(str, &half2);
	fail_unless(half1 == 0, "zero value expected");
	CHECK_STR(half2, >= 8, == 8, "testpath");
	str_free(str);
	str_free(half1);
	str_free(half2);
}
END_TEST

//-------------------------------------------------------------------------------
// FSTR
//-------------------------------------------------------------------------------

START_TEST(test_fstr_init)
{
	char buf[40];
	struct fstr fstr;

	FSTR_INIT_FOR_BUF(&fstr, buf);
	CHECK_STR(&fstr, == 39, == 0, "");

	fstr_init(&fstr, buf, 0, sizeof(buf)-1);
	CHECK_STR(&fstr, == 39, == 0, "");
}
END_TEST

START_TEST(test_fstr_add_str)
{
	char buf[11];
	struct fstr fstr;

	FSTR_INIT_FOR_BUF(&fstr, buf);
	struct str *str = str_from_cstr("hello");

	fstr_add_str(&fstr, str);
	CHECK_STR(&fstr, == 10, == 5, "hello");
	str_free(str);

	str = str_from_cstr("world!");
	fstr_add_str(&fstr, str);
	CHECK_STR(&fstr, == 10, == 10, "helloworld");
	str_free(str);

	FSTR_INIT_FOR_BUF(&fstr, buf);
	str = str_from_cstr("");
	fstr_add_str(&fstr, str);
	CHECK_STR(&fstr, == 10, == 0, "");
	str_free(str);
}
END_TEST

START_TEST(test_fstr_add_cstr)
{
	char buf[11];
	struct fstr fstr;

	FSTR_INIT_FOR_BUF(&fstr, buf);

	fstr_add_cstr(&fstr, "prevedmedved");
	CHECK_STR(&fstr, == 10, == 10, "prevedmedv");
}
END_TEST

START_TEST(test_fstr_add_printf)
{
	char buf[11];
	struct fstr fstr;

	FSTR_INIT_FOR_BUF(&fstr, buf);

	fstr_add_printf(&fstr, "hello: %s!", "nsf");
	CHECK_STR(&fstr, == 10, == 10, "hello: nsf");

	FSTR_INIT_FOR_BUF(&fstr, buf);
	fstr_add_printf(&fstr, "hello");
	CHECK_STR(&fstr, == 10, == 5, "hello");
	fstr_add_printf(&fstr, "world!!!!");
	CHECK_STR(&fstr, == 10, == 10, "helloworld");
}
END_TEST

Suite *strstr_suite()
{
	Suite *s = suite_create("strstr");

	TCase *tc_str = tcase_create("str");
	tcase_add_test(tc_str, test_str_new);
	tcase_add_test(tc_str, test_str_clear);
	tcase_add_test(tc_str, test_str_from_cstr);
	tcase_add_test(tc_str, test_str_from_cstr_len);
	tcase_add_test(tc_str, test_str_ensure_cap);
	tcase_add_test(tc_str, test_str_printf);
	tcase_add_test(tc_str, test_str_dup);
	tcase_add_test(tc_str, test_str_from_file);

	tcase_add_test(tc_str, test_str_add_str);
	tcase_add_test(tc_str, test_str_add_cstr);
	tcase_add_test(tc_str, test_str_add_printf);
	tcase_add_test(tc_str, test_str_add_file);

	tcase_add_test(tc_str, test_str_trim);
	tcase_add_test(tc_str, test_str_ltrim);
	tcase_add_test(tc_str, test_str_rtrim);
	tcase_add_test(tc_str, test_str_split_path);

	TCase *tc_fstr = tcase_create("fstr");
	tcase_add_test(tc_fstr, test_fstr_init);
	tcase_add_test(tc_fstr, test_fstr_add_str);
	tcase_add_test(tc_fstr, test_fstr_add_cstr);
	tcase_add_test(tc_fstr, test_fstr_add_printf);

	suite_add_tcase(s, tc_str);
	suite_add_tcase(s, tc_fstr);
	return s;
}
