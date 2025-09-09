import subprocess
import sys
from package_helpers import clone_and_checkout, append_paths, run
import platform

def main():
    build_mode = sys.argv[1]
    install_prefix = sys.argv[2]
    module_destination = sys.argv[3]

    os_name = platform.system().lower()

    package_output_dir = "tabulate"
    commit_hash_release = "b35db4cce50a4b296290b0ae827305cdeb23751e"
    tabulate_dir = append_paths(module_destination, "modules", package_output_dir)
    build_dir = append_paths(module_destination, "dependencies", os_name, package_output_dir)
    tabulate_cmake_dir = tabulate_dir

    clone_and_checkout("https://github.com/p-ranav/tabulate.git", destination=tabulate_dir, commit_hash=commit_hash_release)

    if os_name == "windows":
        run(f'cmake -S "{tabulate_cmake_dir}" -B "{build_dir}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DCMAKE_PREFIX_PATH="{install_prefix}" '
            f'-DSAMPLES=OFF '
            f'-Dtabulate_BUILD_TESTS=OFF '
            f'-DBUILD_SHARED_LIBS=ON')
        run(f'cmake --build "{build_dir}" --config {build_mode} --target install')
    elif os_name in ["linux", "darwin", "freebsd"]:
        run(f'cmake -S "{tabulate_cmake_dir}" -B "{build_dir}" '
            f'-DCMAKE_BUILD_TYPE="{build_mode}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DCMAKE_PREFIX_PATH="{install_prefix}" '
            f'-DSAMPLES=OFF '
            f'-Dtabulate_BUILD_TESTS=OFF '
            f'-DBUILD_SHARED_LIBS=ON')
        run(f'cmake --build "{build_dir}" --target install')

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"Error while running command: {e}", file=sys.stderr)
        sys.exit(1)