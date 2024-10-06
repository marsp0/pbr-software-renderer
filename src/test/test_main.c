#include "test_vector2.h"
#include "test_vector3.h"
#include "test_vector4.h"
#include "test_matrix.h"
#include "test_json.h"
#include "test_png.h"
#include "test_crc.h"
#include "test_scene.h"
#include "test_camera.h"
#include "test_rasterizer.h"
#include "test_clip.h"

#include "test_utils.h"

int main()
{
    INIT_TESTS();

    TEST_GROUP(test_vector2);
    TEST_GROUP(test_vector3);
    TEST_GROUP(test_vector4);
    TEST_GROUP(test_matrix);
    TEST_GROUP(test_json);
    TEST_GROUP(test_png);
    TEST_GROUP(test_crc);
    TEST_GROUP(test_camera);
    TEST_GROUP(test_rasterizer);
    TEST_GROUP(test_clip);
    TEST_GROUP(test_scene);

    TESTS_SUMMARY();
}