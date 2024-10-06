#include "test_time_utils.h"

 #include <stdio.h>

 #include "test_utils.h"
 #include "../time_utils.h"

 static void test_time_now_when_initialized()
 {
     time_init();
     timestamp_t now = time_now();

     ASSERT_TRUE(2 * SECOND > now);
 }

 static void test_time_now_when_not_initialized()
 {
     timestamp_t now = time_now();

     // 1692351781571436787 - nanoseconds to 18.08.2023
     ASSERT_TRUE(1692351781571436787 < now);
 }

 static void test_time_multiple_initializations()
 {
     time_init();
     timestamp_t t1 = time_now();
     time_init();
     timestamp_t t2 = time_now();

     ASSERT_TRUE(t2 > t1);
 }

 void test_time_utils()
 {
     TEST_CASE(test_time_now_when_not_initialized);
     TEST_CASE(test_time_now_when_initialized);
     TEST_CASE(test_time_multiple_initializations);
 }