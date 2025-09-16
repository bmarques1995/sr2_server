import subprocess
import sys
from package_helpers import clone_and_checkout, append_paths, run
import platform

def main():
    build_mode = sys.argv[1]
    install_prefix = sys.argv[2]
    module_destination = sys.argv[3]

    os_name = platform.system().lower()

    package_output_dir = "flatbuffers"
    commit_hash_release = "1c514626e83c20fffa8557e75641848e1e15cd5e"
    flatbuffers_dir = append_paths(module_destination, "modules", package_output_dir)
    build_dir = append_paths(module_destination, "dependencies", os_name, package_output_dir)
    flatbuffers_patch = append_paths(module_destination, "patches", "flatbuffers.patch")

    clone_and_checkout("https://github.com/google/flatbuffers.git", destination=flatbuffers_dir, commit_hash=commit_hash_release, patch=flatbuffers_patch)

    if os_name == "windows":
        run(f'cmake -S "{flatbuffers_dir}" -B "{build_dir}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DFLATBUFFERS_BUILD_SHAREDLIB=ON')
        run(f'cmake --build "{build_dir}" --config {build_mode} --target install')
    elif os_name in ["linux", "darwin", "freebsd"]:
        run(f'cmake -S "{flatbuffers_dir}" -B "{build_dir}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DCMAKE_BUILD_TYPE="{build_mode}" '
            f'-DFLATBUFFERS_BUILD_SHAREDLIB=ON')
        run(f'cmake --build "{build_dir}" --target install')

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"Error while running command: {e}", file=sys.stderr)
        sys.exit(1)
