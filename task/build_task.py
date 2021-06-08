import os
import sys
import subprocess


def BuildTask(argv: list[str]):
    build_type = argv[0]
    if "RelWithDebInfo" == build_type:
        pass
    elif "Release" == build_type:
        pass
    else:
        build_type = "Debug"

    root_path = os.path.dirname(os.path.dirname(__file__))
    build_path = "{}/build/{}/".format(root_path, build_type)

    if not os.path.exists(build_path):
        os.makedirs(build_path)

    build_cmd = "cmake -S {} -B {} -DCMAKE_BUILD_TYPE={} {custom_options}".format(
        root_path, build_path, build_type, custom_options=" ".join(argv[1:]))
    subprocess.run(build_cmd, shell=True)

    compile_commands_file = "{}/{}".format(root_path, "compile_commands.json")

    if os.path.exists(compile_commands_file):
        os.remove(compile_commands_file)

    os.symlink("{}/compile_commands.json".format(build_path),
               compile_commands_file)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("{} error argv len : {}".format(__file__, len(sys.argv)))
    BuildTask(sys.argv[1:])
