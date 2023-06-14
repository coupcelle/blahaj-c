build:
	gcc ./src/main.c ./src/color.c -o target/linux-blahaj -Wall -g -Ilib/toml/include -Llib/toml/lib -ltoml
	i686-w64-mingw32-gcc ./src/main.c ./src/color.c -o target/windows-blahaj.exe -Wall -g -Ilib/toml-win/include -Llib/toml-win/lib -ltoml

clean:
	rm -f target/linux-blahaj
	rm -f target/windows-blahaj.exe
