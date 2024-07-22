import random
import discretegauss
import numpy as np
import os
import time
from tqdm import tqdm

class sample():
	def __init__(self):
		self.dir_path = "sample"
		self.rng = random.SystemRandom()
		random.seed(0)
		self.s = 8
		self.n_lwe = 3000
		self.n_gd = 20000

	def sample_gauss(self, vec_name, row, col, seed=0):
		vec = np.zeros((row, col))		 
		start_time = time.time()
		for i in tqdm(range(row)):
			for j in tqdm(range(col), leave=False):
				vec[i, j] = discretegauss.sample_dgauss(self.s)
		end_time = time.time()
		np.savetxt(os.path.join(self.dir_path, f"{vec_name}.txt"), vec, fmt='%d')
		sample_time = end_time - start_time
		print(f"time_{vec_name}: {sample_time}")
  
	def sample_uniform(self, vec_name, row, col, num_max, seed=0):
		vec = np.zeros((row, col))		 
		start_time = time.time()
		for i in tqdm(range(row)):
			for j in tqdm(range(col), leave=False):
				vec[i, j] = discretegauss.sample_uniform(num_max, self.rng)
		end_time = time.time()
		np.savetxt(os.path.join(self.dir_path, f"{vec_name}.txt"), vec, fmt='%d')
		sample_time = end_time - start_time
		print(f"time_{vec_name}: {sample_time}")

def main():
	dir_path = "sample"
	os.makedirs(dir_path, exist_ok=True)
	rng = rng = random.SystemRandom()
	random.seed(0)
 
	s = 8
	n_lwe = 3000
	n_gd = 20000
	R = np.zeros((n_lwe, n_gd)) #  10716.48199 s
	S = np.zeros((n_lwe, n_gd)) #  10803.02530s
	A = np.zeros((n_lwe, n_lwe)) # 25.34763 s
	e_1 = np.zeros((1, n_lwe)) # 0.44916 s
	e_2 = np.zeros((1, n_lwe)) # 0.43628 s
	e_3 = np.zeros((1, n_gd)) # 2.90527 s
	m = np.zeros((1, n_gd)) # 0.07339 s
 
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
		e_1[0, j] = discretegauss.sample_dgauss(s)
	end_time_e_1 = time.time()
	np.savetxt(os.path.join(dir_path, "e_1.txt"), e_1, fmt='%d')
	time_e_1 = end_time_e_1 - start_time_e_1
	tqdm.write(f"time_e_1: {time_e_1}")

	start_time_e_2 = time.time()
	for j in tqdm(range(n_lwe)):
		e_2[0, j] = discretegauss.sample_dgauss(s)
	end_time_e_2 = time.time()
	np.savetxt(os.path.join(dir_path, "e_2.txt"), e_2, fmt='%d')
	time_e_2 = end_time_e_2 - start_time_e_2
	tqdm.write(f"time_e_2: {time_e_2}")

	start_time_e_3 = time.time()
	for j in tqdm(range(n_gd)):
		e_3[0, j] = discretegauss.sample_dgauss(s)
	end_time_e_3 = time.time() 
	np.savetxt(os.path.join(dir_path, "e_3.txt"), e_3, fmt='%d')
	time_e_3 = end_time_e_3 - start_time_e_3
	tqdm.write(f"time_e_3: {time_e_3}")

	start_time_m = time.time()
	for j in tqdm(range(n_gd)):
		m[0, j] = discretegauss.sample_uniform(pow(2, 77), rng)
	end_time_m = time.time() 
	np.savetxt(os.path.join(dir_path, "m.txt"), m, fmt='%d')
	time_m = end_time_m - start_time_m
	tqdm.write(f"time_m: {time_m}")

	with open(os.path.join(dir_path, 'time.txt'), mode='w') as f:
		f.write(time_R)
		f.write(time_S)
		f.write(time_A)
		f.write(time_e_1)
		f.write(time_e_2)
		f.write(time_e_3)
		f.write(time_m)

if __name__ == "__main__":
    main()