import os
import sys
import shutil
import subprocess

root_path = os.path.dirname(os.path.dirname(__file__))
compile_commands_file = "{}/{}".format(root_path, "compile_commands.json")


def GenerateTask(argv: list[str]):
    global root_path
    global compile_commands_file

    gen_type = argv[0]
    if "RelWithDebInfo" == gen_type:
        pass
    elif "Release" == gen_type:
        pass
    else:
        gen_type = "Debug"

    gen_path = "{}/build/{}/".format(root_path, gen_type)

    if not os.path.exists(gen_path):
        os.makedirs(gen_path)

    gen_cmd = "cmake -S {} -B {} -DCMAKE_BUILD_TYPE={} {custom_options}".format(
        root_path, gen_path, gen_type, custom_options=" ".join(argv[1:]))
    print("TASK: generate cmd: {}".format(gen_cmd))
    subprocess.run(gen_cmd, shell=True)

    if os.path.islink(compile_commands_file):
        os.remove(compile_commands_file)

    os.symlink("{}/compile_commands.json".format(gen_path),
               compile_commands_file)


def BuildTask(argv: list[str]):
    global root_path
    global compile_commands_file

    build_type = argv[0]
    if "RelWithDebInfo" == build_type:
        pass
    elif "Release" == build_type:
        pass
    else:
        build_type = "Debug"

    build_path = "{}/build/{}/".format(root_path, build_type)

    build_cmd = "cmake --build {} {custom_options}".format(
        build_path, custom_options=" ".join(argv[1:]))
    print("TASK: build cmd: {}".format(build_cmd))
    subprocess.run(build_cmd, shell=True)


def InstallTask(argv: list[str]):
    global root_path
    global compile_commands_file

    install_type = argv[0]
    if "RelWithDebInfo" == install_type:
        pass
    elif "Release" == install_type:
        pass
    else:
        install_type = "Debug"

    build_path = "{}/build/{}/".format(root_path, install_type)

    install_cmd = "cmake --install {} {custom_options}".format(
        build_path, custom_options=" ".join(argv[1:]))
    print("TASK: install cmd: {}".format(install_cmd))
    subprocess.run(install_cmd, shell=True)


def CleanTask(argv: list[str]):
    global root_path
    global compile_commands_file

    build_type = argv[0]
    if "RelWithDebInfo" == build_type:
        pass
    elif "Release" == build_type:
        pass
    else:
        build_type = "Debug"

    build_path = "{}/build/{}/".format(root_path, build_type)
    install_path = "{}/install/{}/".format(root_path, build_type)

    if os.path.exists(build_path):
        shutil.rmtree(build_path)

    if os.path.islink(compile_commands_file):
        os.remove(compile_commands_file)

    if os.path.exists(install_path):
        shutil.rmtree(install_path)


def ProtoTask(argv: list[str]):
    global root_path
    global compile_commands_file

    # Build   ???proto???????????????j
    proto_path = "{}/{}".format(root_path, argv[0])
    # Move    ???install????????????
    install_path = "{}/{}".format(root_path, argv[1])

    for root, dirs, files in os.walk(proto_path):
        for file in files:
            if file.endswith(".proto"):
                if "descriptor.proto" in file:
                    continue
                filename = os.path.join(root, file)
                build_cmd = "protoc --cpp_out={custom_options}:{} --proto_path={} {}".format(
                    install_path, proto_path, filename, custom_options=" ".join(argv[2:]))
                print("PROTO: Build cmd: {}".format(build_cmd))
                subprocess.run(build_cmd, shell=True)


def OpTask(op_code: str, argv: list[str]):
    if "Generate" == op_code:
        GenerateTask(argv)
    elif "Build" == op_code:
        BuildTask(argv)
    elif "Install" == op_code:
        InstallTask(argv)
    elif "Clean" == op_code:
        CleanTask(argv)
    elif "Proto" == op_code:
        ProtoTask(argv)
    else:
        print("Unkown op_code: {} argv: {}".format(op_code, argv))
        exit(1)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("{} error argv len : {}".format(__file__, len(sys.argv)))

    op_code = sys.argv[1]
    OpTask(op_code, sys.argv[2:])
