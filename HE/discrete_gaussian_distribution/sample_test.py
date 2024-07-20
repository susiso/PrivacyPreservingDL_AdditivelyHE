from sample import sample

def main():
	sampler = sample()
	sampler.sample_gauss("test_sample", 3, 3, 0)
	sampler.sample_uniform("test_uniform", 3, 3, 0)

if __name__ == "__main__":
    main()