<?php

$ffi = FFI::cdef(
    "void echo_ffi();",
    __DIR__ . "/ffi/echo.so"
);

$ffi->echo_ffi();
