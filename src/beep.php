<?php

if (count($argv) < 2) {
    echo "The path to CSV file is required" . PHP_EOL;
    exit(1);
}

$path = $argv[1];
if (!file_exists($path)) {
    echo "File $path does not exist" . PHP_EOL;
    exit(1);
}

$handle = fopen($path, "r");
if ($handle === false) {
    echo "Could not open file $path" . PHP_EOL;
    exit(1);
}

/** @var array{0: int, 1: string}[] $notes */
$notes = [];
while (($n = fgetcsv($handle)) !== false) {
    $notes[] = $n;
}
fclose($handle);

$ffi = FFI::cdef("
    typedef struct {
        double frequency;
        int duration;
    } Note;

    void beep(Note note);
", __DIR__ . "/ffi/beep.so");

$cNote = $ffi->new("Note");
$cNote->frequency = 443.0;
$cNote->duration = 2000;

$ffi->beep($cNote);

