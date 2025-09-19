import subprocess
import sys
from package_helpers import append_msvc_compiler_location, clone_and_checkout, run, append_paths, append_vs_ninja_host, append_prefix_to_path
import platform
import os

def main():
    build_mode = sys.argv[1]
    install_prefix = sys.argv[2]
    module_destination = sys.argv[3]
    
    os_name = platform.system().lower()

    final_build_mode = build_mode.lower()
    package_output_dir = "postgresql"
    commit_hash_release = "7885b94dd81b98bbab9ed878680d156df7bf857f"
    postgredir_dir = append_paths(module_destination, "modules", package_output_dir)
    postgredir_build_dir = append_paths(postgredir_dir, "build")

    if os_name == "windows":
        vs_compiler = sys.argv[4]
        append_msvc_compiler_location(vs_compiler)
        append_vs_ninja_host(vs_compiler)

    append_prefix_to_path(install_prefix)
    clone_and_checkout(repo="https://git.postgresql.org/git/postgresql.git", destination=postgredir_dir, commit_hash=commit_hash_release)
    if os_name == "windows":
        run(f'meson setup build --prefix="{install_prefix}" --buildtype="{final_build_mode}" --vsenv', cwd=postgredir_dir)
        run(f'ninja', cwd=postgredir_build_dir)
        run(f'ninja install', cwd=postgredir_build_dir)
        run(f'git clean -dfx', cwd=postgredir_dir)
    elif os_name in ["linux", "darwin", "freebsd"]:
        run(f'meson setup build --prefix="{install_prefix}" --buildtype="{final_build_mode}"', cwd=postgredir_dir)
        run(f'ninja', cwd=postgredir_build_dir)
        run(f'ninja install', cwd=postgredir_build_dir)
        run(f'git clean -dfx', cwd=postgredir_dir)

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"Build or install failed: {e}", file=sys.stderr)
        sys.exit(1)