<?php

class Note
{
    public function __construct(
        public readonly float $frequency,
        public readonly int   $duration // ms
    )
    {
    }
}

/**
 * @param string $path
 * @return void
 */
function validateFile(string $path): void
{
    if (!file_exists($path)) {
        echo "File $path does not exist" . PHP_EOL;
        exit(1);
    }

    $handle = fopen($path, "r");
    if ($handle === false) {
        echo "Could not open file $path" . PHP_EOL;
        exit(1);
    }

    fclose($handle);
}

/**
 * @param string $path
 * @param array<string, float> $equalTemperament
 * @return array<Note>
 */
function parseCsv(string $path, array $equalTemperament): array
{
    $handle = fopen($path, "r");
    fgetcsv($handle); // skip header

    $notes = [];
    while (($row = fgetcsv($handle)) !== false) {
        $frequency = $equalTemperament[$row[0]];
        $duration = floatval($row[1]);
        $notes[] = new Note(
            $frequency,
            $duration
        );
    }
    fclose($handle);

    return $notes;
}

/**
 * @param array<Note> $notes
 */
function playNotes(array $notes): void
{
    $ffi = FFI::cdef("
        typedef struct {
            double frequency;
            int duration;
        } Note;

        void synthesizer(Note notes[], int num_notes);
    ", __DIR__ . "/ffi/libsynthesizer.so");

    $cNotes = $ffi->new("Note[" . count($notes) . "]");
    foreach ($notes as $i => $note) {
        $cNote = $cNotes[$i];
        $cNote->frequency = $note->frequency;
        $cNote->duration = $note->duration;
    }

    $ffi->synthesizer($cNotes, count($notes));
}

/**
 * @param array $argv
 * @param array<string, float> $equalTemperament
 */
function main(array $argv, array $equalTemperament): void
{
    if (count($argv) < 2) {
        echo "The path to CSV file is required" . PHP_EOL;
        exit(1);
    }

    $path = $argv[1];
    validateFile($path);

    $notes = parseCsv($path, $equalTemperament);
    playNotes($notes);
}

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
    "R" => 0.0
];

main($argv, $equalTemperament);
