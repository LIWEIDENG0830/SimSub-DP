import pickle
import traj_dist.distance as tdist
import time
import numpy as np
from pathlib import Path

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

if __name__ == "__main__":

    # chengdu
    datapath = './data/chengdu/traj_all_20_400'
    datapath_pickle = './data/chengdu/traj_all_20_400.pkl'
    n_queries, n_data = 1000, 10000
    dist_path = './features/chengdu_dtw_distance_all_1000'
    distance_type = 'dtw'
    comp_path = './compressed/ours/chengdu_compressed_traj.pkl'

    st_time = time.time()
    if not Path(datapath_pickle).exists():
        trajs = load_trajs(datapath)
        save_pickle(trajs, datapath_pickle)
    else:
        trajs = load_pickle(datapath_pickle)
    # query_trajs, data_trajs = trajs[:n_queries], trajs[n_queries:n_queries + n_data]
    trajs = [traj for traj in trajs[:n_queries+n_data]]
    ed_time = time.time()
    print('Data Loading Time Cost:', ed_time - st_time)

    print(distance_type)
    print(dist_path)
    print(comp_path)
    
    k_list = [5, 10, 50]

    ground_dists = load_pickle(dist_path)
    comp_trajs_dict = load_pickle(comp_path)

    print(comp_trajs_dict.keys())

    sorted_ground_idx = np.argsort(ground_dists, axis=-1)

    for comp_ratio in comp_trajs_dict:
        comp_trajs = [np.array(traj) for traj in comp_trajs_dict[comp_ratio]]
        query_trajs, data_trajs = comp_trajs[:n_queries], comp_trajs[n_queries:]
        st_time = time.time()
        comp_dists = tdist.cdist(query_trajs, data_trajs, metric=distance_type)
        ed_time = time.time()
        print('Compression ratio: {}, Distance Computation Cost: {}'.format(comp_ratio, ed_time-st_time))
        sorted_comp_idx = np.argsort(comp_dists, axis=-1)

        acck_dict = {}
        for k in k_list:
            acck_all = 0
            for i in range(n_queries):
                acck = len((set(sorted_comp_idx[i][:k]) & set(sorted_ground_idx[i][:k]))) / k
                acck_all += acck
            acck_dict[k] = acck_all / n_queries
        print(acck_dict)