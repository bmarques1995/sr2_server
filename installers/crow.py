import subprocess
import sys
from package_helpers import clone_and_checkout, append_paths, run
import platform

def main():
    build_mode = sys.argv[1]
    install_prefix = sys.argv[2]
    module_destination = sys.argv[3]

    os_name = platform.system().lower()

    package_output_dir = "crow"
    commit_hash_release = "2f224d4622381ef02d47fc03d1fcbe0190f079d5"
    crow_dir = append_paths(module_destination, "modules", package_output_dir)
    build_dir = append_paths(module_destination, "dependencies", os_name, package_output_dir)
    crow_cmake_dir = crow_dir
    crow_patch = append_paths(module_destination, "patches", "crow.patch")

    clone_and_checkout("https://github.com/CrowCpp/Crow.git", destination=crow_dir, commit_hash=commit_hash_release, patch=crow_patch)

    if os_name == "windows":
        run(f'cmake -S "{crow_cmake_dir}" -B "{build_dir}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DCROW_USE_BOOST=ON '
            f'-DCROW_BUILD_EXAMPLES=OFF '
            f'-DCROW_BUILD_TESTS=OFF '
            f'-DBUILD_SHARED_LIBS=ON')
        run(f'cmake --build "{build_dir}" --config {build_mode} --target install')
    else:
        run(f'cmake -S "{crow_cmake_dir}" -B "{build_dir}" '
            f'-DCMAKE_BUILD_TYPE="{build_mode}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DCROW_USE_BOOST=ON '
            f'-DCROW_BUILD_EXAMPLES=OFF '
            f'-DCROW_BUILD_TESTS=OFF '
            f'-DBUILD_SHARED_LIBS=ON')
        run(f'cmake --build "{build_dir}" --target install')

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"Error while running command: {e}", file=sys.stderr)
        sys.exit(1)
