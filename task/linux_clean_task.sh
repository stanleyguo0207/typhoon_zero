if [ -d `pwd`/build/ ];then
	rm `pwd`/build/ -r
fi

if [ -L `pwd`/compile_commands.json ];then
	rm `pwd`/compile_commands.json
fi
