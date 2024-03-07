# phiano

PHPerKaigi 2024発表用のリポジトリです。

This project is a simple synthesizer implemented in PHP and C. It reads musical notes from a CSV file and plays them using the SDL library.

## Structure

The project is structured as follows:

- `index.php`: The main PHP script that reads notes from a CSV file and plays them using the synthesizer.
- `ffi/synthesizer.c`: The C code for the synthesizer. It uses the SDL library to generate and play the audio.
- `songs/`: A directory containing example songs in CSV format.

## Environment
- PHP 8.1 on Mac OS
- SDL2

## Usage

```sh
zsh ./run.sh tulip
```