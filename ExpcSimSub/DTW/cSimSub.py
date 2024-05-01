import pickle
import numpy as np
import time

def trace_back(DP_list):
    i = DP_list[-1].shape[0] - 1
    j = np.argmin(DP_list[-1][-1])
    traj_idx = [j]
    DP_idx = len(DP_list) - 1
    while i > 0 and j > 0:
        cur_DP = DP_list[DP_idx]
        if DP_idx == 0:
            # Check three ways on current DP
            three_way = np.argmin([cur_DP[i-1, j], cur_DP[i, j-1], cur_DP[i-1, j-1]])
            if three_way == 0: # Down
                i = i - 1
            elif three_way == 1: # Left
                j = j - 1
                traj_idx.append(j)
            else:
                i = i - 1
                j = j - 1
                traj_idx.append(j)
        else:
            prev_DP = DP_list[DP_idx-1]
            three_way = np.argmin([cur_DP[i-1, j], prev_DP[i, j-1], prev_DP[i-1, j-1]])
            if three_way == 0:
                i = i - 1
            elif three_way == 1:
                j = j - 1
                DP_idx = DP_idx - 1
                traj_idx.append(j)
            else:
                i = i - 1
                j = j - 1
                DP_idx = DP_idx - 1
                traj_idx.append(j)
    sub_traj = [traj_idx[-1], traj_idx[0]]
    return sub_traj

def fastDTWSub(traj_data, traj_query, prev_DP, min_len):
    '''
    '''
    m = len(traj_data)
    n = len(traj_query)
    DP = np.zeros((n, m))
    if min_len == 1:
        ''' Initialize the first column '''

        p_dist = np.linalg.norm(traj_query[0] - traj_data[0])
        DP[0, 0] = p_dist
        for i in range(1, n):
            p_dist = np.linalg.norm(traj_query[i] - traj_data[0])
            DP[i, 0] = p_dist + DP[i-1, 0]
        for j in range(1, m):
            p_dist = np.linalg.norm(traj_query[0] - traj_data[j])
            DP[0, j] = p_dist
        
        for i in range(1, n):
            for j in range(1, m):
                p_dist = np.linalg.norm(traj_query[i] - traj_data[j])
                DP[i, j] = min(DP[i-1, j], DP[i, j-1], DP[i-1, j-1]) + p_dist
    else:
        assert prev_DP is not None
        # Initial the min_len columns
        DP[:, :min_len-1] = np.inf
        # Initial the first row
        for i in range(min_len-1, m):
            p_dist = np.linalg.norm(traj_query[0] - traj_data[i])
            DP[0, i] = prev_DP[0, i-1] + p_dist
        # Dynamic programming
        for i in range(1, n):
            for j in range(min_len-1, m):
                p_dist = np.linalg.norm(traj_query[i] - traj_data[j])
                DP[i, j] = min(DP[i-1, j], prev_DP[i-1, j-1], prev_DP[i, j-1]) + p_dist

    return DP
    
def cSimSubDP(traj_c, traj_q, c_length):
    '''
    solving constrained SimSub problem based on dynamic programming
    '''
    DP = None
    DP_list = []
    for min_len in range(1, c_length+1):
        DP = fastDTWSub(traj_c, traj_q, DP, min_len)
        DP_list.append(DP)
    subsim = np.min(DP_list[-1][-1])
    subtraj = trace_back(DP_list)
    return subsim, subtraj

def load_pickle(filepath):
    with open(filepath, 'rb') as f:
        return pickle.load(f)

def save_pickle(obj, filepath):
    with open(filepath, 'wb') as f:
        pickle.dump(obj, file=f)

if __name__ == "__main__":

    # save_path = './results/porto_DTW_cSimSub.pkl'
    # cand_test_path = '../data/porto/cand_test.pkl'
    # query_test_path = '../data/porto/query_test.pkl'

    save_path = './results/chengdu_DTW_cSimSub.pkl'
    cand_test_path = '../data/chengdu/cand_test.pkl'
    query_test_path = '../data/chengdu/query_test.pkl'
    c_lengths = [1, 2, 3, 4, 5, 6, 7]
    results_record = {'Time': [], 'C_length': [], 'subsim': [], 'subtrajs': []}

    cand_trajs = [np.array(traj) for traj in load_pickle(cand_test_path)][:5]
    query_trajs = [np.array(traj) for traj in load_pickle(query_test_path)][:5]

    for c_length in c_lengths:
        subtrajs = []
        subsims = []
        st_time = time.time()
        for traj_Q, traj_C in zip(query_trajs, cand_trajs):
            subsim, subtraj = cSimSubDP(traj_C, traj_Q, c_length)
            subsims.append(subsim)
            subtrajs.append(subtraj)
        ed_time = time.time()
        results_record['Time'].append(ed_time - st_time)
        results_record['C_length'].append(c_length)
        results_record['subtrajs'].append(subtrajs)
        results_record['subsim'].append(subsims)

        print('N Samples: ', len(query_trajs))
        print('C_length: ', c_length)
        print('cExactS cost: ', ed_time - st_time)

    save_pickle(results_record, save_path)