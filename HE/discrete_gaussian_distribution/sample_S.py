from sample import sample

def main():
	sampler = sample()
	sampler.sample_gauss("S", sampler.n_lwe, sampler.n_gd, 1)

if __name__ == "__main__":
    main()