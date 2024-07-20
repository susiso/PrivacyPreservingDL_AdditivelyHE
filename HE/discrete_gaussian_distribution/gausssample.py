import random
import discretegauss
import numpy as np
import os
import time
from tqdm import tqdm

def main():
	dir_path = "sample"
	os.makedirs(dir_path, exist_ok=True)
 
	random.seed(0)
	s = 8
	n_lwe = 3000
	n_gd_list = [20000, 27882, 50000, 52650, 100000, 200000, 300000, 402250]
	for n_gd in n_gd_list:
		print(f"make n_gd={n_gd}")
		R = np.zeros((n_lwe, n_gd))
		S = np.zeros((n_lwe, n_gd))
		e_3 = np.zeros(n_gd)
		start_time_RS = time.time()
		for i in tqdm(range(n_lwe)):
			for j in tqdm(range(n_gd), leave=False):
				R[i, j] = discretegauss.sample_dgauss(s)
				S[i, j] = discretegauss.sample_dgauss(s)
		end_time_RS = time.time()
		start_time_e_3 = time.time()
		for j in tqdm(range(n_gd)):
			e_3[j] = discretegauss.sample_dgauss(s)
		end_time_e_3 = time.time()
		gd_path = os.path.join(dir_path, f"n_gd_{n_gd}")
		os.makedirs(gd_path, exist_ok=True)
		np.savetxt(os.path.join(gd_path, "R.txt"), R)
		np.savetxt(os.path.join(gd_path, "S.txt"), S)
		np.savetxt(os.path.join(gd_path, "e_3.txt"), e_3)
		RS_time = end_time_RS - start_time_RS
		e_3_time = end_time_e_3 - start_time_e_3
		tqdm.write(f"{n_gd}: \n RS_time: {RS_time} \n e_3_time {e_3_time}")

if __name__ == "__main__":
    main()