#include "test_file.h"

#include "test_utils.h"
#include "../file.h"

static void test_file_when_file_exists()
{
    file_t* file = file_new("./assets/WaterBottle.glb");

    ASSERT_EQUAL(file->cursor, 0);
    ASSERT_EQUAL(file->size, 8961340);

    file_free(file);
}

void test_file()
{
    TEST_CASE(test_file_when_file_exists);
}