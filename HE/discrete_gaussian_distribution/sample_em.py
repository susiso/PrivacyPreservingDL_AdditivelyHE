from sample import sample

def main():
	sampler = sample()
	sampler.sample_gauss("e_1", 1, sampler.n_lwe, 3)
	sampler.sample_gauss("e_2", 1, sampler.n_lwe, 4)
	sampler.sample_gauss("e_3", 1, sampler.n_gd, 5)
	sampler.sample_uniform("m", 1, sampler.n_gd, 6)

if __name__ == "__main__":
    main()