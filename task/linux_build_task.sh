if [ ! -d `pwd`/build/ ];then
	mkdir `pwd`/build && cd `pwd`/build && cmake .. && ln -s compile_commands.json ..
else
	cd `pwd` && cmake --build build
fi
