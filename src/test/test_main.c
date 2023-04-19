#include "test_vector.h"
#include "test_matrix.h"
#include "test_color.h"
#include "test_json.h"
#include "test_png.h"
#include "test_crc.h"
#include "test_scene.h"
#include "test_time.h"

int main()
{
    test_vector();
    test_matrix();
    test_color();
    test_json();
    test_png();
    test_crc();
    test_scene();
    test_time();
}