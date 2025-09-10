import subprocess
import sys
from qtsuper import build_qt_package
import platform

def main():
    print("Entered script")
    build_mode = sys.argv[1]
    install_prefix = sys.argv[2]
    module_destination = sys.argv[3]
    os_name = platform.system().lower()
    if os_name == "windows" and len(sys.argv) > 4:
        vs_compiler = sys.argv[4]
    else:
        vs_compiler = ""

    build_qt_package("qtbase", "6.9.1", build_mode, install_prefix, module_destination, vs_compiler)

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"Build or install failed: {e}", file=sys.stderr)
        sys.exit(1)