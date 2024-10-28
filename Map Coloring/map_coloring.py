#!/usr/bin/env python3

import matplotlib.pyplot as plt
import geopandas as gpd
import pandas as pd
import numpy as np
import argparse
import os

from solver import Solver


ALGORITHMS = ['BT', 'BJ', 'MAC-BT']
ALGORITHMS_DESCRIPTION = {
    'BT': 'Backtracking algoritmus vždy ohodnotí jeden vrchol a rekurzivně pokračuje do dalšího vrcholu. Pokud narazí na vrchol, který nelze ohodnotit, vrátí se zpět a pokusí se ohodnotit jiný vrchol. Tento proces se opakuje, dokud není ohodnocen poslední vrchol.',
    'BJ': 'Backjumping algoritmus je modifikací backtracking algoritmu, který sleduje konflikty a pokouší se vyhnout zbytečným opakováním. Pokud narazí na konflikt, vrátí se zpět na nejhlubší konfliktující vrchol a pokusí se ohodnotit jiný vrchol.',
    'MAC-BT': 'MAC-BT algoritmus je modifikací backtracking algoritmu, který využívá metodu zpětného šíření (MAC) k omezení domény proměnných. Tato metoda zajišťuje, že proměnné jsou ohodnoceny tak, aby nedocházelo k žádným konfliktům.'
}

COLORS_NUMBER = 4

MAPS_DIR = 'maps'
COUNTRY = 'AUS'
LEVEL = '1'


def parse_args():
    parser = argparse.ArgumentParser(
        prog=os.path.basename(__file__),
        description='Map Coloring',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )

    parser.add_argument('-a', '--algorithm', choices=ALGORITHMS, default='BT',)

    args = parser.parse_args()
    return args

def create_neighbours_file(map_data, neighbours_path):
    neighbours = {}
    for index, district in map_data.iterrows():
        print(f'Processing district {index}...')
        neighbours[index] = map_data[map_data.geometry.touches(district.geometry)].index.tolist()
    df = pd.DataFrame(neighbours.items(), columns=['district', 'neighbours'])
    df.to_csv(neighbours_path, index=False)

def convert_to_int_array(x):
    x = x[1:-1]
    if x == '':
        return []

    split = x.split(', ')
    int_array = list(map(int, split))
    return int_array


if __name__ == '__main__':
    args = parse_args()
    print(f'Algorithm: {args.algorithm}')
    print('-' * 50)
    print(ALGORITHMS_DESCRIPTION[args.algorithm])
    print('-' * 50)


    map_path = os.path.join(MAPS_DIR, f'{COUNTRY}_{LEVEL}.shp')
    neighbours_path = os.path.join(MAPS_DIR, f'{COUNTRY}_{LEVEL}_neighbours.csv')
    if not os.path.exists(map_path):
        raise FileNotFoundError(f'File {map_path} not found')
    map_data = gpd.read_file(map_path)

    if not os.path.exists(neighbours_path):
        print(f'File {neighbours_path} not found, creating it...')
        create_neighbours_file(map_data, neighbours_path)
    neighbours = pd.read_csv(neighbours_path)

    neighbours['neighbours'] = neighbours['neighbours'].apply(lambda x: convert_to_int_array(x))


    solver = Solver(map_data, neighbours, COLORS_NUMBER)
    if args.algorithm == 'BT':
        solver.bt()
    elif args.algorithm == 'BJ':
        solver.bj()
    elif args.algorithm == 'MAC-BT':
        solver.mac_bt()
    else:
        raise ValueError(f'Alogrithm {args.algorithm} not implemented')


    colormap = plt.cm.tab10(np.linspace(0, 1, COLORS_NUMBER))
    fig, ax = plt.subplots(figsize=(8, 6))
    map_data['color'] = [colormap[val] for val in map_data['value']]
    ax.clear()
    map_data.plot(color=map_data['color'], edgecolor='black', ax=ax)
    ax.set_title(f'Randomly Colored Map (Algorithm: {args.algorithm})')

    plt.show()

