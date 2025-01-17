#include "test_vector2.h"
#include "test_vector3.h"
#include "test_vector4.h"
#include "test_matrix.h"
#include "test_json.h"
#include "test_png.h"
#include "test_crc.h"
#include "test_scene.h"
#include "test_time_utils.h"
#include "test_file.h"
#include "test_math_utils.h"

#include "test_utils.h"

int32_t main()
{
    TESTS_INIT();

    TEST_GROUP(test_vector2);
    TEST_GROUP(test_vector3);
    TEST_GROUP(test_vector4);
    TEST_GROUP(test_matrix);
    TEST_GROUP(test_json);
    TEST_GROUP(test_png);
    TEST_GROUP(test_crc);
    TEST_GROUP(test_scene);
    TEST_GROUP(test_time_utils);
    TEST_GROUP(test_file);
    TEST_GROUP(test_math_utils);

    TESTS_SUMMARY();
    
    int32_t exit_code = TESTS_FAIL_COUNT() > 0 ? 1 : 0;
    
    return exit_code;
}