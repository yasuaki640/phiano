cd `dirname $0`

if [ -z "$1" ]; then
    echo "Pass a song name"
    exit 1
fi

gcc -shared -o ./ffi/beep.so ./ffi/beep.c `sdl2-config --cflags --libs`
php beep.php ./songs/"$1".csv
