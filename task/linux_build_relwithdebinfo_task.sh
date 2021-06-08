script_dir=$(cd $(dirname $0);pwd)
project_path=$(dirname $script_dir)
build_path=$project_path/build/relwithdebinfo/

if [ ! -d $build_path ];then
	if [ -L $project_path/compile_commands.json ];then
		rm $project_path/compile_commands.json
	fi
	mkdir -p $build_path\
		&& cd $build_path\
		&& cmake $project_path\
			-DCMAKE_BUILD_TYPE=RelWithDebInfo\
		&& ln -s compile_commands.json $project_path
else
	cd $project_path\
		&& cmake --build $build_path
fi
