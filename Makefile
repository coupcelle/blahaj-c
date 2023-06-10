build:
	gcc ./src/main.c -o target/linux-blahaj -Wall -g -Ilib/toml/include -Llib/toml/lib -ltoml
	i686-w64-mingw32-gcc ./src/main.c -o target/windows-blahaj.exe -Wall -g -Ilib/toml-win/include -Llib/toml-win/lib -ltoml
	i686-w64-mingw32-gcc ./win2k/color-test.c -o target/wold-blahaj.exe -Wall -g -Ilib/toml-win/include -Llib/toml-win/lib -ltoml


clean:
	rm -f target/linux-blahaj
	rm -f target/windows-blahaj.exe
