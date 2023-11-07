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

// open csv file
$handle = fopen($path, "r");
if ($handle === false) {
    echo "Could not open file $path" . PHP_EOL;
    exit(1);
}

// read csv file
$songs = [];
while (($note = fgetcsv($handle)) !== false) {
    $songs[] = $note;
}

// close csv file
fclose($handle);

var_dump($songs);