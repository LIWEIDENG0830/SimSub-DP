# This file is used to extract the simplified trajectory from our alg
from pathlib import Path
import time
import pickle
from tqdm import tqdm

def load_pickle(filepath):
    with open(filepath, 'rb') as f:
        return pickle.load(f)

def save_pickle(obj, filepath):
    with open(filepath, 'wb') as f:
        pickle.dump(obj, file=f)

def load_trajs(filepath):
    trajs = []
    idx = 0
    with open(filepath) as f:
        for line in f:
            items = line.strip().split()
            if len(items) == 1:
                if idx != 0:
                    trajs.append(traj)
                traj = []
            else:
                traj.append([float(items[1]), float(items[2])])
            idx += 1
        trajs.append(traj)
    return trajs

if __name__ == '__main__':

    comp_dir = Path('/data1/dlw/trajectory/trajcSimilar_M/ExpComp/chengdu/')
    save_path = './compressed/ours/chengdu_compressed_traj.pkl'
    datapath = './data/chengdu/traj_all_20_400'
    datapath_pickle = './data/chengdu/traj_all_20_400.pkl'
    n_queries, n_data = 1000, 10000
    filetemp = 'trajc_idx_{}.txt'

    # Full data

    st_time = time.time()
    if not Path(datapath_pickle).exists():
        trajs = load_trajs(datapath)
        save_pickle(trajs, datapath_pickle)
    else:
        trajs = load_pickle(datapath_pickle)

    comp_trajs = {}
    for i in range(n_queries+n_data):
        filepath = comp_dir / filetemp.format(i)
        with open(filepath) as f:
            lines = f.readlines()
            for j in range(int(len(lines)/2)):
                comp_ratio = lines[j*2].strip()
                points_idx = list(map(int,lines[j*2+1].strip().strip(',').split(',')))
                comp_traj = [trajs[i][idx] for idx in points_idx]
                if comp_ratio not in comp_trajs:
                    comp_trajs[comp_ratio] = []
                comp_trajs[comp_ratio].append(comp_traj)
    
    # Put the extracted comp_trajectories into files
    save_pickle(comp_trajs, save_path)