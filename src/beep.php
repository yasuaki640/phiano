<?php

/** @var array<string, float> */
$equalTemperament = [
    "C" => 261.626,
    "C#" => 277.183,
    "Db" => 277.183,
    "D" => 293.665,
    "D#" => 311.127,
    "Eb" => 311.127,
    "E" => 329.628,
    "F" => 349.228,
    "F#" => 369.994,
    "Gb" => 369.994,
    "G" => 391.995,
    "G#" => 415.305,
    "Ab" => 415.305,
    "A" => 440.000,
    "A#" => 466.164,
    "Bb" => 466.164,
    "B" => 493.883,
];

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

/** @var array{0: string, 1: float}[] $notes */
$notes = [];
fgetcsv($handle); // skip header
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
$cNote->frequency = $equalTemperament[$notes[1][0]];
$cNote->duration = 2000;

$ffi->beep($cNote);

