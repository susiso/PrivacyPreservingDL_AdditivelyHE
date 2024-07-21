from sample import sample

def main():
	sampler = sample()
	sampler.sample_gauss("e_1_a", 1, sampler.n_lwe, 3)
	sampler.sample_gauss("e_2_a", 1, sampler.n_lwe, 4)
	sampler.sample_gauss("e_3_a", 1, sampler.n_gd, 5)
	sampler.sample_gauss("e_1_b", 1, sampler.n_lwe, 6)
	sampler.sample_gauss("e_2_b", 1, sampler.n_lwe, 7)
	sampler.sample_gauss("e_3_b", 1, sampler.n_gd, 8)
	sampler.sample_uniform("m", 1, sampler.n_gd, 9)

if __name__ == "__main__":
    main()