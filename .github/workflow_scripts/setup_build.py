import os
import pathlib
import sys, requests
import tarfile

# Patch releases
UT_LINUX = 'https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-Linux-amd64.tar.bz2'
UT_WINDOWS = 'https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-Windows.zip'
UT_SDK = 'https://github.com/OldUnreal/UnrealTournamentPatches/releases/download/v469d/OldUnreal-UTPatch469d-SDK.tar.bz2'


def setup_linux():
    # Get the libs/OldUnrealSDK path from our current file
    current_path = pathlib.Path(os.path.realpath(f'{__file__}/../../../libs/OldUnrealSDK/'))
    current_path.mkdir(exist_ok=True, parents=True)

    for archive_name, url in (('sdk.tar.bz2', UT_SDK), ('linux.tar.bz2', UT_LINUX)):
        # Download the latest url
        response = requests.get(url)
        response.raise_for_status()

        print(f"Downloading {url} as {archive_name}")
        with open(archive_name, 'wb') as fh:
            fh.write(response.content)

        print(f"Extracting to {current_path}")
        with tarfile.open(archive_name, "r:bz2") as tar:
            tar.extractall(current_path)


def setup_windows():
    # Do the same as linux and also SDL2!
    print("Not implemented yet!")


def main():
    if len(sys.argv) <= 1:
        print("Error: Requires 1 parameter of OS")
        sys.exit(1)

    operating_system = sys.argv[1]

    if operating_system == 'linux':
        setup_linux()
    elif operating_system == 'windows':
        setup_windows()
    else:
        print("Unknown OS", operating_system)


if __name__ == "__main__":
    main()
