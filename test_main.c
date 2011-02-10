#include <stdlib.h>
#include "test_suites.h"

int main(int argc, char **argv)
{
	int failed_n;
	SRunner *sr = srunner_create(strstr_suite());
	srunner_add_suite(sr, linear_math_suite());

	srunner_run_all(sr, CK_NORMAL);
	failed_n = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (failed_n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
