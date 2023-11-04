<?php

$ffiEcho = FFI::cdef(
    "void echo_ffi();",
    __DIR__ . "/ffi/echo.so"
);

$ffiEcho->echo_ffi();
echo PHP_EOL;

$ffiCalc = FFI::cdef(
    "int factorial(int n);",
    __DIR__ . "/ffi/calc.so"
);
echo $ffiCalc->factorial(5) . PHP_EOL;
