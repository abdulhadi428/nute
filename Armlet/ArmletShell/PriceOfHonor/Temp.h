#pragma once

#define COMPILE_TIME_CHECK(e) extern char _COMPILE_TIME_CHECK_[(e)?1:-1]