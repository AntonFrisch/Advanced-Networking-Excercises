#!/usr/bin/env python3
import sys
import requests

URL = "https://pypi.org/pypi/sampleproject/json"

def main() -> int:
    try:
        resp = requests.get(URL, timeout=10)
        resp.raise_for_status()
    except requests.RequestException as e:
        print(f"HTTP request failed: {e}", file=sys.stderr)
        return 1

    try:
        data = resp.json()  # Convert response body to JSON (Python dict/list structure)
    except ValueError as e:
        print(f"Invalid JSON response: {e}", file=sys.stderr)
        return 1

    # The filenames are in: data["releases"][<version>] -> list of file dicts -> ["filename"]
    releases = data.get("releases", {})
    for version in releases:
        files = releases[version]
        for file_entry in files:
            if "filename" in file_entry:
                print(file_entry["filename"])

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
