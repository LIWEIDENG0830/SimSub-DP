# SimSub-DP

## Introduction

This is the official implementation of the paper, titled "Exact and Efficient Similar Subtrajectory Search: Integrating Constraints and Simplification", submitted to ICDE2025.

## Structure
This project consists of two components, i.e., ExpcSimSub and ExpsSimSub, which corresponds to the two problems solved in our study, in which cSimSub refers to the Constrained SimSub problem and sSimSub refers to SimSub for Trajectory Simplification. The former will return the most similar subtrajectory from the data trajectory, where the length of the returned subtrajectory will be no less than a user-specified integer C. The latter is to perform trajectory simplification, in which the simplified trajectory achieves the best self-similarity to itself.

For more details about the SimSub problem, we refer the reader to https://github.com/zhengwang125/SimSub for more details.

## Dataset
Three real-world trajectory datasets are used in our paper, i.e., ChengDu, Porto, and Geolife, in which ChengDu may not be publicly avaiable due to DiDi's policy. The other two datasets can be downloaded through the following links.

```
Porto: https://www.kaggle.com/datasets/crailtap/taxi-trajectory
Geolife: https://www.microsoft.com/en-us/research/publication/geolife-gps-trajectory-dataset-user-guide/
```


## Running Procedures

### Running cSimSub
In this repo, we implement the solution of cSimSub under DTW and Frechet measurements. It should be noted that for the most trajectory distance as long as it can be computed through dynamic programming, our algorithm can work with it. If you want to run with the other trajectory distance measurements, you can implement it with yourself or directly contact with us. We will be happy to enrich our repo.

To reproduce the results in our paper, after obtaining the data, you should randomly sample a set of trajectory pairs and store them as pickle file, in which one trajectory is for query and the other trajectory is for data.

To run our algorithm under DTW distance, you should go to the 'ExpcSimSub/DTW/' directory first, modify the data path in 'cSimSub.py' (around in lines 107-108), specify an integer you like (around in line 109), and run the following command.

```
python cSimSub.py
```

Defaultly, the searched most similar subtrajectory will be saved in 'ExpcSimSub/DTW/results/', which can be modified in line 106.

### Running sSimSub
Similar to the solutions in the cSimSub problem, we also implement the solution of sSimSub under DTW and Frechet measurements. To enrich the implementation for more trajectory distance, you can directly contact with us.

In this repo, we implement our solution for sSimSub in C++ considering its efficiency, i.e., we have the time complexity of $O(Cmn)$ to achieve trajectory simplification. By the way, if the python implementation you want to obtain, please contact with us or implement it by yourself.

To reproduce the results reported in our paper, you should prepare the data to the format we need, i.e., we have the same data format requirement with https://github.com/inabao/trajcSimilar, compile and run the C++ codes with the following command.

```
cd ExpsSimSub/sSimSub/
cmake .
make
./application
```

To evaluate the effectiveness of the simplified trajectories, you should compute the ground-truth distances before simplification and save them as a pickle file (i.e., we do not implement an indexing to speed up the searching process, thus a full distance matrix between trajectories is needed. You can modify this part for fast evaluation). Then, you should read the simplified trajectories into a single pickle file through the following command.

```
python simptraj_readout.py
```

After that, run the following command will output the KNN query results.

```
python knn_evaluate.py
```

## Contact

If you have any question about our implementation or our algorithm, please do not hesitate to contact with us. Proposing issues and emailing are both welcome.

```
Liwei Deng: deng_liwei@std.uestc.edu.cn or denglw0830@gmail.com
```

## Acknowledge
Our implementations are inspired or based by the following repos. Thanks for their great contributions. 

```
https://github.com/zhengwang125/SimSub
https://github.com/inabao/trajcSimilar
https://github.com/zhengwang125/RLTS
https://github.com/zhengwang125/Query-TS
```

If you think our code is helpful, please consider to cite the following papers (corresponding to the repos above).

```
@article{Wang2020EfficientAE,
  title={Efficient and effective similar subtrajectory search with deep reinforcement learning},
  author={Zheng Wang and Cheng Long and G. Cong and Yiding Liu},
  journal={VLDB},
  year={2020},
  volume={13},
  pages={2312 - 2325},
}
@article{Jin2023EfficientNS,
  title={Efficient Non-Learning Similar Subtrajectory Search},
  author={Jiabao Jin and Peng Cheng and Lei Chen and Xuemin Lin and Wenjie Zhang},
  journal={VLDB},
  year={2024},
}
@article{Wang2021TrajectorySW,
  title={Trajectory Simplification with Reinforcement Learning},
  author={Zheng Wang and Cheng Long and Gao Cong},
  journal={ICDE},
  year={2021},
  pages={684-695},
}
@article{Wang2023CollectivelyST,
  title={Collectively Simplifying Trajectories in a Database: A Query Accuracy Driven Approach},
  author={Zheng Wang and Cheng Long and Gao Cong and Christian S. Jensen},
  journal={ICDE24},
  year={2023},
}
```
