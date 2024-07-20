import random
import discretegauss
import numpy as np
import os
import time
from tqdm import tqdm

def main():
	dir_path = "sample"
	os.makedirs(dir_path, exist_ok=True)
	rng = rng = random.SystemRandom()
	random.seed(0)
 
	s = 8
	n_lwe = 3000
	n_gd = 402250
	R = np.zeros((n_lwe, n_gd))
	S = np.zeros((n_lwe, n_gd))
	A = np.zeros((n_lwe, n_lwe))
	e_1 = np.zeros((1, n_lwe))
	e_2 = np.zeros((1, n_lwe))
	e_3 = np.zeros((1, n_gd))
 
	start_time_R = time.time()
	for i in tqdm(range(n_lwe)):
		for j in tqdm(range(n_gd), leave=False):
			R[i, j] = discretegauss.sample_dgauss(s)
	end_time_R = time.time()
	np.savetxt(os.path.join(dir_path, "R.txt"), R, fmt='%d')
	time_R = end_time_R - start_time_R
	tqdm.write(f"time_R: {time_R}")
 
	start_time_S = time.time()
	for i in tqdm(range(n_lwe)):
		for j in tqdm(range(n_gd), leave=False):
			S[i, j] = discretegauss.sample_dgauss(s)
	end_time_S = time.time()
	np.savetxt(os.path.join(dir_path, "S.txt"), S, fmt='%d')
	time_S = end_time_S - start_time_S
	tqdm.write(f"time_S: {time_S}")

	start_time_A = time.time()
	for i in tqdm(range(n_lwe)):
		for j in tqdm(range(n_lwe), leave=False):
			A[i, j] = discretegauss.sample_uniform(pow(2, 77), rng)
	end_time_A = time.time()
	np.savetxt(os.path.join(dir_path, "A.txt"), A, fmt='%d')
	time_A = end_time_A - start_time_A
	tqdm.write(f"time_A: {time_A}")
 
	start_time_e_1 = time.time()
	for j in tqdm(range(n_lwe)):
		e_1[j] = discretegauss.sample_dgauss(s)
	end_time_e_1 = time.time()
	np.savetxt(os.path.join(dir_path, "e_1.txt"), e_1, fmt='%d')
	time_e_1 = end_time_e_1 - start_time_e_1
	tqdm.write(f"time_e_1: {time_e_1}")

	start_time_e_2 = time.time()
	for j in tqdm(range(n_lwe)):
		e_2[j] = discretegauss.sample_dgauss(s)
	end_time_e_2 = time.time()
	np.savetxt(os.path.join(dir_path, "e_2.txt"), e_2, fmt='%d')
	time_e_2 = end_time_e_2 - start_time_e_2
	tqdm.write(f"time_e_2: {time_e_2}")

	start_time_e_3 = time.time()
	for j in tqdm(range(n_gd)):
		e_3[j] = discretegauss.sample_dgauss(s)
	end_time_e_3 = time.time() 
	np.savetxt(os.path.join(dir_path, "e_3.txt"), e_3, fmt='%d')
	time_e_3 = end_time_e_3 - start_time_e_3
	tqdm.write(f"time_e_3: {time_e_3}")

	with open(os.path.join(dir_path, 'time.txt'), mode='w') as f:
		f.write(time_R)
		f.write(time_S)
		f.write(time_A)
		f.write(time_e_1)
		f.write(time_e_2)
		f.write(time_e_3)

if __name__ == "__main__":
    main()