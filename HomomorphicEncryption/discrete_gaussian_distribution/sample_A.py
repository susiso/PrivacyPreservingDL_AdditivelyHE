from sample import sample

def main():
	sampler = sample()
	sampler.sample_uniform("A", sampler.n_lwe, sampler.n_lwe, pow(2, 77), 2)

if __name__ == "__main__":
    main()