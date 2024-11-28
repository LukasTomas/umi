#!/usr/bin/env python3

import os
import argparse

from url_bfs import BFSator


def parse_args():
    parser = argparse.ArgumentParser(
        prog=os.path.basename(__file__),
        description="Finds path between two URLs",
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument("first_url", help="First URL (example: https://www.example.com)")
    parser.add_argument("second_url", help="Second URL (example: https://www.example2.com)")

    args = parser.parse_args()
    return args.first_url, args.second_url


if __name__ == '__main__':
    first_url, second_url = parse_args()
    print(f"Find path: {first_url} - {second_url}")

    bfsator = BFSator(first_url, second_url)
    path = bfsator.find_path_bfs()
    if path:
        print(f"Path found: {path}")
        exit(0)
    else:
        print("Did not find a path")
        exit(1)
