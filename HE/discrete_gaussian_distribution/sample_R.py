from sample import sample

def main():
	sampler = sample()
	sampler.sample_gauss("R", sampler.n_lwe, sampler.n_gd, 0)

if __name__ == "__main__":
    main()