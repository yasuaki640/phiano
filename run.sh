cd "$(dirname "$0")"

if [ -z "$1" ]; then
    echo "Please provide a song name"
    exit 1
fi

cd ./src/ffi
gcc -shared -o libsynthesizer.so synthesizer.c $(sdl2-config --cflags --libs)

cd ../
php index.php "./songs/$1.csv"
